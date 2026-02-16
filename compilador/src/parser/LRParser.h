#pragma once
#include "../lexer/Lexer.h"
#include "../ast/ASTNode.h"
#include "LRTokenMap.h"   
#include <string>

class LRParser {
public:
    enum class Mode { Ej1, Ej2, Schema };

    LRParser(Mode mode);

    
    bool parse(Lexer& lexer, std::string* error_msg = nullptr);

    
    ASTNode* parseToAST(Lexer& lexer, std::string* error_msg = nullptr);

private:
    int action_[5][3];
    int gotoE_[5];

    int idReglas_[2];
    int lonReglas_[2];

    LRMode modeActual; 

    void initEj1();
    void initEj2();
    void clearTables(int action_[5][3], int gotoE_[5]);
    void initSchema();

};
