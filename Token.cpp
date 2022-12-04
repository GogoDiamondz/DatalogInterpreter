#include "Token.h"

std::string Token::tokenTypeToString(TokenType t) const {
    std::string tokenTypeString;
    switch(t) {
        case TokenType::COLON :
            tokenTypeString = "COLON";
            break;
        case TokenType::COLON_DASH :
            tokenTypeString = "COLON_DASH";
            break;
        case TokenType::UNDEFINED :
            tokenTypeString = "UNDEFINED";
            break;
        case TokenType::COMMA :
            tokenTypeString = "COMMA";
            break;
        case TokenType::PERIOD :
            tokenTypeString = "PERIOD";
            break;
        case TokenType::Q_MARK :
            tokenTypeString = "Q_MARK";
            break;
        case TokenType::LEFT_PAREN :
            tokenTypeString = "LEFT_PAREN";
            break;
        case TokenType::RIGHT_PAREN :
            tokenTypeString = "RIGHT_PAREN";
            break;
        case TokenType::MULTIPLY :
            tokenTypeString = "MULTIPLY";
            break;
        case TokenType::ADD :
            tokenTypeString = "ADD";
            break;
        case TokenType::SCHEMES :
            tokenTypeString = "SCHEMES";
            break;
        case TokenType::FACTS :
            tokenTypeString = "FACTS";
            break;
        case TokenType::RULES :
            tokenTypeString = "RULES";
            break;
        case TokenType::QUERIES :
            tokenTypeString = "QUERIES";
            break;
        case TokenType::ID :
            tokenTypeString = "ID";
            break;
        case TokenType::STRING :
            tokenTypeString = "STRING";
            break;
        case TokenType::COMMENT :
            tokenTypeString = "COMMENT";
            break;
        case TokenType::END :
            tokenTypeString = "EOF";
            break;
    }
    return tokenTypeString;
}

Token::Token(TokenType type, std::string description, int line) {
    tokenType = type;
    tokenDescription = description;
    lineNumber = line;
}

std::string Token::ToString() const {
    std::ostringstream oss;
    oss << "(" << getTokenTypeString() << "," << "\"" << tokenDescription << "\"" << "," << lineNumber << ")";
    return oss.str();
}