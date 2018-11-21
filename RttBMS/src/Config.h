/*
 _____    _     _     ____    __  __    _____
|  __ \  | |   | |   |  _ \  |  \/  |  / ____|
| |__) | | |_  | |_  | |_) | | \  / | | (___
|  _  /  | __| | __| |  _ <  | |\/| |  \___ \
| | \ \  | |_  | |_  | |_) | | |  | |  ____) |
|_|  \_\  \__|  \__| |____/  |_|  |_| |_____/

Projeto desenvolvido por Samuel Rettore
email -> rettore@gmail.com

Baseado no projeto BMSDuino @autor Claudio - Solar Souzam
Canal -> https://www.youtube.com/watch?v=0L6wfxrfS9M&t=1s

Definicao de Quantos S ...ou celulas tem o Sistema
Por exemplo um Projeto de 2S(duas celulas) segue o seguinte mapeamento
A1 para leitura de tensao e 31 para controle da celulas:
A1 - > 31
A2 - > 32
......

Define tensao inicial das celulas do banco usado para calculo, referencia da leitura
SE VOCE DEFINIR 2 CELULAS o sistema entendera que seu banco terá duas celulas,
se definir 7 usará mesma logica.

Ex 3S ou 3 CELULAS

#define aRefS1 3.89
#define aRefS2 3.89
#define aRefS3 3.89

ex 7S ou 7 CELULAS

#define aRefS1 3.89
#define aRefS2 3.89
#define aRefS3 3.89
#define aRefS4 3.89
#define aRefS5 3.89
#define aRefS6 3.89
#define aRefS7 3.89

*/
#define aRefS1 3.89
#define aRefS2 3.89
#define aRefS3 3.89
#define aRefS4 3.89
#define aRefS5 3.89
#define aRefS6 3.89
#define aRefS7 3.89

//referencia tensao total do banco
#define vbatRef 24.8

//Define pino do arduino Vbat da bateria ou leitor total do banco
#define pinoVbat 15

//Velocidade Serial Arduino
#define velocidade_serial 115200
