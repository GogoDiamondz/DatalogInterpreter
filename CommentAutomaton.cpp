#include "CommentAutomaton.h"

void CommentAutomaton::S0(const std::string& input) {
    if (input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void CommentAutomaton::S1(const std::string& input) {
    if (index >= int(input.size())) {}
    else if (input[index] == '|') {
        inputRead++;
        index++;
        SBlock(input);
    }
    else {
        inputRead++;
        index++;
        SLine(input);
    }
}

void CommentAutomaton::SBlock(const std::string& input) {
    if (index >= int(input.size())) {
        Serr();
    }
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

void CommentAutomaton::SBlockFinish(const std::string& input) {
    if (input[index] == '#') {
        inputRead++;
    }
    else {
        if (input[index] == '\n') { newLines++; }
        inputRead++;
        index++;
        SBlock(input);
    }
}

void CommentAutomaton::SLine(const std::string& input) {
    if (index < int(input.size()) && input[index] != '\n') {
        inputRead++;
        index++;
        SLine(input);
    }
}