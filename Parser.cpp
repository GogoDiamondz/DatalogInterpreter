#include "Parser.h"

/*
datalogProgram	->	SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON ruleList QUERIES COLON query queryList EOF
scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN
idList  	-> 	COMMA ID idList | lambda
schemeList	->	scheme schemeList | lambda
factList	->	fact factList | lambda
fact    	->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
stringList	-> 	COMMA STRING stringList | lambda
ruleList	->	rule ruleList | lambda
rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD
headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
parameter	->	STRING | ID
parameterList	-> 	COMMA parameter parameterList | lambda
predicateList	->	COMMA predicate predicateList | lambda
query    ->  predicate Q_MARK
queryList	->	query queryList | lambda
*/

std::string Parser::tokenTypeToString(TokenType t) const {
    std::string tokenTypeString;
    switch(t) {
        case TokenType::COLON :
            tokenTypeString = "COLON";
            break;
        case TokenType::COLON_DASH :
            tokenTypeString = "COLON_DASH";
            break;
        case TokenType::UNDEFINED :
            tokenTypeString = "UNDEFINED";
            break;
        case TokenType::COMMA :
            tokenTypeString = "COMMA";
            break;
        case TokenType::PERIOD :
            tokenTypeString = "PERIOD";
            break;
        case TokenType::Q_MARK :
            tokenTypeString = "Q_MARK";
            break;
        case TokenType::LEFT_PAREN :
            tokenTypeString = "LEFT_PAREN";
            break;
        case TokenType::RIGHT_PAREN :
            tokenTypeString = "RIGHT_PAREN";
            break;
        case TokenType::MULTIPLY :
            tokenTypeString = "MULTIPLY";
            break;
        case TokenType::ADD :
            tokenTypeString = "ADD";
            break;
        case TokenType::SCHEMES :
            tokenTypeString = "SCHEMES";
            break;
        case TokenType::FACTS :
            tokenTypeString = "FACTS";
            break;
        case TokenType::RULES :
            tokenTypeString = "RULES";
            break;
        case TokenType::QUERIES :
            tokenTypeString = "QUERIES";
            break;
        case TokenType::ID :
            tokenTypeString = "ID";
            break;
        case TokenType::STRING :
            tokenTypeString = "STRING";
            break;
        case TokenType::COMMENT :
            tokenTypeString = "COMMENT";
            break;
        case TokenType::END :
            tokenTypeString = "EOF";
            break;
    }
    return tokenTypeString;
}

void Parser::Match(TokenType to_match) {
    if (tokens.at(index)->getTokenType() == to_match) {
        index++;
    }
    else {
        throw(tokens.at(index));
    }
}

// datalogProgram	->	SCHEMES COLON scheme schemeList FACTS COLON factList RULES COLON ruleList QUERIES COLON query queryList EOF
void Parser::ParseDatalogProgram() {
    Match(TokenType::SCHEMES);
    Match(TokenType::COLON);
    ParseScheme();
    ParseSchemeList();
    Match(TokenType::FACTS);
    Match(TokenType::COLON);
    ParseFactList();
    Match(TokenType::RULES);
    Match(TokenType::COLON);
    ParseRuleList();
    Match(TokenType::QUERIES);
    Match(TokenType::COLON);
    ParseQuery();
    ParseQueryList();
    Match(TokenType::END);
    CreateProgram();
}

// scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN
void Parser::ParseScheme() {
    Match(TokenType::ID);
    Match(TokenType::LEFT_PAREN);
    Match(TokenType::ID);
    ParseIdList();
    Match(TokenType::RIGHT_PAREN);
}

// idList  	-> 	COMMA ID idList | lambda
void Parser::ParseIdList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::COMMA) {
        Match(TokenType::COMMA);
        Match(TokenType::ID);
        ParseIdList();
    }
    // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::RIGHT_PAREN) {}
    else {
        throw(tokens.at(index));
    }
}

// schemeList	->	scheme schemeList | lambda
void Parser::ParseSchemeList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::ID) {
        ParseScheme();
        ParseSchemeList();
    }
    // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::FACTS) {}
    else {
        throw(tokens.at(index));
    }
}

// factList	->	fact factList | lambda
void Parser::ParseFactList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::ID) {
        ParseFact();
        ParseFactList();
    }
    // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::RULES) {}
    else {
        throw(tokens.at(index));
    }
}

// fact    	->	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
void Parser::ParseFact() {
    Match(TokenType::ID);
    Match(TokenType::LEFT_PAREN);
    Match(TokenType::STRING);
    ParseStringList();
    Match(TokenType::RIGHT_PAREN);
    Match(TokenType::PERIOD);
}

// stringList	-> 	COMMA STRING stringList | lambda
void Parser::ParseStringList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::COMMA) {
        Match(TokenType::COMMA);
        Match(TokenType::STRING);
        ParseStringList();
    }
        // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::RIGHT_PAREN) {}
    else {
        throw(tokens.at(index));
    }
}

// ruleList	->	rule ruleList | lambda
void Parser::ParseRuleList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::ID) {
        ParseRule();
        ParseRuleList();
    }
        // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::QUERIES) {}
    else {
        throw(tokens.at(index));
    }
}

// rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD
void Parser::ParseRule() {
    ParseHeadPredicate();
    Match(TokenType::COLON_DASH);
    ParsePredicate();
    ParsePredicateList();
    Match(TokenType::PERIOD);
}

// headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
void Parser::ParseHeadPredicate() {
    Match(TokenType::ID);
    Match(TokenType::LEFT_PAREN);
    Match(TokenType::ID);
    ParseIdList();
    Match(TokenType::RIGHT_PAREN);
}

// predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
void Parser::ParsePredicate() {
    Match(TokenType::ID);
    Match(TokenType::LEFT_PAREN);
    ParseParameter();
    ParseParameterList();
    Match(TokenType::RIGHT_PAREN);
}

// parameter	->	STRING | ID
void Parser::ParseParameter() {
    if (tokens.at(index)->getTokenType() == TokenType::STRING) {
        Match(TokenType::STRING);
    }
    else if (tokens.at(index)->getTokenType() == TokenType::ID) {
        Match(TokenType::ID);
    }
    else {
        throw(tokens.at(index));
    }
}

// parameterList	-> 	COMMA parameter parameterList | lambda
void Parser::ParseParameterList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::COMMA) {
        Match(TokenType::COMMA);
        ParseParameter();
        ParseParameterList();
    }
        // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::RIGHT_PAREN) {}
    else {
        throw(tokens.at(index));
    }
}

// predicateList	->	COMMA predicate predicateList | lambda
void Parser::ParsePredicateList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::COMMA) {
        Match(TokenType::COMMA);
        ParsePredicate();
        ParsePredicateList();
    }
        // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::PERIOD) {}
    else {
        throw(tokens.at(index));
    }
}

// query    ->  predicate Q_MARK
void Parser::ParseQuery() {
    ParsePredicate();
    Match(TokenType::Q_MARK);
}

// queryList	->	query queryList | lambda
void Parser::ParseQueryList() {
    // FIRST set
    if (tokens.at(index)->getTokenType() == TokenType::ID) {
        ParseQuery();
        ParseQueryList();
    }
        // FOLLOW set
    else if (tokens.at(index)->getTokenType() == TokenType::END) {}
    else {
        throw(tokens.at(index));
    }
}

void Parser::CreateProgram() {
    index = 0;
    // Schemes
    while(tokens.at(index)->getTokenType() != TokenType::FACTS) {
        while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::FACTS) ++index;
        if (tokens.at(index)->getTokenType() != TokenType::FACTS) {
            // there are schemes
            program.AddScheme(CreateScheme());
            // skip LEFT_PAREN
            ++index;
        }
    }
    // Facts
    while(tokens.at(index)->getTokenType() != TokenType::RULES) {
        while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::RULES) ++index;
        if (tokens.at(index)->getTokenType() != TokenType::RULES) {
            // there are facts
            program.AddFact(CreateFact());
            // skip LEFT_PAREN and PERIOD
            index += 2;
        }
    }
    // Rules
    while(tokens.at(index)->getTokenType() != TokenType::QUERIES) {
        while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::QUERIES) ++index;
        if (tokens.at(index)->getTokenType() != TokenType::QUERIES) {
            // there are rules
            program.AddRule(CreateRule());
            // skip PERIOD
            ++index;
        }
    }
    // Queries
    while(tokens.at(index)->getTokenType() != TokenType::END) {
        while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::END) ++index;
        if (tokens.at(index)->getTokenType() != TokenType::END) {
            // there are queries
            program.AddQuery(CreateQuery());
            // skip LEFT_PAREN and Q_MARK
            index += 2;
        }
    }
}

Predicate Parser::CreateScheme() {
    // TokenType is ID
    std::string ID = tokens.at(index)->getTokenDescription();
    // advance past ID and LEFT_PAREN
    index += 2;
    // Current token is the first parameter
    std::vector<Parameter> parameters;
    while (tokens.at(index)->getTokenType() != TokenType::RIGHT_PAREN) {
        if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
        else{
            Parameter p(tokens.at(index)->getTokenDescription());
            parameters.push_back(p);
            ++index;
        }
    }
    Predicate s(ID, parameters);
    return s;
}

Predicate Parser::CreateFact() {
    while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::RULES) ++index;
    if (tokens.at(index)->getTokenType() == TokenType::RULES) {
        // there are no facts
        std::vector<Parameter> noParameters;
        Predicate p("", noParameters);
        return p;
    }
    // TokenType is ID
    std::string ID = tokens.at(index)->getTokenDescription();
    // advance past ID and LEFT_PAREN
    index += 2;
    // Current token is the first parameter
    std::vector<Parameter> parameters;
    while (tokens.at(index)->getTokenType() != TokenType::RIGHT_PAREN) {
        if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
        else{
            Parameter p(tokens.at(index)->getTokenDescription());
            parameters.push_back(p);
            ++index;
        }
    }
    Predicate f(ID, parameters);
    return f;
}

Rule Parser::CreateRule() {
    while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::QUERIES) ++index;
    if (tokens.at(index)->getTokenType() == TokenType::QUERIES) {
        // there are no rules
        std::vector<Parameter> noHParameters;
        Predicate h("", noHParameters);
        std::vector<Predicate> noBody;
        Rule r(h, noBody);
        return r;
    }
    // TokenType is ID
    std::string headID = tokens.at(index)->getTokenDescription();
    // advance past ID and LEFT_PAREN
    index += 2;
    // Current token is the first parameter
    std::vector<Parameter> headParameters;
    while (tokens.at(index)->getTokenType() != TokenType::RIGHT_PAREN) {
        if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
        else{
            Parameter p(tokens.at(index)->getTokenDescription());
            headParameters.push_back(p);
            ++index;
        }
    }
    Predicate headPredicate(headID, headParameters);
    // skip RIGHT_PAREN and COLON_DASH
    index += 2;
    std::vector<Predicate> predicates;
    while (tokens.at(index)->getTokenType() != TokenType::PERIOD) {
        // TokenType is ID
        std::string ID = tokens.at(index)->getTokenDescription();
        // advance past ID and LEFT_PAREN
        index += 2;
        // Current token is the first parameter
        std::vector<Parameter> parameters;
        while (tokens.at(index)->getTokenType() != TokenType::RIGHT_PAREN) {
            if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
            else{
                Parameter p(tokens.at(index)->getTokenDescription());
                parameters.push_back(p);
                ++index;
            }
        }
        Predicate p(ID, parameters);
        predicates.push_back(p);
        // skip RIGHT_PAREN
        ++index;
        // if COMMA, skip COMMA
        if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
    }

    Rule r(headPredicate, predicates);
    return r;
}

Predicate Parser::CreateQuery() {
    while (tokens.at(index)->getTokenType() != TokenType::ID && tokens.at(index)->getTokenType() != TokenType::END) ++index;
    if (tokens.at(index)->getTokenType() == TokenType::END) {
        // there are no queries
        std::vector<Parameter> noParameters;
        Predicate p("", noParameters);
        return p;
    }
    // TokenType is ID
    std::string ID = tokens.at(index)->getTokenDescription();
    // advance past ID and LEFT_PAREN
    index += 2;
    // Current token is the first parameter
    std::vector<Parameter> parameters;
    while (tokens.at(index)->getTokenType() != TokenType::RIGHT_PAREN) {
        if (tokens.at(index)->getTokenType() == TokenType::COMMA) ++index;
        else{
            Parameter p(tokens.at(index)->getTokenDescription());
            parameters.push_back(p);
            ++index;
        }
    }

    Predicate q(ID, parameters);
    return q;
}