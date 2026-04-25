#include "preprocessor.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>


// Função auxiliar para substituir as chaves do EQU no código
std::string substituir_equ(std::string linha, const std::map<std::string, std::string>& tabela_equ) {
    for (const auto& par : tabela_equ) {
        std::string chave = par.first;
        std::string valor = par.second;
        
        size_t pos = 0;
        // Procura a palavra, garantindo que não vai substituir parte de outra palavra (ex: N1 dentro de N10)
        while ((pos = linha.find(chave, pos)) != std::string::npos) {
            bool inicio_valido = (pos == 0 || linha[pos-1] == ' ' || linha[pos-1] == ',');
            bool fim_valido = (pos + chave.length() == linha.length() || linha[pos + chave.length()] == ' ' || linha[pos + chave.length()] == ',');
            
            if (inicio_valido && fim_valido) {
                linha.replace(pos, chave.length(), valor);
                pos += valor.length();
            } else {
                pos += chave.length();
            }
        }
    }
    return linha;
}

/* 
Esta função remove todo acúcar de sintaxe, comentários e espaços desnecessários do arquivo
original, permitindo mais fácilmente realizar o parsing do .pre
*/
void pre_processar(const std::string& nome_arquivo_entrada) {
    std::ifstream arquivo_entrada(nome_arquivo_entrada);
    
    // Abre o arquivo .asm para leitura
    if (!arquivo_entrada.is_open()) {
        std::cerr << "|| Pré Processamento || >>Erro<< Nao foi possivel abrir o arquivo .asm" << nome_arquivo_entrada << std::endl;
        return;
    }

    // Cria o nome do arquivo de saida trocando .asm por .pre
    std::string nome_arquivo_saida = nome_arquivo_entrada;
    size_t pos = nome_arquivo_saida.find(".asm");
    if (pos != std::string::npos) {
        nome_arquivo_saida.replace(pos, 4, ".pre");
    } else {
        nome_arquivo_saida += ".pre"; 
    }

    // Vetores para guardar as linhas de cada secao (separar os 2)
    std::vector<std::string> secao_text;
    std::vector<std::string> secao_data;

    // Variaveis de controle
    int secao_atual = 0; // 0 = Inicio/EQU, 1 = TEXT, 2 = DATA
    std::string linha;

    // Variáveis para a lógica do checklist
    std::string rotulo_pendente = "";
    bool pular_proxima_linha = false;
    std::map<std::string, std::string> tabela_equ;

    // Lê o arquivo .asm linha por linha
    while (std::getline(arquivo_entrada, linha)) {
        // Limpeza usando o utils.cpp
        linha = remove_comments(linha); // Remove tudo depois do ;
        to_upper(linha);                // Converte tudo para maiusculo
        linha = clear_spaces(linha);    // Remove espacos extras
        linha = trim(linha);            // Tira espacos do comeco e do fim

        // Se a linha ficou vazia apos limpar, ignora e vai pra proxima
        if (linha.empty()) continue;     

        // Garante que se pode dar rótulo seguido de dois pontos e ENTER.
        // O rótulo é considerado como da linha seguinte 
        if (linha.back() == ':' && linha.find(' ') == std::string::npos) {
            rotulo_pendente = linha;
            continue; // Pula pra próxima linha para pegar a instrução
        }

        // Se tínhamos um rótulo pendente da linha anterior, junta agora
        if (!rotulo_pendente.empty()) {
            linha = rotulo_pendente + " " + linha;
            rotulo_pendente = "";
        }

        // Tratamento da Diretiva IF
        if (pular_proxima_linha) {
            pular_proxima_linha = false;
            continue; // Ignora a linha atual
        }

        if (linha.find("IF ") == 0) { // Se a linha começa com "IF "
            std::string label_if = linha.substr(3); // Pega o rótulo do valor depois de "IF "
            // Verifica se o rótulo existe no mapa EQU e se o valor é zero
            if (tabela_equ.find(label_if) != tabela_equ.end()) {
                if (tabela_equ[label_if] == "0") {
                    pular_proxima_linha = true; // Se for 0, a próxima instrução não entra no .pre
                }
            }
            continue; // O IF em si nunca vai pro .pre
        }

        // Tratamento da Diretiva EQU
        size_t pos_equ = linha.find(" EQU ");
        if (pos_equ != std::string::npos) {
            std::string label = linha.substr(0, pos_equ);
            if (label.back() == ':') label.pop_back(); // Tira o dois-pontos do rótulo
            std::string valor = linha.substr(pos_equ + 5);
            
            tabela_equ[label] = valor; // Salva o valor na memória
            continue; // O EQU nunca vai pro .pre
        }

        // Se chegamos aqui, é uma instrução real ou seção
        // Substitui qualquer valor de EQU antes de salvar
        linha = substituir_equ(linha, tabela_equ);

        // Identifica onde estamos no arquivo
        if (linha == "SECTION TEXT") {
            secao_atual = 1;
            secao_text.push_back(linha);
            continue;
        } else if (linha == "SECTION DATA") {
            secao_atual = 2;
            secao_data.push_back(linha);
            continue;
        }

        // Guarda a instrução limpa no vetor correto
        if (secao_atual == 1) {
            secao_text.push_back(linha);
        } else if (secao_atual == 2) {
            secao_data.push_back(linha);
        }
    }

    arquivo_entrada.close();

    // Grava tudo no arquivo de saida (TEXT primeiro, DATA depois)
    std::ofstream arquivo_saida(nome_arquivo_saida);
    if (!arquivo_saida.is_open()) {
        std::cerr << "|| Pré Processamento || >>Erro<< Nao foi possivel criar o arquivo " << nome_arquivo_saida << std::endl;
        return;
    }

    for (const std::string& l : secao_text) { // Escreve no arquivo o TEXT
        arquivo_saida << l << "\n";
    }
    for (const std::string& l : secao_data) { // Escreve no arquivo o DATA
        arquivo_saida << l << "\n";
    }

    arquivo_saida.close();
    std::cout << "|| Pré Processamento || >>SUCESSO<< Foi gerado o arquivo: " << nome_arquivo_saida << std::endl;
}


/* 
Comentário Checklist:
- Seções TEXT e DATA escritos no .pre com DATA por último: FEITO
- Não ser Case Sensitive: FEITO
- CONST com positivos, negativos e hexa: FUNCIONALIDADE DO ASSEMBLER
- COPY separado por "," sem espaços: FEITO (clear_spaces resolve)
- Desconsiderar Espaços e enters: FEITO (trim, clear_spaces e o if (linha.empty()) continue; resolvem)
- Rótulo sozinho na linha com ENTER: FEITO
- Comentários removidos no EQU/IF: FEITO
*/