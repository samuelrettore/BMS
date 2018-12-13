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


/*
Le Dados porta Analogica analogReader
Usando Shield de tensão 0-25v Modelo NET
*/
float ControlaIO::lePorta3(uint8_t portaAnalogica) {
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
