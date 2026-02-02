#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "lexer/Lexer.h"
#include "common/TokenType.h"
#include "parser/LRParser.h"   

static std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("No se pudo abrir: " + path);
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

static void runLexer(const std::string& input) {
    Lexer lx(input);
    while (true) {
        Token t = lx.next();
        std::cout << "[" << tt(t.type) << "] "
                  << t.lexeme
                  << " (L" << t.line << ",C" << t.col << ")\n";
        if (t.type == TokenType::END) break;
    }
}


static void runLR(const std::string& input, LRParser::Mode mode) {
    Lexer lx(input);
    LRParser parser(mode);

    std::string err;
    bool ok = parser.parse(lx, &err);

    if (ok) {
        std::cout << "Cadena aceptada \n";
    } else {
        std::cout << "Error \n";
        std::cout << err << "\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Uso:\n";
        std::cout << "  compilador --mode lexer <archivo>\n";
        std::cout << "  compilador --mode lr1   <archivo>\n";
        std::cout << "  compilador --mode lr2   <archivo>\n";
        return 0;
    }

    std::string modeFlag = argv[1];
    std::string mode = argv[2];
    std::string file = argv[3];

    if (modeFlag != "--mode") {
        std::cout << "Falta --mode\n";
        return 1;
    }

    std::string input = readFile(file);

    if (mode == "lexer") {
        runLexer(input);
        return 0;
    }

    if (mode == "lr1") {
        runLR(input, LRParser::Mode::Ej1);
        return 0;
    }

    if (mode == "lr2") {
        runLR(input, LRParser::Mode::Ej2);
        return 0;
    }

    std::cout << "Modo no soportado aun: " << mode << "\n";
    return 0;
}
