#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <sstream>
#include <iostream>

enum class TokenType {
    COLON,
    COLON_DASH,
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    END
};

class Token
{
private:
    TokenType tokenType;
    std::string tokenDescription;
    int lineNumber;

public:
    Token(TokenType type, std::string description, int line);

    TokenType getTokenType() const { return tokenType; }
    std::string tokenTypeToString(TokenType t) const;
    std::string getTokenTypeString() const { return tokenTypeToString(tokenType); }
    std::string getTokenDescription() const { return tokenDescription; }

    std::string ToString() const;

    friend std::ostream& operator<< (std::ostream& os, const Token* t) {
        os << t->ToString();
        return os;
    }
};

#endif // TOKEN_H

