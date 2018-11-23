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

Define tensao inicial(leitura multimetro) das celulas do banco usado para calculo, referencia da leitura
SE VOCE DEFINIR 2 CELULAS o sistema entendera que seu banco terá duas celulas,
se definir 7 usará mesma logica.

Ex 3S ou 3 CELULAS

#define aRefS1 3.89 -> A1 le dados do divisor de tensao e 31 aciona sistema de resistor para equalizar celula
#define aRefS2 3.89 -> A2 le dados do divisor de tensao e 32 aciona sistema de resistor para equalizar celula
#define aRefS3 3.89 -> A3 le dados do divisor de tensao e 33 aciona sistema de resistor para equalizar celula

ex 7S ou 7 CELULAS

#define aRefS1 3.89 -> A1 le dados do divisor de tensao e 31 aciona sistema de resistor para equalizar celula
#define aRefS2 3.89 -> A2 le dados do divisor de tensao e 32 aciona sistema de resistor para equalizar celula
#define aRefS3 3.89 -> A3 le dados do divisor de tensao e 33 aciona sistema de resistor para equalizar celula
#define aRefS4 3.89 -> A4 le dados do divisor de tensao e 34 aciona sistema de resistor para equalizar celula
#define aRefS5 3.89 -> A5 le dados do divisor de tensao e 35 aciona sistema de resistor para equalizar celula
#define aRefS6 3.89 -> A6 le dados do divisor de tensao e 36 aciona sistema de resistor para equalizar celula
#define aRefS7 3.89 -> A7 le dados do divisor de tensao e 37 aciona sistema de resistor para equalizar celula

Abaixo pré definido sistema de 7S ou 7 celulas
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
//Configuracao do alor referncial que a bms trabalha em mV
#define bms 0.060
//Tensao Minima BMS para de atuar
#define min_voltage 24.0
//Tensao Minima por celula
#define min_voltage_cell 3.30

/*
  Outros dados
*/
//Numero de amostras coletadas pelo sistema em cada porta analogica (leitura de tensao)
#define AMOSTRAS 2000
//Configuracoes placa de rede.
#define W5100_CS  10
#define SDCARD_CS 4
//Velocidade Serial Arduino
#define velocidade_serial 115200
