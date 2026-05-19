# Montador e Simulador - Assembler Fictício

![UnB](https://github.com/user-attachments/assets/dfad9149-cb02-4821-8b15-1ed1b413db16)

## Universidade de Brasília

Este projeto contém a implementação em C++ de um montador de passagem única e simulador para uma linguagem assembly inventada. O sistema foi desenvolvido como requisito do Trabalho Prático 1 para a disciplina de Software Básico, ministrada pelo Prof. Bruno Macchiavello.

## Desenvolvedores

- **Gabriel de Castro Dias - 211055432**
- **Arthur Luiz Lima de Araújo - 232000472**

## Ambiente de Desenvolvimento

- **Sistema Operacional:** Linux (Ubuntu / Linux Mint)
- **Compilador:** GCC (G++)
- **Linguagem:** C++ (Padrão C++11 ou superior)
- **Bibliotecas:** Foram utilizadas apenas bibliotecas padrão da linguagem (STL), sem dependências adicionais.

## Instruções de Compilação

Para compilar todo o projeto e gerar o executável único chamado `montador`, utilize o seguinte comando no terminal dentro da pasta raiz:

```bash
g++ *.cpp -o montador
```

## Instruções de Execução

O programa identifica a ação a ser realizada automaticamente através da extensão do arquivo de entrada:

### 1. Pré-processamento

**Para realizar o pré-processamento de um arquivo assembly (retirada de comentários, normalização de espaços e resolução de EQU/IF), utilize:**

```bash
./montador nome_do_arquivo.asm

-> Saída: Gera um arquivo nome_do_arquivo.pre.
```

### 2. Montagem

**Para montar um arquivo pré-processado e gerar o código objeto e o arquivo de pendências, utilize:**

```bash
./montador nome_do_arquivo.pre

-> Saída: Gera os arquivos nome_do_arquivo.obj (resolvido) e nome_do_arquivo.pen (com pendências).
```

### 3. Simulação

**Para executar o simulador e rodar o código de máquina gerado, utilize:**

```bash
./montador nome_do_arquivo.obj

-> O simulador solicitará entradas do usuário via teclado (INPUT) e exibirá resultados na tela (OUTPUT).
```
