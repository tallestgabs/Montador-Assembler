#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>

// compilar com g++ main.cpp utils.cpp -o namefile 

int main(){

    std::string linha = "   \t COPY A,  B  ADD  C  D ; isso eh um comentario aaaa\n   ";
    std::cout << "Antes de limpar: [" << linha << "]" << "\n";

    linha = remove_comments(linha);

    linha = trim(linha);

    to_upper(linha);


    std::cout << "Depois de limpar [" << clear_spaces(linha) << "]" << "\n";

    return 0;
}

