#pragma once
#include <string>

enum class TokenType : int {
    IDENT = 0,
    INTNUM = 1,
    REALNUM = 2,
    STRING = 3,
    TYPE = 4,        // int,float,void

    OPSUMA = 5,      // +,-
    OPMUL = 6,       // *,/
    OPREL = 7,       // <,<=,>,>=
    OPOR = 8,        // ||
    OPAND = 9,       // &&
    OPNOT = 10,      // !
    OPIGUAL = 11,    // ==, !=

    SEMI = 12,       // ;
    COMMA = 13,      // ,
    LPAREN = 14,     // (
    RPAREN = 15,     // )
    LBRACE = 16,     // {
    RBRACE = 17,     // }
    ASSIGN = 18,     // =

    IF = 19,
    WHILE = 20,
    RETURN = 21,
    ELSE = 22,

    END = 23         // $
};

inline int tt(TokenType t) { return static_cast<int>(t); }
