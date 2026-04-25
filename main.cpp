#include <iostream>
#include <string>
#include "preprocessor.h"
#include "assembler.h"
#include "simulator.h"

// compilar com g++ -std=c++11 main.cpp utils.cpp -o namefile 

int main(int argc, char* argv[]){
    if (argc < 2) return 1;
    std::string filename = argv[1];
    
    if (filename.find(".asm") != std::string::npos) {
        pre_processar(filename);
    } else if (filename.find(".pre") != std::string::npos) {
        // Chama o montador -> gera .obj e .pen 
    } else if (filename.find(".obj") != std::string::npos) {
        // Chama o simulador
    }
    return 0;
}