#pragma once
#include <string>
#include "../common/Token.h"

class Lexer {
public:
    explicit Lexer(const std::string& input);
    Token next(); // regresa el siguiente token

private:
    std::string text;
    size_t pos;
    int line;
    int col;

    char peek(int k = 0) const;
    char advance();
    void skipSpaces();

    bool isLetter(char c) const;
    bool isDigit(char c) const;
};
