#pragma once
#include "../ast/ASTNode.h"
#include <string>

class AjvGenerator {
public:
    static std::string generate(ASTNode* root);
};
