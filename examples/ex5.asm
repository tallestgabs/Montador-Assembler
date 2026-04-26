; ======================================================
; TESTE 5: ROTULOS E CONSTANTES NEGATIVAS/HEXA
; ======================================================
UM_HEXA EQU 0XA1

SECTION DATA
NEGATIVO: CONST -10
HEXA: CONST UM_HEXA

SECTION TEXT
INICIO:
    LOAD NEGATIVO

PULO:
    ; Vamos colocar varias quebras e comentarios entre o rotulo e a instrucao
    ; O programa tem que saber segurar esse rotulo "PULO:" na memoria

    JMP PULO
    STOP