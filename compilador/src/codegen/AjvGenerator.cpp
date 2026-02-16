#include "AjvGenerator.h"
#include <sstream>

std::string AjvGenerator::generate(ASTNode* root) {

    std::stringstream js;

    std::string schemaName = root->children[0]->value;
    std::string propName = root->children[1]->value;
    std::string propType = root->children[1]->children[0]->value;

    js << "import Ajv from \"ajv\";\n\n";
    js << "const ajv = new Ajv();\n\n";

    js << "const schema = {\n";
    js << "  type: \"object\",\n";
    js << "  title: \"" << schemaName << "\",\n";
    js << "  properties: {\n";
    js << "    " << propName << ": { type: \"" << propType << "\" }\n";
    js << "  },\n";
    js << "  required: [\"" << propName << "\"]\n";
    js << "};\n\n";

    js << "export const validate" << schemaName
       << " = ajv.compile(schema);\n";

    return js.str();
}
