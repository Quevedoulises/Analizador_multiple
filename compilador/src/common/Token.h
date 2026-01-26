#pragma once
#include <string>
#include "TokenType.h"

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int col;
};
