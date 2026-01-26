#include "Lexer.h"
#include <unordered_map>
#include <stdexcept>

static const std::unordered_map<std::string, TokenType> RESERVED = {
    {"if", TokenType::IF},
    {"while", TokenType::WHILE},
    {"return", TokenType::RETURN},
    {"else", TokenType::ELSE},
    {"int", TokenType::TYPE},
    {"float", TokenType::TYPE},
    {"void", TokenType::TYPE},
};

Lexer::Lexer(const std::string& input)
    : text(input), pos(0), line(1), col(1) {}

char Lexer::peek(int k) const {
    size_t i = pos + k;
    if (i >= text.size()) return '\0';
    return text[i];
}

char Lexer::advance() {
    char c = peek();
    if (c == '\0') return '\0';
    pos++;
    if (c == '\n') { line++; col = 1; }
    else { col++; }
    return c;
}

bool Lexer::isLetter(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isDigit(char c) const {
    return (c >= '0' && c <= '9');
}

void Lexer::skipSpaces() {
    while (true) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
        } else {
            break;
        }
    }
}

Token Lexer::next() {
    skipSpaces();

    int startLine = line;
    int startCol = col;
    char c = peek();

    // EOF
    if (c == '\0') {
        return {TokenType::END, "$", startLine, startCol};
    }

    // Identificador o reservada: letra (letra|digito)*
    if (isLetter(c)) {
        std::string lex;
        lex.push_back(advance());
        while (true) {
            char p = peek();
            if (isLetter(p) || isDigit(p)) lex.push_back(advance());
            else break;
        }

        auto it = RESERVED.find(lex);
        if (it != RESERVED.end()) {
            return {it->second, lex, startLine, startCol};
        }
        return {TokenType::IDENT, lex, startLine, startCol};
    }

    // Entero: digito+
    // Real: entero.entero
    if (isDigit(c)) {
        std::string lex;
        while (isDigit(peek())) lex.push_back(advance());

        if (peek() == '.' && isDigit(peek(1))) {
            lex.push_back(advance()); // '.'
            while (isDigit(peek())) lex.push_back(advance());
            return {TokenType::REALNUM, lex, startLine, startCol};
        }

        return {TokenType::INTNUM, lex, startLine, startCol};
    }

    // Cadena: "...."
    if (c == '"') {
        advance(); // consume "
        std::string lex;
        while (true) {
            char p = peek();
            if (p == '\0') throw std::runtime_error("Error lexico: cadena sin cerrar");
            if (p == '"') { advance(); break; }
            lex.push_back(advance());
        }
        return {TokenType::STRING, lex, startLine, startCol};
    }

    // Operadores dobles primero
    std::string two;
    two.push_back(c);
    two.push_back(peek(1));

    if (two == "&&") { advance(); advance(); return {TokenType::OPAND, "&&", startLine, startCol}; }
    if (two == "||") { advance(); advance(); return {TokenType::OPOR, "||", startLine, startCol}; }
    if (two == "==") { advance(); advance(); return {TokenType::OPIGUAL, "==", startLine, startCol}; }
    if (two == "!=") { advance(); advance(); return {TokenType::OPIGUAL, "!=", startLine, startCol}; }
    if (two == "<=") { advance(); advance(); return {TokenType::OPREL, "<=", startLine, startCol}; }
    if (two == ">=") { advance(); advance(); return {TokenType::OPREL, ">=", startLine, startCol}; }

    // Operadores simples y signos
    if (c == '+' || c == '-') { advance(); return {TokenType::OPSUMA, std::string(1,c), startLine, startCol}; }
    if (c == '*' || c == '/') { advance(); return {TokenType::OPMUL,  std::string(1,c), startLine, startCol}; }

    if (c == '<' || c == '>') { advance(); return {TokenType::OPREL,  std::string(1,c), startLine, startCol}; }

    if (c == '!') { advance(); return {TokenType::OPNOT, "!", startLine, startCol}; }

    if (c == '=') { advance(); return {TokenType::ASSIGN, "=", startLine, startCol}; }

    if (c == ';') { advance(); return {TokenType::SEMI, ";", startLine, startCol}; }
    if (c == ',') { advance(); return {TokenType::COMMA, ",", startLine, startCol}; }
    if (c == '(') { advance(); return {TokenType::LPAREN, "(", startLine, startCol}; }
    if (c == ')') { advance(); return {TokenType::RPAREN, ")", startLine, startCol}; }
    if (c == '{') { advance(); return {TokenType::LBRACE, "{", startLine, startCol}; }
    if (c == '}') { advance(); return {TokenType::RBRACE, "}", startLine, startCol}; }

    // Si llega aqui: caracter no valido
    throw std::runtime_error(std::string("Error lexico: simbolo no valido '") + c + "'");
}
