#include "Utils.h"
#include <string>
#include <algorithm>






void to_upper(std::string &str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}


std::string trim(const std::string &str){
    size_t first = str.find_first_not_of(" \t\n\r"); // (   ADD  \n) first=3 (indice do A)
    
    // se a string inteira for vazia retorna vazio (npos representa o valor max de size_t ou -1)
    if(std::string::npos == first){
        return "";
    }

    size_t last = str.find_last_not_of(" \t\n\r"); // (  ADD   \n) last=4 (indice do D)
    
    return str.substr(first, (last - first + 1));  // corta a string (indice primeira letra, (indice do ultimo - primeiro + 1))
}


std::string clear_spaces(const std::string &str){
    std::string result = "";
    bool is_space = false;  // flag que diz se eh espaco

    for(char c : str){
        // se for espaco, \t, \r ativa a flag
        if(c == ' ' || c == '\t' || c == '\r'){
            is_space = true;
        }

        // se for uma letra, virgula
        else{
            // se no elem anterior a flag foi ativada
            if(is_space){
                // adicionamos espaco se a string nao for vazia
                // o ultimo caracter inserido nao foi uma virgula
                // e o caracter atual nao eh uma virgula
                if(!result.empty() && result.back() != ',' && c != ','){
                    result += ' ';
                }
            }

            // add caracter na string final
            result += c;
            is_space = false;
        }

    }

    return result;
}




