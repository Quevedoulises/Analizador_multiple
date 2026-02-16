#include <iostream>
#include <fstream>
#include <sstream>

#include "lexer/Lexer.h"
#include "parser/LRParser.h"
#include "ast/ASTNode.h"
#include "codegen/JSONGenerator.h"
#include "codegen/AjvGenerator.h"

void printAST(ASTNode* node, int level = 0) {
    if (!node) return;

    for (int i = 0; i < level; i++)
        std::cout << "  ";

    std::cout << node->value << "\n";

    for (auto c : node->children)
        printAST(c, level + 1);
}

int main(int argc, char* argv[]) {

    // verificar argumento
    if (argc < 2) {
        std::cout << "Uso: compilador archivo.schema\n";
        return 0;
    }

    // abrir archivo
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo\n";
        return 0;
    }

    // leer contenido completo
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string input = buffer.str();

    // ===== ANALISIS =====
    Lexer lexer(input);
    LRParser parser(LRParser::Mode::Schema);

    std::string error;
    ASTNode* root = parser.parseToAST(lexer, &error);

    if (!root) {
        std::cout << "ERROR: " << error << "\n";
        return 0;
    }

    // ===== RESULTADOS =====
    std::cout << "AST:\n";
    printAST(root);

    std::cout << "\nJSON:\n";
    std::cout << JSONGenerator::generate(root) << "\n";

    std::cout << "\nJS AJV:\n";
    std::cout << AjvGenerator::generate(root) << "\n";

    return 0;
}
