#include "Lexer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "Parser.h"
#include "Relation.h"
#include "Database.h"
#include "Graph.h"

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

    //TODO Project 5

    //FIXME delete: graph testing
#if 0
    std::cout << "GRAPH TEST" << std::endl;
    Graph* testGraph = new Graph();
    std::set<int> adjacentNodes1;
    int node1 = 0;
    adjacentNodes1.insert(2);
    adjacentNodes1.insert(3);
    testGraph->AddNode(node1, adjacentNodes1);
    std::set<int> adjacentNodes2;
    int node2 = 1;
    adjacentNodes2.insert(0);
    adjacentNodes2.insert(3);
    testGraph->AddNode(node2, adjacentNodes2);
    std::set<int> adjacentNodes3;
    int node3 = 4;
    adjacentNodes3.insert(0);
    testGraph->AddNode(node3, adjacentNodes3);
    std::set<int> adjacentNodes4;
    int node4 = 5;
    testGraph->AddNode(node4, adjacentNodes4);
    std::cout << testGraph->ToString() << std::endl;

    delete testGraph;
#endif

    std::vector<Rule> rules = program.getRules();
    // build dependency graph
    Graph* dependencyGraph = new Graph();
    for (int i = 0; i < int(rules.size()); ++i) {
        std::set<int> dependencies;
        for (Predicate predicate: rules.at(i).getBody()) {
            std::string id = predicate.getID();
            for (int k = 0; k < int(rules.size()); ++k) {
                if (rules.at(k).getHead().getID() == id) dependencies.insert(k);
            }
        }
        dependencyGraph->AddPair(i, dependencies);
    }
    std::cout << "Dependency Graph" << std::endl;
    std::cout << dependencyGraph << std::endl;

    // reverse dependency graph
    Graph* reverseGraph = dependencyGraph->Reverse();
    //std::cout << "Reverse Graph" << std::endl; //FIXME delete
    //std::cout << reverseGraph << std::endl; //FIXME delete

    // DFS Forest on reverse graph
    std::vector<int> inOrder;
    for (auto pair : reverseGraph->GetAdjacencyList()) inOrder.push_back(pair.first);
    reverseGraph->DFS_Forest(inOrder);

    //std::cout << "Post Order" << std::endl; //FIXME delete
    std::vector<int> postOrder = reverseGraph->GetPostOrder();
#if 0 //FIXME delete
    for (int i : postOrder) std::cout << i << " ";
    std::cout << std::endl;
#endif

    //std::cout << "Reverse Post Order" << std::endl; //FIXME delete
    std::vector<int> reversePostOrder = postOrder;
    std::reverse(reversePostOrder.begin(), reversePostOrder.end());
#if 0 //FIXME delete
    for (int i : reversePostOrder) std::cout << i << " ";
    std::cout << std::endl;
#endif

    //std::cout << "Strongly Connected Components" << std::endl; //FIXME delete
    dependencyGraph->DFS_Forest(reversePostOrder);
    std::map<int, std::set<int>> SCCs = dependencyGraph->GetSCCs();
#if 0 //FIXME delete
    for (auto SCC : SCCs) {
        std::cout << SCC.first << ": ";
        for (int i : SCC.second) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
#endif

    // evaluate the rules in each component
    std::cout << "Rule Evaluation" << std::endl;
     for (auto SCC : SCCs) {
        std::cout << "SCC: ";
        auto iter = SCC.second.begin();
        std::cout << "R" << *iter;
        ++iter;
        while (iter != SCC.second.end()) {
            std::cout << ",R" << *iter;
            ++iter;
        }
        std::cout << std::endl;
        bool repeat = true;
        int numPasses = 0;
        // determine whether the SCC is "independent" (contains only one rule and that rule does not depend on itself)
        bool independent = true;
        if (SCC.second.size() == 1) {
            for (int ruleNumber: SCC.second) {
                for (Predicate p : rules.at(ruleNumber).getBody()) {
                    if (p.getID() == rules.at(ruleNumber).getHead().getID()) independent = false;
                }
            }
        }
        else independent = false;
        while (repeat) {
            ++numPasses;
            repeat = false;
            for (auto ruleNumber: SCC.second) {
                Rule rule = rules.at(ruleNumber);
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
                if (relationInDatabase->Union(joinedRelations)) {
                    repeat = true;
                    // output new tuples
                    std::set<Tuple> newTuples;
                    for (Tuple t: relationInDatabase->getTuples()) {
                        if (oldTuples.count(t) == 0) {
                            newTuples.insert(t);
                        }
                    }
                    std::vector<std::string> originalAttributes = relationInDatabase->getHeader().getAttributes();
                    std::vector<int> uniqueAttributesIndices;
                    // populate uniqueAttributesIndices
                    for (int i = 0; i < int(originalAttributes.size()); ++i) {
                        // if there are no duplicates of the current attribute before the current attribute
                        if (std::count(originalAttributes.begin(), originalAttributes.begin() + i,
                                       originalAttributes.at(i)) == 0) {
                            uniqueAttributesIndices.push_back(i);
                        }
                    }
                    // project unique attributes
                    relationInDatabase = relationInDatabase->Project(uniqueAttributesIndices);
                    for (Tuple t: newTuples) {
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
                if (independent) repeat = false;
            }
        }
        std::cout << numPasses << " passes: ";
        iter = SCC.second.begin();
        std::cout << "R" << *iter;
        ++iter;
        while (iter != SCC.second.end()) {
            std::cout << ",R" << *iter;
            ++iter;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    //FIXME Project 4 code
#if 0
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
#endif

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
    delete dependencyGraph;

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
