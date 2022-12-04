#include "UndefinedCommentAutomaton.h"

void UndefinedCommentAutomaton::S0(const std::string& input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void UndefinedCommentAutomaton::S1(const std::string& input) {
    if (index >= int(input.size())) {
        Serr();
    }
    else if (input[index] == '|') {
        inputRead++;
        index++;
        SBlock(input);
    }
    else {
        Serr();
    }
}

void UndefinedCommentAutomaton::SBlock(const std::string& input) {
    if (index >= int(input.size())) {}
    else if (input[index] != '|') {
        if (input[index] == '\n') { newLines++; }
        inputRead++;
        index++;
        SBlock(input);
    }
    else {
        inputRead++;
        index++;
        SBlockFinish(input);
    }
}

void UndefinedCommentAutomaton::SBlockFinish(const std::string& input) {
    if (index >= int(input.size())) {}
    else if (input[index] == '#') {
        Serr();
    }
    else {
        if (input[index] == '\n') { newLines++; }
        inputRead++;
        index++;
        SBlock(input);
    }
}