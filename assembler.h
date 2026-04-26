#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <map>

// Estrutura para a Tabela de Instruções
struct Instruction {
    /*
    - Contém os opcodes simbólicos acompanhadas das informações para 
    gerar código de máquina
    - Basicamente, esta tabela contém o formato da instrução e opcode numérico.
    - Esta tabela está pronta, isto é, faz parte do código do montador.
    */
    int opcode;        // Código da Instrução
    int size;          // Espaço que a instrução ocupará na memória (PC increment)
    int num_operands;  // Formato da instrução: Quantos argumentos ela espera (0, 1 ou 2), 
                       // num_operands sempre é SIZE - 1
};

// Estrutura para a Tabela de Diretivas
struct Directive {
    /*
    - Contém as diretivas da linguagem. Em geral, 
    para cada diretiva existe uma rotina que 
    implementa as ações correspondentes. Esta tabela também está pronta.
    - Nos exemplos, nós vamos assumir que as diretivas 
    CONST and SPACE ocupam 1 palavra de memória (16 bits)
    */
    int size;
};

// Tabela de Símbolos (A estrutura fundamental da Passagem Única)
struct SymbolData {
    /*
    Contém todos os símbolos definidos no programa e seus atributos. 
    Essa tabela é criada na 1a passagem e usada na 2a (não estaremos usando 2a passagem)
    */
    int address;                 // Endereço de memória onde o rótulo foi definido
    bool is_defined;             // True se já achamos a declaração dele (ex: L1: SPACE)
    std::vector<int> pendencies; // Lista de posições (PC) que ficaram com "buraco" esperando esse rótulo
};

// Declaração da função principal
void montar(const std::string& nome_arquivo_pre);

#endif