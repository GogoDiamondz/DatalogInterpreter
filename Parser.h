#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include "Token.h"
#include "DatalogProgram.h"

class Parser
{
private:
    DatalogProgram program;
    std::vector<Token*> tokens;
    size_t index;

    std::string tokenTypeToString(TokenType t) const;
    void Match(TokenType to_match);
    void ParseScheme();
    void ParseIdList();
    void ParseSchemeList();
    void ParseFactList();
    void ParseFact();
    void ParseStringList();
    void ParseRuleList();
    void ParseRule();
    void ParseHeadPredicate();
    void ParsePredicate();
    void ParseParameter();
    void ParseParameterList();
    void ParsePredicateList();
    void ParseQuery();
    void ParseQueryList();

    void CreateProgram();
    Predicate CreateScheme();
    Predicate CreateFact();
    Rule CreateRule();
    Predicate CreateQuery();

public:
    Parser(std::vector<Token*> t) {
        tokens = t;
        index = 0;
    }

    ~Parser() {}

    int getIndex() { return index; }

    void ParseDatalogProgram();
    DatalogProgram getProgram() { return program; }

    std::string ToString() const {
        std::ostringstream oss;
        oss << program;
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, const Parser* p) {
        os << p->ToString();
        return os;
    }
};

#endif // PARSER_H

