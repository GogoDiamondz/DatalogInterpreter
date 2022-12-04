#include "SchemesAutomaton.h"

void SchemesAutomaton::S0(const std::string& input) {
    if (input[index] == 'S') {
        inputRead++;
        index++;
        SS(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SS(const std::string& input) {
    if (input[index] == 'c') {
        inputRead++;
        index++;
        SC(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SC(const std::string& input) {
    if (input[index] == 'h') {
        inputRead++;
        index++;
        SH(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SH(const std::string& input) {
    if (input[index] == 'e') {
        inputRead++;
        index++;
        SE1(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SE1(const std::string& input) {
    if (input[index] == 'm') {
        inputRead++;
        index++;
        SM(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SM(const std::string& input) {
    if (input[index] == 'e') {
        inputRead++;
        index++;
        SE2(input);
    }
    else {
        Serr();
    }
}

void SchemesAutomaton::SE2(const std::string& input) {
    if (input[index] == 's') {
        inputRead++;
    }
    else {
        Serr();
    }
}