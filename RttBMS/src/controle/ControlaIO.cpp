#include "../Config.h"
#include "Controle.h"

/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v
*/
float ControlaIO::lePortaShield25v(uint8_t portaAnalogica) {
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
float ControlaIO::lePortaCalculoResistor(uint8_t portaAnalogica) {
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 1000000.0; // Resistor 1 - 1M
  float R2 = 99000.0; // Resistor 2 - 100k
  float value = 0;
  // Le valor da porta de entrada
  //value = analogRead(portaAnalogica);
  int x = 500;
  for(int i = 1; i<=x;i++){
    value += (float)analogRead(portaAnalogica);
  }
  value = value/x;
  vout = (value * TENSAO_REFERENCIA) / 1024.0;
  vin = vout / (R2/(R1+R2));
  return vin;
}
/*
Le Dados porta Analogica analogReader
Usando bloco de divisores de tensao.
*/
float ControlaIO::lePortaDivisor(uint8_t portaAnalogica, float referencia) {
  float total=0;
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
  }
  total = total / (float)AMOSTRAS;
  if(referencia<=0){
    Serial.print("#------------------- leitura relacao porta = ");
    Serial.print(portaAnalogica);
    Serial.print(", valor ");
    Serial.print(total,5);
    Serial.println("/Tensao Multimetro");
  }
  //Divide pela relacao
  total = total/referencia;
  if(total<0.1){
    total = 0.0;
  }
  //total  =4.1;
  return total;
}
