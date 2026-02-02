#include "LRParser.h"
#include "LRTokenMap.h"
#include <stack>

static const int ACCEPT = 999;

void LRParser::clearTables(int action_[5][3], int gotoE_[5]) {
    for (int i = 0; i < 5; i++) {
        gotoE_[i] = 0;
        for (int j = 0; j < 3; j++) action_[i][j] = 0;
    }
}

LRParser::LRParser(Mode mode) {
    // No terminal E = 2 (segun el profe)
    idReglas_[0] = 2;
    idReglas_[1] = 2;

    // Regla 1: E -> id + E (3 simbolos)
    // Regla 2: E -> eps (0 simbolos)
    lonReglas_[0] = 3;
    lonReglas_[1] = 0;

    if (mode == Mode::Ej1) initEj1();
    else initEj2();
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
    int token = toLRToken(look);

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
            token = toLRToken(look);

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
