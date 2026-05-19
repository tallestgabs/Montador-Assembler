#include "simulator.h"
#include <iostream>
#include <fstream>
#include <iomanip>

// copia a linha do .obj para o vetor (nossa RAM simulada)
void simulator(std::string &objFile){

    // abre o arquivo
    std::ifstream arquivo(objFile);
    if(!arquivo.is_open()){
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo " << objFile << "\n";
        return;
    }

    // vetor que simula a ram
    std::vector<int> ram;
    int code;

    // coloca os numeros do arquivo na ram
    while (arquivo >> code) {
        ram.push_back(code);
    }
    arquivo.close();

    if(ram.empty()){
        std::cerr << "ERRO: A RAM esta vazia. Arquivo .obj sem conteudo. \n";
        return;
    }

    // registradores do processador simulado
    int ACC = 0;  // acumulador
    int PC = 0;   // program counter
    bool running = true;

    std::cout << "\n ===== INICIANDO SIMULADOR ====\n";

    while (PC < ram.size() && ram[PC] != 14) {

        switch(ram[PC]) {  

            case 1:     //ADD
                ACC += ram[ram[PC+1]];
                PC += 2; 
                break;

            case 2:     //SUB
                ACC -= ram[ram[PC+1]];
                PC += 2; 
                break;

            case 3:     //MUL
                ACC *= ram[ram[PC+1]];
                PC += 2; 
                break;

            case 4:     //DIV
                if(ram[ram[PC+1]] == 0){
                    std::cerr << "ERRO: Divisao por zero\n";
                    return;
                }
                ACC /= ram[ram[PC+1]];
                PC += 2; 
                break;

            case 5:     //JMP
                PC = ram[PC+1];
                break;

            case 6:     //JMPN
                if(ACC < 0){
                    PC = ram[PC+1];
                    break;
                }
                PC += 2; 
                break;

            case 7:     //JMPP
                if(ACC > 0){
                    PC = ram[PC+1];
                    break;
                }
                PC += 2; 
                break;

            case 8:     //JMPZ
                if(ACC == 0){
                    PC = ram[PC+1];
                    break;
                }
                PC += 2; 
                break;

            case 9:     //COPY
                ram[ram[PC+2]] = ram[ram[PC+1]];  // mem(op2) <- mem(op1)
                PC += 3; // COPY ocupa 2 enderecos de argumento, portanto devemos pular 3
                break;

            case 10:    //LOAD
                ACC = ram[ram[PC+1]];
                PC += 2; 
                break;

            case 11:    //STORE
                ram[ram[PC+1]] = ACC;
                PC += 2; 
                break;

            case 12:    //INPUT
            {
            std::cout << "Digite um valor inteiro\n";
                std::string entrada;
                std::cin >> entrada;
                // O terceiro parametro sendo 0 faz o C++ detectar automaticamente 
                // se é decimal (ex: 15) ou hexa (ex: 0x0F)
                ram[ram[PC+1]] = std::stoi(entrada, nullptr, 0);
                PC += 2; 
            }
                break;
            

            case 13:    //OUTPUT
                std::cout << "SAIDA: " << ram[ram[PC+1]] << '\n';
                PC += 2; 
                break;

            default:
                std::cerr << "ERRO: Opcode desconhecido -> " << ram[PC] << " no endereco " << PC << "\n";
                return;

        }
        
    }

    std::cout << "\n ===== FIM DO PROGRAMA ===== \n";
}


