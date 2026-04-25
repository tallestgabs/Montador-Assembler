FALSO: EQU 0
VERDADE: EQU 1

SeCtIoN DaTa ; Isso testa se ele ignora maiusculas/minusculas e inverte a ordem
N1: SPACE
N2: SPACE

SECTION TEXT
INPUT N1
COPY N1,     N2 ; Isso testa o COPY e a remocao de espacos e comentarios

IF FALSO
ADD N1 ; Essa linha NAO pode aparecer no arquivo final

IF VERDADE
SUB N2 ; Essa linha DEVE aparecer no arquivo final

ROTULO_SOZINHO:
OUTPUT N2 ; Isso testa a quebra de linha do rotulo
STOP