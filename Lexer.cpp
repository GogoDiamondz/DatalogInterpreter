#include "Lexer.h"
#include "ColonAutomaton.h"
#include "ColonDashAutomaton.h"
#include "CommaAutomaton.h"
#include "PeriodAutomaton.h"
#include "Q_MarkAutomaton.h"
#include "Left_ParenAutomaton.h"
#include "Right_ParenAutomaton.h"
#include "MultiplyAutomaton.h"
#include "AddAutomaton.h"
#include "SchemesAutomaton.h"
#include "FactsAutomaton.h"
#include "RulesAutomaton.h"
#include "QueriesAutomaton.h"
#include "IDAutomaton.h"
#include "StringAutomaton.h"
#include "CommentAutomaton.h"
#include "UndefinedStringAutomaton.h"
#include "UndefinedCommentAutomaton.h"
#include <string>

Lexer::Lexer() {
    CreateAutomata();
}

Lexer::~Lexer() {
    for (int i = automata.size() - 1; i >= 0; --i) {
        delete automata.at(i);
    }
    for (int i = tokens.size() - 1; i >= 0; --i) {
        delete tokens.at(i);
    }
}

void Lexer::CreateAutomata() {
    automata.push_back(new ColonAutomaton());
    automata.push_back(new ColonDashAutomaton());
    automata.push_back(new CommaAutomaton());
    automata.push_back(new PeriodAutomaton());
    automata.push_back(new Q_MarkAutomaton());
    automata.push_back(new Left_ParenAutomaton());
    automata.push_back(new Right_ParenAutomaton());
    automata.push_back(new MultiplyAutomaton());
    automata.push_back(new AddAutomaton());
    automata.push_back(new SchemesAutomaton());
    automata.push_back(new FactsAutomaton());
    automata.push_back(new RulesAutomaton());
    automata.push_back(new QueriesAutomaton());
    automata.push_back(new IDAutomaton());
    automata.push_back(new StringAutomaton());
    automata.push_back(new CommentAutomaton());
    automata.push_back(new UndefinedStringAutomaton());
    automata.push_back(new UndefinedCommentAutomaton());
}

void Lexer::Run(std::string& input) {
    int lineNumber = 1;
    int maxRead = 0;
    Automaton* maxAutomaton;

    // While there are more characters to tokenize
    while (input.size() > 0) {
        maxRead = 0;
        maxAutomaton = automata.front();

        // Handle whitespace in between tokens
        while (input.size() > 0 && isspace(input.at(0))) {
            if (input.at(0) == '\n') { ++lineNumber; }
            input = input.substr(1);
        }

        // Here is the "Parallel" part of the algorithm
        //   Each automaton runs with the same input
        for (Automaton* automaton : automata) {
            int inputRead = automaton->Start(input);
            if (inputRead > maxRead) {
                maxRead = inputRead;
                maxAutomaton = automaton;
            }
        }
        // Here is the "Max" part of the algorithm
        if (maxRead > 0) {
            Token* newToken = maxAutomaton->CreateToken(input.substr(0, maxRead), lineNumber);
            lineNumber += maxAutomaton->NewLinesRead();
            tokens.push_back(newToken);
        }
        // No automaton accepted input
        // Create single character undefined token
        else {
            if (input.size() > 0) {
                maxRead = 1;
                Token *newToken = new Token(TokenType::UNDEFINED, input.substr(0, maxRead), lineNumber);
                tokens.push_back(newToken);
            }
        }
        // Update `input` by removing characters read to create Token
        input = input.substr(maxRead);
    }
    tokens.push_back(new Token(TokenType::END, "", lineNumber));
}
