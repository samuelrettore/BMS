#include "../Config.h"
#include "Controle.h"

/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v
*/
float ControlaIO::lePorta(uint8_t portaAnalogica) {
  float total=0;
  int valorDC = 0;
  float tensaoDC = 0;
  for (int i=0; i<AMOSTRAS; i++) {
    valorDC = analogRead(portaAnalogica);
    tensaoDC = ((valorDC*0.00489)*TENSAO_REFERENCIA);
    total += tensaoDC;
  }
  return total / (float)AMOSTRAS;
}


/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v Modelo China
*/
float ControlaIO::lePorta2(uint8_t portaAnalogica) {
  int val11;
  float val2;
  float temp;
  val11=analogRead(portaAnalogica);
  temp=val11/4.092;
  val2=(temp/10);
  Serial.println(val2);
  delay(2000);
  return val2;
}
