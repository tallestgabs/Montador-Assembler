#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "preprocessor.h"
#include "assembler.h"
#include "simulator.h"
#include "parser.h" 


void imprimir_tabela(const std::string& titulo, const std::string& conteudo) {
    std::cout << "\n" << titulo << "\n";
    std::cout << "+----------+----------+------------+------------+\n";
    std::cout << "| End      | Opcode   | Operando 1 | Operando 2 |\n";
    std::cout << "+----------+----------+------------+------------+\n";

    if (conteudo.empty()) {
        std::cout << "| (Arquivo vazio ou nao encontrado)             |\n";
        std::cout << "+----------+----------+------------+------------+\n";
        return;
    }

    // Converte a string de números do arquivo para um vetor
    std::vector<int> mem;
    std::stringstream ss(conteudo);
    int num;
    while (ss >> num) {
        mem.push_back(num);
    }

    int pc = 0;
    bool secao_dados = false; // Flag para saber quando chegamos nas variáveis

    while (pc < mem.size()) {
        int val = mem[pc];

        // Imprime Endereço e Opcode/Dado
        std::cout << "| " << std::left << std::setw(8) << pc 
                  << " | " << std::setw(8) << val << " | ";

        // Se já passamos do STOP, tudo é apenas dado/variável (ocupa 1 espaço)
        if (secao_dados) {
            std::cout << std::setw(10) << "-" << " | " << std::setw(10) << "-" << " |\n";
            pc++;
            continue;
        }

        // Lógica de avanço baseada na Tabela de Instruções
        if (val == 14) { // STOP (0 operandos)
            std::cout << std::setw(10) << "-" << " | " << std::setw(10) << "-" << " |\n";
            secao_dados = true; // Avisa que as próximas linhas são da SECTION DATA
            pc++;
        }
        else if (val == 9) { // COPY (2 operandos)
            std::string op1 = (pc + 1 < mem.size()) ? std::to_string(mem[pc+1]) : "?";
            std::string op2 = (pc + 2 < mem.size()) ? std::to_string(mem[pc+2]) : "?";
            std::cout << std::setw(10) << op1 << " | " << std::setw(10) << op2 << " |\n";
            pc += 3;
        }
        else if (val >= 1 && val <= 13) { // Restante das instruções (1 operando)
            std::string op1 = (pc + 1 < mem.size()) ? std::to_string(mem[pc+1]) : "?";
            std::cout << std::setw(10) << op1 << " | " << std::setw(10) << "-" << " |\n";
            pc += 2;
        }
        else { // Valor solto ou erro (trata como tamanho 1)
            std::cout << std::setw(10) << "-" << " | " << std::setw(10) << "-" << " |\n";
            pc++;
        }
    }
    std::cout << "+----------+----------+------------+------------+\n";
}

//funcao para testar o parser
void parser_test(std::string &file) {
    std::cout << "\n=== MODO TESTE: INICIANDO TESTE DO PARSER ===\n\n";
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



void assembler_test(std::string &file) {
    std::cout << "\n=== MODO TESTE: ASSEMBLER (PASSAGEM UNICA) ===\n";
    
    montar(file);

    std::string base = file.substr(0, file.find(".pre"));
    std::string s_pen, s_obj;
    
    std::ifstream f_pen(base + ".pen");
    if(std::getline(f_pen, s_pen)) {
        imprimir_tabela("[SAIDA .PEN] (Com pendencias -1)", s_pen);
    }

    std::ifstream f_obj(base + ".obj");
    if(std::getline(f_obj, s_obj)) {
        imprimir_tabela("[SAIDA .OBJ] (Final Resolvido)", s_obj);
    }
}





// compilar com g++ *.cpp -o namefile 

int main(int argc, char* argv[]){

    if (argc < 2) return 1;
    std::string filename = argv[1];


    // =========== MODO TESTE DO PARSER =============
    // remova os comentarios apenas quando for testar
    //parser_test(filename);
    //return 0;
    // ==============================================

    // ========== MODO TESTE DO ASSEMBLER ===========
    // Descomente as duas linhas abaixo para ver o relatorio no terminal
    assembler_test(filename);
    return 0;
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