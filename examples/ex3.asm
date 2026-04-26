; ======================================================
; TESTE 3: TRATAMENTO AGRESSIVO DO COPY
; ======================================================

SECTION TEXT


ROTULO:

COPY    A   ,    B
     COPY C,D    ; Tudo colado direto
COPY E ,F        ; Espaco so antes
COPY G, H        ; Espaco so depois

STOP

SECTION DATA
A: SPACE
B: SPACE
C: SPACE
D: SPACE
E: SPACE
F: SPACE
G: SPACE
H: SPACE