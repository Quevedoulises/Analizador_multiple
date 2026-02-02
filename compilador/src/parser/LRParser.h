#pragma once
#include <string>
#include "../lexer/Lexer.h"

class LRParser {
public:
    enum class Mode { Ej1, Ej2 };

    explicit LRParser(Mode mode);

    // true = acepta, false = error
    bool parse(Lexer& lexer, std::string* error_msg = nullptr);

private:
    // action[state][token] token: 0=id 1=+ 2=$
    int action_[5][3];
    // goto para no terminal E
    int gotoE_[5];

    // arreglos del profe para reducciones (ej2)
    int idReglas_[2];
    int lonReglas_[2];

    void initEj1();
    void initEj2();

    static void clearTables(int action_[5][3], int gotoE_[5]);
};
