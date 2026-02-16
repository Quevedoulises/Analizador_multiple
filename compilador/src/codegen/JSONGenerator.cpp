#include "JSONGenerator.h"
#include <sstream>

std::string JSONGenerator::generate(ASTNode* root) {

    if (!root) return "{}";

    std::stringstream json;

    std::string schemaName = root->children[0]->value;
    std::string propName = root->children[1]->value;
    std::string propType = root->children[1]->children[0]->value;

    json << "{\n";
    json << "  \"type\": \"object\",\n";
    json << "  \"title\": \"" << schemaName << "\",\n";
    json << "  \"properties\": {\n";
    json << "    \"" << propName << "\": {\n";
    json << "      \"type\": \"" << propType << "\"\n";
    json << "    }\n";
    json << "  }\n";
    json << "}";

    return json.str();
}
