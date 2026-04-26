#ifndef PARSER_H
#define PARSER_H

#include <string>

struct AssemblyLine {
    std::string rotulo;  
    std::string operacao; 
    std::string operandos;
};

AssemblyLine line_parser(std::string linha);

#endif