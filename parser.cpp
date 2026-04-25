#include "parser.h"
#include "utils.h"

AssemblyLine line_parser(std::string linha){
    AssemblyLine linha_parseada = {"", "", ""};

    //rotulo
    size_t pos_rotulo = linha.find(":");
    if(pos_rotulo != std::string::npos){
        linha_parseada.rotulo = linha.substr(0, pos_rotulo);

        // remove rotulo e : da linha original
        linha = linha.substr(pos_rotulo + 1);

        linha = trim(linha);
    }

    // operacao e operandos
    size_t pos_espaco = linha.find(' ');
    if(pos_espaco != std::string::npos){
        // se tiver espaco entao tem operacao e operandos
        linha_parseada.operacao = linha.substr(0, pos_espaco);
        linha_parseada.operandos = linha.substr(0, pos_espaco + 1);
    }
    else{
        // se nao tiver espaco eh apenas instrucao (ex: STOP)
        linha_parseada.operacao = linha;
    }

    return linha_parseada;
}