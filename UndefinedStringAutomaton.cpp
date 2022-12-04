#include "UndefinedStringAutomaton.h"

void UndefinedStringAutomaton::S0(const std::string& input) {
    if (input[index] == '\'') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void UndefinedStringAutomaton::S1(const std::string& input) {
    if (index >= int(input.size())) {}
    else if (input[index] != '\'') {
        if (input[index] == '\n') { newLines++; }
        inputRead++;
        index++;
        S1(input);
    }
    else {
        inputRead++;
        index++;
        S2(input);
    }
}

void UndefinedStringAutomaton::S2(const std::string& input) {
    if (input[index] == '\'') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}