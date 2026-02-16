#pragma once
#include "../common/Token.h"
#include "../common/TokenType.h"

enum class LRMode {
    Expr,     
    Schema    
};

inline int toLRToken(const Token& t, LRMode mode) {

    // ===============================
    // MODO ORIGINAL 
    // ===============================
    if (mode == LRMode::Expr) {
        if (t.type == TokenType::IDENT)  return 0; // id
        if (t.type == TokenType::OPSUMA) return 1; // +
        if (t.type == TokenType::END)    return 2; // $
        return -1;
    }

    // ===============================
    // MODO SCHEMA (NUEVO)
    // schema id { id : type } $
    // ===============================
    if (mode == LRMode::Schema) {
        if (t.type == TokenType::SCHEMA)  return 0;
        if (t.type == TokenType::IDENT)   return 1;
        if (t.type == TokenType::LBRACE)  return 2;
        if (t.type == TokenType::RBRACE)  return 3;
        if (t.type == TokenType::COLON)   return 4;
        if (t.type == TokenType::T_STRING) return 5;
        if (t.type == TokenType::T_NUMBER) return 6;
        if (t.type == TokenType::END)     return 7;
        return -1;
    }

    return -1;
}
