#ifndef UNDEFINEDSTRINGAUTOMATON_H
#define UNDEFINEDSTRINGAUTOMATON_H

#include "Automaton.h"

// This automaton will recognize a string that is not terminated
class UndefinedStringAutomaton : public Automaton
{
private:
    void S1(const std::string& input);
    void S2(const std::string& input);

public:
    UndefinedStringAutomaton() : Automaton(TokenType::UNDEFINED) {}  // Call the base constructor

    void S0(const std::string& input);
};

#endif // UNDEFINEDSTRINGAUTOMATON_H

