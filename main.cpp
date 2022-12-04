#include "Lexer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "Parser.h"
#include "Relation.h"
#include "Database.h"

Relation* EvaluatePredicate(Database* database, Predicate predicate);

std::string readFileIntoString(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
    // Open input file
    if (argc < 2) {
        std::cerr << "Please provide name of input file";
        return 1;
    }
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open " << argv[1] << " for input";
        return 2;
    }

    std::string input;

    input = readFileIntoString(argv[1]);

    Lexer *lexer = new Lexer();
    lexer->Run(input);
    std::vector<Token *> tokens = lexer->getTokens();

    // Remove comment tokens
    for (int i = tokens.size() - 1; i >= 0; --i) {
        if (tokens.at(i)->getTokenType() == TokenType::COMMENT) {
            tokens.erase(tokens.begin() + i);
        }
    }

    Parser *parser = new Parser(tokens);
    try {
        parser->ParseDatalogProgram();
    }
    catch (Token *currToken) {
        std::cout << "Failure!\n   " << currToken;
        return 3;
    }

    DatalogProgram program = parser->getProgram();
    Database *database = new Database;

    // Iterate through schemes and create database
    std::vector<Predicate> schemes = program.getSchemes();
    for (Predicate scheme: schemes) {
        // Create header
        std::vector<std::string> attributes;
        for (Parameter parameter: scheme.getParameters()) {
            attributes.push_back(parameter.getID());
        }
        Header header(attributes);
        // Add new relation to database
        Relation *relation = new Relation(scheme.getID(), header);
        database->SetTable(scheme.getID(), relation);
    }

    // Iterate through facts and add tuples to the relations
    std::vector<Predicate> facts = program.getFacts();
    for (Predicate fact: facts) {
        // Lookup the relation
        Relation *relation = database->GetTable(fact.getID());
        Tuple t;
        for (Parameter parameter: fact.getParameters()) {
            t.Append(parameter.getID());
        }
        // Add new tuple
        relation->AddTuple(t);
    }

    // Iterate through rules and evaluate each rule
    std::cout << "Rule Evaluation" << std::endl;
    std::vector<Rule> rules = program.getRules();
    bool databaseChanges = true;
    int numIterations = 0;
    while (databaseChanges) {
        databaseChanges = false;
        ++numIterations;
        for (Rule rule: rules) {
            std::cout << rule << "." << std::endl;
            std::vector<Predicate> predicates = rule.getBody();
            Relation *joinedRelations = new Relation;
            // if there are any predicates, evaluate the first predicate
            if (predicates.size() > 0) {
                Predicate predicate = predicates.at(0);
                // evaluate first predicate
                Relation *relation = EvaluatePredicate(database, predicate);
                // set joinedRelations to the first relation
                joinedRelations = relation;
            }
            // iterate through the rest of the predicates, evaluate them, and join them
            for (int i = 1; i < int(predicates.size()); ++i) {
                Predicate predicate = predicates.at(i);
                // evaluate predicate
                Relation *relation = EvaluatePredicate(database, predicate);
                joinedRelations = joinedRelations->Join(relation);
            }
            // get the attributes in the head predicate
            std::vector<std::string> headAttributes;
            for (Parameter parameter: rule.getHead().getParameters()) {
                headAttributes.push_back(parameter.getID());
            }
            // get the indices of the head attributes from the joinedRelations attributes
            std::vector<int> indices;
            // iterate through head attributes
            for (std::string headAttribute: headAttributes) {
                // iterate through joinedRelations attributes (with index)
                for (int i = 0; i < int(joinedRelations->getHeader().getAttributes().size()); ++i) {
                    // if the current attribute in joinedRelations attributes is a headAttribute
                    if (joinedRelations->getHeader().getAttributes().at(i) == headAttribute) {
                        // save the index for projecting
                        indices.push_back(i);
                    }
                }
            }
            // project the columns that appear in the head predicate
            joinedRelations = joinedRelations->Project(indices);
            // rename to match the relation in database that matches the head
            std::vector<std::string> newAttributes = database->GetTable(
                    rule.getHead().getID())->getHeader().getAttributes();
            joinedRelations = joinedRelations->Rename(newAttributes);
            // union with the relation in the database
            Relation *relationInDatabase = database->GetTable(rule.getHead().getID());
            // keep track of current tuples (before union)
            std::set<Tuple> oldTuples = relationInDatabase->getTuples();
            if(relationInDatabase->Union(joinedRelations)) {
                databaseChanges = true;
                // output new tuples
                std::set<Tuple> newTuples;
                for (Tuple t : relationInDatabase->getTuples()) {
                    if (oldTuples.count(t) == 0) {
                        newTuples.insert(t);
                    }
                }
                std::vector<std::string> originalAttributes = relationInDatabase->getHeader().getAttributes();
                std::vector<int> uniqueAttributesIndices;
                // populate uniqueAttributesIndices
                for (int i = 0; i < int(originalAttributes.size()); ++i) {
                    // if there are no duplicates of the current attribute before the current attribute
                    if (std::count(originalAttributes.begin(), originalAttributes.begin() + i, originalAttributes.at(i)) == 0) {
                        uniqueAttributesIndices.push_back(i);
                    }
                }
                // project unique attributes
                relationInDatabase = relationInDatabase->Project(uniqueAttributesIndices);
                for (Tuple t : newTuples) {
                    std::vector<std::string> attributes = relationInDatabase->getHeader().getAttributes();
                    if (attributes.size() > 0) {
                        std::cout << "  " << attributes.at(0) << "=" << t.getValues().at(0);
                        for (int i = 1; i < int(attributes.size()); ++i) {
                            std::cout << ", " << attributes.at(i) << "=" << t.getValues().at(i);
                        }
                        std::cout << std::endl;
                    }
                }
            }
        }
    }
    std::cout << "\nSchemes populated after " << numIterations << " passes through the Rules.\n\n";

    // Iterate through queries and evaluate each query
    std::cout << "Query Evaluation" << std::endl;
    std::vector<Predicate> queries = program.getQueries();
    for (Predicate query : queries) {
        Relation* relation = EvaluatePredicate(database, query);
        // output query and answer
        std::cout << query << "?";
        // if relation isn't empty
        if (!relation->IsEmpty()) {
            std::cout << " Yes(" << relation->Size() << ")" << std::endl;
            std::vector<std::string> originalAttributes = relation->getHeader().getAttributes();
            std::vector<int> uniqueAttributesIndices;
            // populate uniqueAttributesIndices
            for (int i = 0; i < int(originalAttributes.size()); ++i) {
                // if there are no duplicates of the current attribute before the current attribute
                if (std::count(originalAttributes.begin(), originalAttributes.begin() + i, originalAttributes.at(i)) == 0) {
                    uniqueAttributesIndices.push_back(i);
                }
            }
            // project unique attributes
            relation = relation->Project(uniqueAttributesIndices);
            for (Tuple t : relation->getTuples()) {
                std::vector<std::string> attributes = relation->getHeader().getAttributes();
                if (attributes.size() > 0) {
                    std::cout << "  " << attributes.at(0) << "=" << t.getValues().at(0);
                    for (int i = 1; i < int(attributes.size()); ++i) {
                        std::cout << ", " << attributes.at(i) << "=" << t.getValues().at(i);
                    }
                    std::cout << std::endl;
                }
            }
        }
        // relation is empty
        else {
            std::cout << " No" << std::endl;
        }
    }

    delete lexer;
    delete parser;

    return 0;
}

Relation* EvaluatePredicate(Database* database, Predicate predicate) {
    Relation* relation = database->GetTable(predicate.getID());
    std::vector<std::string> newAttributes; // for Rename
    std::vector<int> newIndices; // for Project
    std::vector<Parameter> parameters = predicate.getParameters();
    std::vector<std::string> parameterIDs;
    for (Parameter p : parameters) {
        parameterIDs.push_back(p.getID());
    }
    for (int i = 0; i < int(parameterIDs.size()); ++i) {
        std::string id = parameterIDs[i];
        if (id[0] == '\'') {
            relation = relation->Select(i, id);
        }
            // id is an "ID" (variable)
        else {
            // if there are matching variables
            if (count(parameterIDs.begin(), parameterIDs.end(), id) > 1) {
                // select all the rows of the relation that have the match
                for (int k = i; k < int(parameterIDs.size()); ++k) {
                    if (parameterIDs.at(i) == parameterIDs.at(k)) {
                        relation = relation->Select(i, k);
                    }
                }
            }
        }
        // add parameterID to new attributes (for Rename) and add index to new indices (for Project)
        // if parameter is a variable (ID)
        if (parameterIDs.at(i)[0] != '\'') {
            // get new attribute at that index (for Rename)
            newIndices.push_back(i);
            newAttributes.push_back(parameterIDs.at(i));
        }
            // parameter is a constant (STRING)
        else {
            // keep old attribute at that index (for Rename)
            std::string key = predicate.getID();
            newAttributes.push_back(database->GetTable(key)->getHeader().getAttributes().at(i));
        }
    }
    relation = relation->Rename(newAttributes);
    relation = relation->Project(newIndices);
    return relation;
}
