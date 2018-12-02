#include "../Config.h"
#include "Controle.h"

/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v
*/
float ControlaIO::lePorta(int portaAnalogica) {
  float total=0;
  int valorDC = 0;
  float tensaoDC = 0;
  for (int i=0; i<AMOSTRAS; i++) {
    valorDC = analogRead(portaAnalogica);
    tensaoDC = ((valorDC*0.00489)*5);
    total += tensaoDC;
  }
  return total / (float)AMOSTRAS;
}


/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v Modelo China
*/
float ControlaIO::lePorta_modo2(int portaAnalogica) {
  float total=0;
  int valorDC = 0;
  float tensaoDC = 0;
  for (int i=0; i<AMOSTRAS; i++) {
    float temp = 0;
    int val11 = analogRead(portaAnalogica);
    temp = val11/4.092;
    val11 = (int) temp; //
    valorDC = ((val11%100)/10);
    total += tensaoDC;
  }
  return total / (float)AMOSTRAS;
}
