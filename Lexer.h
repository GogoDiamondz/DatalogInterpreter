#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <sstream>
#include "Automaton.h"
#include "Token.h"

class Lexer
{
private:
    std::vector<Automaton*> automata;
    std::vector<Token*> tokens;

    void CreateAutomata();

public:
    Lexer();
    ~Lexer();

    void Run(std::string& input);

    std::vector<Token*> getTokens() { return tokens; }

    std::string ToString() const {
        std::ostringstream oss;
        for (Token* token : tokens) {
            oss << token;
            oss << std::endl;
        }
        oss << "Total Tokens = " << tokens.size();
        return oss.str();
    }

    friend std::ostream& operator<< (std::ostream& os, const Lexer* l) {
        os << l->ToString();
        return os;
    }

};

#endif // LEXER_H

