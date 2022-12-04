#include "RulesAutomaton.h"

void RulesAutomaton::S0(const std::string& input) {
    if (input[index] == 'R') {
        inputRead++;
        index++;
        SR(input);
    }
    else {
        Serr();
    }
}

void RulesAutomaton::SR(const std::string& input) {
    if (input[index] == 'u') {
        inputRead++;
        index++;
        SU(input);
    }
    else {
        Serr();
    }
}

void RulesAutomaton::SU(const std::string& input) {
    if (input[index] == 'l') {
        inputRead++;
        index++;
        SL(input);
    }
    else {
        Serr();
    }
}

void RulesAutomaton::SL(const std::string& input) {
    if (input[index] == 'e') {
        inputRead++;
        index++;
        SE(input);
    }
    else {
        Serr();
    }
}

void RulesAutomaton::SE(const std::string& input) {
    if (input[index] == 's') {
        inputRead++;
    }
    else {
        Serr();
    }
}