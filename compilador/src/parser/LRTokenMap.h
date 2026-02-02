#pragma once
#include "../common/Token.h"
#include "../common/TokenType.h"

// Tabla LR pide:
// id = 0, + = 1, $ = 2
inline int toLRToken(const Token& t) {
    if (t.type == TokenType::IDENT)  return 0; // id
    if (t.type == TokenType::OPSUMA) return 1; // +
    if (t.type == TokenType::END)    return 2; // $

    return -1; // invalido para esta gramatica
}