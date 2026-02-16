#pragma once
#include <string>
#include <vector>

enum class NodeType {
    SCHEMA,
    PROPERTY,
    TYPE,
    CONSTRAINT
};

class ASTNode {
public:
    NodeType type;
    std::string value;
    std::vector<ASTNode*> children;

    ASTNode(NodeType t, const std::string& val = "")
        : type(t), value(val) {}

    void add(ASTNode* child) {
        children.push_back(child);
    }
};
