#ifndef SCHEMESAUTOMATON_H
#define SCHEMESAUTOMATON_H

#include "Automaton.h"

class SchemesAutomaton : public Automaton
{
private:
    void SS(const std::string& input);
    void SC(const std::string& input);
    void SH(const std::string& input);
    void SE1(const std::string& input);
    void SM(const std::string& input);
    void SE2(const std::string& input);

public:
    SchemesAutomaton() : Automaton(TokenType::SCHEMES) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif // SCHEMESAUTOMATON_H

