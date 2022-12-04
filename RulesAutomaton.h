#ifndef RULESAUTOMATON_H
#define RULESAUTOMATON_H

#include "Automaton.h"

class RulesAutomaton : public Automaton
{
private:
    void SR(const std::string& input);
    void SU(const std::string& input);
    void SL(const std::string& input);
    void SE(const std::string& input);

public:
    RulesAutomaton() : Automaton(TokenType::RULES) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif // RULESAUTOMATON_H

