#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "preprocessor.h"
#include "assembler.h"
#include "simulator.h"
#include "parser.h" 

//funcao para testar o parser
void parser_test(std::string &file) {
    std::cout << "\n=== INICIANDO TESTE DO PARSER ===\n\n";
    std::cout << "\n=== LENDO ARQUIVO: " << file << "===\n\n";

    // abre arquivo
    std::ifstream arquivo(file);
    if(!arquivo.is_open()){
        std::cerr << "Erro: Nao foi possivel abrir o arquivo" << file << "\n";
        return;
    }

    std::string linha;
    int numero_linha = 1;

    while(std::getline(arquivo, linha)){

        //passa linha por linha para o parser
        AssemblyLine linha_atual = line_parser(linha);

        //mostra os resultados
        std::cout << "Linha " << numero_linha << ": [" << linha << "]\n";
        std::cout << "  Rotulo:    [" << linha_atual.rotulo << "]\n";
        std::cout << "  Operacao:  [" << linha_atual.operacao << "]\n";
        std::cout << "  Operandos: [" << linha_atual.operandos << "]\n";
        std::cout << "--------------------------------------\n";

        numero_linha++;

    }

    arquivo.close();
    std::cout << "=== FIM DA LEITURA ===\n\n";

}






// compilar com g++ *.cpp -o namefile 

int main(int argc, char* argv[]){

    if (argc < 2) return 1;
    std::string filename = argv[1];


    // =========== MODO TESTE DO PARSER =============
    // remova os comentarios apenas quando for testar
    parser_test(filename);
    return 0;
    // ==============================================
    // ==============================================

    
    if (filename.find(".asm") != std::string::npos) {
        pre_processar(filename);
    } else if (filename.find(".pre") != std::string::npos) {
        // Chama o montador -> gera .obj e .pen 
    } else if (filename.find(".obj") != std::string::npos) {
        // Chama o simulador
    }
    return 0;
}