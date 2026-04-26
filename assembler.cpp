#include "assembler.h"
#include "parser.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Tabela de Instruções: {Opcode Numérico, Tamanho (PC Increment), Número de Operandos}
const std::map<std::string, Instruction> tabela_instrucoes = {
    {"ADD",    {1,  2, 1}}, // ACC <- ACC + mem(OP)
    {"SUB",    {2,  2, 1}}, // ACC <- ACC - mem(OP)
    {"MUL",    {3,  2, 1}}, // ACC <- ACC × mem(OP)
    {"DIV",    {4,  2, 1}}, // ACC <- ACC ÷ mem(OP)
    {"JMP",    {5,  2, 1}}, // PC <- OP
    {"JMPN",   {6,  2, 1}}, // Se ACC<0 então PC <- OP
    {"JMPP",   {7,  2, 1}}, // Se ACC>0 então PC <- OP
    {"JMPZ",   {8,  2, 1}}, // Se ACC=0 então PC <- OP
    {"COPY",   {9,  3, 2}}, // mem(OP2) <- mem(OP1)
    {"LOAD",   {10, 2, 1}}, // ACC <- mem(OP)
    {"STORE",  {11, 2, 1}}, // mem(OP) <- ACC
    {"INPUT",  {12, 2, 1}}, // mem(OP) <- entrada
    {"OUTPUT", {13, 2, 1}}, // saída <- mem(OP)
    {"STOP",   {14, 1, 0}}  // Suspende a execução
};

// Importante! Como temos apenas 1 registrador, a maioria das instruções tem 1 operando apenas


// Tabela de Diretivas: {Tamanho em memória}
const std::map<std::string, Directive> tabela_diretivas = {
    {"SPACE", {1}},
    {"CONST", {1}},
    {"SECTION", {0}}
};


// Função de Montagem
void montar(const std::string& nome_arquivo_pre) { // Recebe o arquivo .pre do preprocessamento
    std::ifstream arquivo_entrada(nome_arquivo_pre); // Lê o arquivo fonte
    
    if (!arquivo_entrada.is_open()) { // Se não abriu, dá erro
        std::cerr << "|| Assembler || >>ERRO<< Nao foi possivel abrir o arquivo " << nome_arquivo_pre << std::endl;
        return;
    }

    // Nomes dos arquivos de saída (é o mesmo)
    std::string nome_arquivo_obj = nome_arquivo_pre;
    std::string nome_arquivo_pen = nome_arquivo_pre;
    
    // Muda o nome da extensão no final
    size_t pos = nome_arquivo_obj.find(".pre");
    if (pos != std::string::npos) {
        nome_arquivo_obj.replace(pos, 4, ".obj");
        nome_arquivo_pen.replace(pos, 4, ".pen");
    } else {
        // Para caso não tenha extensão (o que não deve na teoria acontecer)
        nome_arquivo_obj += ".obj";
        nome_arquivo_pen += ".pen";
    }

    std::map<std::string, SymbolData> tabela_simbolos; // Cria a tabela de símbolos antes da 1a passagem
    
    // Arrays para guardar o estado do código de máquina
    std::vector<std::string> codigo_obj; // Terá as pendências resolvidas
    std::vector<std::string> codigo_pen; // Manterá os "buracos" (-1) não resolvidos



    /*
    ALGORITMO DE PASSAGEM ÚNICA CONFORME SLIDE:

    - Quando uma referência é feita a um rótulo, o montador irá procurar 
    o símbolo da tabela de símbolo da mesma forma que fizemos antes. 
    Aqui, temos várias possibilidades.
	
        - Se todos os símbolos referidos na linha lida 
        (instrução simbólica) já estão definidos, então a instrução de máquina é gerada diretamente, 
        de forma completa. Neste caso o símbolo estará marcado como “definido = true” na 
        Tabela de Símbolos (TS) e o valor (endereço) já é conhecido.
	
        - Se o símbolo não estiver na TS, então inserimos o mesmo e marcamos 
        “definido = false”. Um campo é criado na TS para apontar para uma lista de símbolos indefinidos.
    
    - Se o símbolo encontrado já estiver na TS mas ainda não estiver definido,
    então ele é inserido na lista especifica do símbolo e “ligado” com os itens que já se encontram na lista.
    
    - Quando, finalmente, o símbolo aparece como rótulo, 
    o seu valor (que é dado pelo valor do location counter) é usado para
    preencher os campos especificados na lista.

    - Por último, esse valor é colocado na TS,
    com a indicação de “símbolo definido”. Daqui para a frente,
    novas ocorrências do símbolo serão substituídas diretamente pelo valor que está na TS
    */



    // A partir daqui é básicamente o algoritmo que tá no slide
    int contador_posicao = 0;
    int contador_linha = 1;
    std::string linha_texto;

    std::cout << "Iniciando a Passagem Unica..." << std::endl;

    while (std::getline(arquivo_entrada, linha_texto)) {
        
        // Obtém uma linha do fonte
        // (O line_parser já separa rótulo, operação e operandos)
        AssemblyLine linha = line_parser(linha_texto);

        // Comentários já foram removidos no .pre


        // --- PASSO 1: TRATAMENTO DE RÓTULOS ---
        // Se existe rótulo, procura rótulo na TS (Tabela de símbolos)
        if (!linha.rotulo.empty()) {
            if (tabela_simbolos.find(linha.rotulo) != tabela_simbolos.end()) {
                // Se achou: Erro: Símbolo Redefinido
                // Dá erro pois não podemos definir o mesmo LABEL duas vezes
                if (tabela_simbolos[linha.rotulo].is_defined) {
                    std::cerr << "|| Assembler || >>ERRO SEMANTICO<<  (Linha " << contador_linha << "): Símbolo redefinido (" << linha.rotulo << ")" << std::endl;
                } else {
                    // Senão: Insere rótulo e contador_posição na TS
                    // BACKPATCHING: Resolve as pendências passadas no array .obj

                    tabela_simbolos[linha.rotulo].is_defined = true;    // Levanta a flag de encontrado 
                    tabela_simbolos[linha.rotulo].address = contador_posicao; // Anota o endereço onde foi encontrado
                    
                    for (int pos_pendente : tabela_simbolos[linha.rotulo].pendencies) { // Vai na lista de pendências
                        codigo_obj[pos_pendente] = std::to_string(contador_posicao); // Resolve cada pendência para este rótulo
                    }
                    tabela_simbolos[linha.rotulo].pendencies.clear();   // Limpa os endereços de pendências do rótulo, pois achamos
                }
            } else {
                tabela_simbolos[linha.rotulo] = {contador_posicao, true, {}}; // Se não está na tabela de símbolos, coloca nela
            }
        }

        // Esse if anterior é um pouco confuso, então o resumo é:
        /*
        - Se a linha tem rótulo, está na TS e ele é definido, dá erro de duplicata

        - Se a linha tem rótulo, está na TS e ele não é definido, define ele e resolve pendências no .obj

        - Se a linha tem rótulo e ele não está na TS, coloca na TS com definido=true e lista de pendências vazia.
        */

        // Então é só a parte que resolve a tabela de símbolos para rótulos


        // --- PASSO 2: TRATAMENTO DE OPERAÇÕES E DIRETIVAS ---
        /*
            Essa parte é basicamente o que estáva descrito 
            no comentário lá em cima de "ALGORITMO DE PASSAGEM ÚNICA CONFORME SLIDE"
            Ele é o que realmente escreve código nos strings antes de passar pros arquivos
        */
        if (!linha.operacao.empty()) {
            
            // Se existe operação, procura rótulo na TI (Tabela de instruções)
            if (tabela_instrucoes.find(linha.operacao) != tabela_instrucoes.end()) {
                // Se a operação é válida, fazemos:

                Instruction inst = tabela_instrucoes.at(linha.operacao);
                
                // Tratamos o Opcode (Palavra 1)
                codigo_obj.push_back(std::to_string(inst.opcode));
                codigo_pen.push_back(std::to_string(inst.opcode));
                contador_posicao++; // Incremento da primeira palavra (passa um endereço para ir para os operandos)
                
                // Tratamos os Operandos (Palavras seguintes)
                // Separa os operandos (ex: "N1,N2" -> ["N1", "N2"])
                std::vector<std::string> operandos_separados;
                std::stringstream ss(linha.operandos);
                std::string item;
                while (std::getline(ss, item, ',')) {
                    operandos_separados.push_back(trim(item));
                }

                // Processa cada operando
                for (const std::string& op : operandos_separados) {
                    if (tabela_simbolos.find(op) != tabela_simbolos.end()) {
                        
                        if (tabela_simbolos[op].is_defined) {

                        /*
                        Se o símbolo analisado agora (instrução simbólica)
                        já estão definido, então a instrução de máquina é gerada diretamente, 
                        de forma completa. Neste caso o símbolo estará marcado como “definido = true”
                        na Tabela de Símbolos (TS) e o valor (endereço) já é conhecido.
                        */

                            codigo_obj.push_back(std::to_string(tabela_simbolos[op].address));
                            codigo_pen.push_back(std::to_string(tabela_simbolos[op].address));
                        } else {

                            /*
                            Se o símbolo encontrado já estiver na TS mas ainda não estiver definido,
                            então ele é inserido na lista especifica do símbolo 
                            e “ligado” com os itens que já se encontram na lista.
                            */
                            
                            // Referência futura de um símbolo já existente na tabela
                            tabela_simbolos[op].pendencies.push_back(contador_posicao);
                            codigo_obj.push_back("-1"); // Será substituído no futuro
                            codigo_pen.push_back("-1"); // Fica com o "buraco" para o arquivo .pen
                        }
                    } else {

                        /*
                        Se o símbolo não estiver na TS, então inserimos o mesmo e marcamos 
                        “definido = false”. Um campo é criado na TS para apontar 
                        para uma lista de símbolos indefinidos.
                        */

                        // Símbolo totalmente novo aparecendo como operando primeiro
                        tabela_simbolos[op] = {-1, false, {contador_posicao}};
                        codigo_obj.push_back("-1");
                        codigo_pen.push_back("-1");
                    }
                    contador_posicao++; // Para cada operando, pulamos 1 endereço
                    // Dessa forma, a maioria das operações vai ocupar 2 endereços (Opcode + operando único)
                    // Enquanto o COPY vai ocupar 3 (Opcode + 2 operandos)
                    // E o STOP vai ocupar 1 (Opcode apenas)
                }
            } 
            // Verifica se é diretiva (não gera opcode, apenas armazena 1 endereço)
            else if (tabela_diretivas.find(linha.operacao) != tabela_diretivas.end()) {
                if (linha.operacao == "SPACE") {
                    codigo_obj.push_back("0"); // Conforme especificação: 00 ou 0
                    codigo_pen.push_back("0");
                    contador_posicao++;
                } 
                else if (linha.operacao == "CONST") {
                    // std::stoi com base 0 reconhece hexadecimais (0x) nativamente e negativos
                    int valor_constante = std::stoi(linha.operandos, nullptr, 0); // converte pra inteiro (permite que o usuário coloque em hexadecimal mas vira inteiro de qualquer jeito)
                    codigo_obj.push_back(std::to_string(valor_constante));
                    codigo_pen.push_back(std::to_string(valor_constante));
                    contador_posicao++;
                }
                else if (linha.operacao == "SECTION") {
                    // Não faz nada! Não gera código objeto e não aumenta o contador_posicao.
                    // Apenas reconhece que é uma diretiva válida para não dar erro léxico.
                    
                    // Talvez if (linha.operandos == "TEXT") pra testar semântica depois
                }
            }
            else {
                std::cerr << "|| Assembler || >>Erro Lexico/Sintatico<< (Linha " << contador_linha << "): Operacao invalida (" << linha.operacao << ")" << std::endl;
            }
        }
        contador_linha++; // Vai pra próxima linha
    }

    arquivo_entrada.close(); // Fecha o .pre

    // --- PASSO 3: GERAÇÃO DOS ARQUIVOS DE SAÍDA ---
    
    // Geração do myfile.obj (Pendências resolvidas)
    std::ofstream arq_obj(nome_arquivo_obj);
    for (size_t i = 0; i < codigo_obj.size(); i++) {
        arq_obj << codigo_obj[i] << (i == codigo_obj.size() - 1 ? "" : " ");
    }
    arq_obj.close();
    std::cout << "Gerado: " << nome_arquivo_obj << std::endl;

    // Geração do myfile.pen (Com as pendências não resolvidas marcadas como -1)
    std::ofstream arq_pen(nome_arquivo_pen);
    for (size_t i = 0; i < codigo_pen.size(); i++) {
        arq_pen << codigo_pen[i] << (i == codigo_pen.size() - 1 ? "" : " ");
    }
    arq_pen.close();
    std::cout << "Gerado: " << nome_arquivo_pen << std::endl;
}