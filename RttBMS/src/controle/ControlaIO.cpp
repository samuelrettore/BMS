#include "../Config.h"
#include "Controle.h"

/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v
*/
float ControlaIO::lePorta(uint8_t portaAnalogica) {
  int valorDC = 0;
  float tensaoDC = 0;
  valorDC = analogRead(portaAnalogica);
  tensaoDC = ((valorDC*0.00489)*TENSAO_REFERENCIA);
  return tensaoDC;
}

/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v Modelo NET
*/
float ControlaIO::lePorta_calculoresitor(uint8_t portaAnalogica) {
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 30000.0; // Resistor 1
  float R2 = 7500.0; // Resistor 2
  int value = 0;
  // read the value at analog input
  value = analogRead(portaAnalogica);
  vout = (value * 5.0) / 1024.0; // see text
  vin = vout / (R2/(R1+R2));
  return vin;
}

/*
Le Dados porta Analogica analogReader
Usando bloco de divisores de tensao.
*/
float ControlaIO::lePortaDivisor(uint8_t portaAnalogica) {
  float total=0;
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
  }
  total = total / (float)AMOSTRAS;
  if(RELACAO<=0){
    Serial.print("#------------------- leitura relacao ==== ");
    Serial.print(total,5);
    Serial.println("/Tensao Multimetro");
  }
  //Divide pela relacao
  total = total/RELACAO;
  //total  =4.1;
  return total;
}
