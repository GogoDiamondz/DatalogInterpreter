#include "QueriesAutomaton.h"

void QueriesAutomaton::S0(const std::string& input) {
    if (input[index] == 'Q') {
        inputRead++;
        index++;
        SQ(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SQ(const std::string& input) {
    if (input[index] == 'u') {
        inputRead++;
        index++;
        SU(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SU(const std::string& input) {
    if (input[index] == 'e') {
        inputRead++;
        index++;
        SE1(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SE1(const std::string& input) {
    if (input[index] == 'r') {
        inputRead++;
        index++;
        SR(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SR(const std::string& input) {
    if (input[index] == 'i') {
        inputRead++;
        index++;
        SI(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SI(const std::string& input) {
    if (input[index] == 'e') {
        inputRead++;
        index++;
        SE2(input);
    }
    else {
        Serr();
    }
}

void QueriesAutomaton::SE2(const std::string& input) {
    if (input[index] == 's') {
        inputRead++;
    }
    else {
        Serr();
    }
}