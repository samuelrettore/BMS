#include "../Config.h"
#include "Controle.h"
//EEPROM
#include <EEPROM.h>

//Le Dados porta Analogica analogReader
float ControlaIO::lePorta(int portaAnalogica) {
  float total=0;
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
  }
  return total / (float)AMOSTRAS;
}



//Le Dados porta Analogica analogReader
/*
float ControlaIO::lePorta(int portaAnalogica) {
// read the value at analog input
float vin = 0.0;
float R1 = 100000000.0; // resistance of R1 (100K) -see text!
float R2 = 100000.0; // resistance of R1 (100K) -see text!
float value = analogRead(portaAnalogica);
float vout = (value * 5.0) / 1024.0; // see text
vin = vout/(R2/(R1+R2));
if (vin<0.09) {
vin=0.0;//statement to quash undesired reading !
}
return vin;
}
*/

//Busca dados na eeprom de refecencia
float ControlaIO::eepromLeReferencia(int endereco) {
  float x;
  EEPROM.get(endereco, x);
  return x;
}

//Busca dados na eeprom de refecencia
void ControlaIO::gravaEEPROM(int endereco, float valor) {
  EEPROM.put(endereco, valor);
}

//eeprom calc
float ControlaIO::eepromCalculaReferenciaGrava(int endereco, float referencia_inicial){
  float valor_porta = lePorta(endereco);
  float x  = valor_porta/referencia_inicial;
  Serial.print("EEPROM referencia = ");
  Serial.print(valor_porta);
  Serial.print(" ,calculo = ");
  Serial.println(x);
  gravaEEPROM(endereco,x);
  delay(1000);
  return x;
}

//Zera EEprom
void ControlaIO::zeraEEPROM(){
  Serial.println("#### -------- zera eeprom ---------##");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  delay(3000);
}
