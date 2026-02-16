cmake_minimum_required(VERSION 3.20)

project(compilador LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compilador
    src/main.cpp

    src/lexer/Lexer.cpp

    src/parser/LRParser.cpp

    src/codegen/JSONGenerator.cpp
    src/codegen/AjvGenerator.cpp
)
