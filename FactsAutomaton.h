#ifndef FACTSAUTOMATON_H
#define FACTSAUTOMATON_H

#include "Automaton.h"

class FactsAutomaton : public Automaton
{
private:
    void SF(const std::string& input);
    void SA(const std::string& input);
    void SC(const std::string& input);
    void ST(const std::string& input);

public:
    FactsAutomaton() : Automaton(TokenType::FACTS) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif // FACTSAUTOMATON_H

