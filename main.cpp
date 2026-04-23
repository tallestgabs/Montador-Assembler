#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>

// compilar com g++ main.cpp Utils.cpp -o namefile 

int main(){

    std::string linha = "   \t COPY A,  B  ADD  C  D \n   ";
    std::cout << "Antes de limpar: [" << linha << "]" << "\n";

    linha = trim(linha);

    to_upper(linha);


    std::cout << "Depois de limpar [" << clear_spaces(linha) << "]" << "\n";

    return 0;
}

