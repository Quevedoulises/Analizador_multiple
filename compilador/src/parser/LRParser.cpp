#include "LRParser.h"
#include "LRTokenMap.h"
#include <stack>
#include <algorithm>
#include "../ast/ASTNode.h"



static const int ACCEPT = 999;

void LRParser::clearTables(int action_[5][3], int gotoE_[5]) {
    for (int i = 0; i < 5; i++) {
        gotoE_[i] = 0;
        for (int j = 0; j < 3; j++) action_[i][j] = 0;
    }
}

LRParser::LRParser(Mode mode) {

    // decidir modo de lenguaje
    if (mode == Mode::Ej1 || mode == Mode::Ej2)
        modeActual = LRMode::Expr;
    else
        modeActual = LRMode::Schema;

    // cargar tabla correspondiente
    if (mode == Mode::Ej1)
        initEj1();
    else if (mode == Mode::Ej2)
        initEj2();
    else
        initSchema();
}

void LRParser::initEj1() {
    clearTables(action_, gotoE_);

    // Tabla Ej1
    action_[0][0] = 2;     // d2
    gotoE_[0] = 1;

    action_[1][2] = ACCEPT; // accept

    action_[2][1] = 3;     // d3
    action_[3][0] = 4;     // d4

    action_[4][2] = -1;    // r1
}

void LRParser::initEj2() {
    clearTables(action_, gotoE_);

    // Tabla Ej2
    action_[0][0] = 2;     // d2
    gotoE_[0] = 1;

    action_[1][2] = ACCEPT;

    action_[2][1] = 3;     // d3
    action_[2][2] = -2;    // r2

    action_[3][0] = 2;     // d2
    gotoE_[3] = 4;

    action_[4][2] = -1;    // r1
}

bool LRParser::parse(Lexer& lexer, std::string* error_msg) {
    std::stack<int> st;
    st.push(0); // estado inicial

    Token look = lexer.next();
    int token = toLRToken(look, modeActual);


    if (token == -1) {
        if (error_msg) *error_msg = "Token invalido para esta gramatica: " + look.lexeme;
        return false;
    }

    while (true) {
        int state = st.top();

        int act = action_[state][token];

        // ACEPTAR
        if (act == ACCEPT) return true;

        // ERROR (celda vacia)
        if (act == 0) {
            if (error_msg) {
                *error_msg = "Error: celda vacia. state=" + std::to_string(state) +
                             " token=" + std::to_string(token) +
                             " lexema=" + look.lexeme +
                             " (line=" + std::to_string(look.line) +
                             ", col=" + std::to_string(look.col) + ")";
            }
            return false;
        }

        // SHIFT
        if (act > 0) {
            st.push(token); // simbolo
            st.push(act);   // estado destino

            look = lexer.next();
            token = toLRToken(look, modeActual);


            if (token == -1) {
                if (error_msg) *error_msg = "Token invalido para esta gramatica: " + look.lexeme;
                return false;
            }
            continue;
        }

        // REDUCE
        int r = -act; // r1=>1 r2=>2

        // EJ1: unica reduccion real E -> id + id (3 simbolos)
        // identificamos Ej1 porque no tiene r2 en (estado 2, $)
        if (r == 1 && action_[2][2] == 0) {
            int rhs_len = 3;

            for (int i = 0; i < rhs_len * 2; i++) st.pop();

            int s2 = st.top();
            int go = gotoE_[s2];

            st.push(2); // E
            st.push(go);
            continue;
        }

        // EJ2: usar arreglos del profe
        int idx = r - 1; // r1->0, r2->1
        if (idx < 0 || idx > 1) {
            if (error_msg) *error_msg = "Regla fuera de rango: r" + std::to_string(r);
            return false;
        }

        int rhs_len = lonReglas_[idx];

        for (int i = 0; i < rhs_len * 2; i++) st.pop();

        int s2 = st.top();
        int go = gotoE_[s2];

        st.push(idReglas_[idx]); // E
        st.push(go);
    }
}

ASTNode* LRParser::parseToAST(Lexer& lexer, std::string* error_msg) {

    std::vector<Token> tokens;

    // leer todos los tokens
    while (true) {
        Token t = lexer.next();
        tokens.push_back(t);
        if (t.type == TokenType::END)
            break;
    }

    // validar estructura minima:
    // schema id { id : type }

    if (tokens.size() < 7) {
        if (error_msg) *error_msg = "Entrada incompleta";
        return nullptr;
    }

    if (tokens[0].type != TokenType::SCHEMA) {
        if (error_msg) *error_msg = "Se esperaba 'schema'";
        return nullptr;
    }

    if (tokens[1].type != TokenType::IDENT) {
        if (error_msg) *error_msg = "Se esperaba identificador de esquema";
        return nullptr;
    }

    if (tokens[2].type != TokenType::LBRACE) {
        if (error_msg) *error_msg = "Se esperaba '{'";
        return nullptr;
    }

    if (tokens[3].type != TokenType::IDENT) {
        if (error_msg) *error_msg = "Se esperaba nombre de propiedad";
        return nullptr;
    }

    if (tokens[4].type != TokenType::COLON) {
        if (error_msg) *error_msg = "Se esperaba ':'";
        return nullptr;
    }

    if (tokens[5].type != TokenType::T_STRING &&
        tokens[5].type != TokenType::T_NUMBER) {
        if (error_msg) *error_msg = "Se esperaba tipo (string|number)";
        return nullptr;
    }

    if (tokens[6].type != TokenType::RBRACE) {
        if (error_msg) *error_msg = "Se esperaba '}'";
        return nullptr;
    }

    // ===== construir AST =====

    ASTNode* root = new ASTNode(NodeType::SCHEMA, "schema");

    // nombre del esquema
    ASTNode* schemaName = new ASTNode(NodeType::TYPE, tokens[1].lexeme);
    root->add(schemaName);

    // propiedad
    ASTNode* property = new ASTNode(NodeType::PROPERTY, tokens[3].lexeme);
    root->add(property);

    // tipo
    ASTNode* type = new ASTNode(NodeType::TYPE, tokens[5].lexeme);
    property->add(type);

    return root;
}


void LRParser::initSchema() {

    // limpiar tablas
    for (int i = 0; i < 5; i++) {
        gotoE_[i] = 0;
        for (int j = 0; j < 3; j++)
            action_[i][j] = 0;
    }

    // lenguaje:
    // schema id { id : type } $

    action_[0][0] = 1; // schema
    action_[1][1] = 2; // id
    action_[2][2] = 3; // {
    action_[3][1] = 4; // id propiedad
    action_[4][4] = 5; // :
    action_[5][5] = 6; // string
    action_[5][6] = 6; // number
    action_[6][3] = 7; // }
    action_[7][7] = ACCEPT; // $
}

