#include "FactsAutomaton.h"

void FactsAutomaton::S0(const std::string& input) {
    if (input[index] == 'F') {
        inputRead++;
        index++;
        SF(input);
    }
    else {
        Serr();
    }
}

void FactsAutomaton::SF(const std::string& input) {
    if (input[index] == 'a') {
        inputRead++;
        index++;
        SA(input);
    }
    else {
        Serr();
    }
}

void FactsAutomaton::SA(const std::string& input) {
    if (input[index] == 'c') {
        inputRead++;
        index++;
        SC(input);
    }
    else {
        Serr();
    }
}

void FactsAutomaton::SC(const std::string& input) {
    if (input[index] == 't') {
        inputRead++;
        index++;
        ST(input);
    }
    else {
        Serr();
    }
}

void FactsAutomaton::ST(const std::string& input) {
    if (input[index] == 's') {
        inputRead++;
    }
    else {
        Serr();
    }
}