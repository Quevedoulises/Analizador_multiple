#pragma once
#include "../ast/ASTNode.h"
#include <string>

class JSONGenerator {
public:
    static std::string generate(ASTNode* root);
};
