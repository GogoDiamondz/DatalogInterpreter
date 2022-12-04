#ifndef QUERIESAUTOMATON_H
#define QUERIESAUTOMATON_H

#include "Automaton.h"

class QueriesAutomaton : public Automaton
{
private:
    void SQ(const std::string& input);
    void SU(const std::string& input);
    void SE1(const std::string& input);
    void SR(const std::string& input);
    void SI(const std::string& input);
    void SE2(const std::string& input);

public:
    QueriesAutomaton() : Automaton(TokenType::QUERIES) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif // QUERIESAUTOMATON_H

