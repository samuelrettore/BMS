#include "Config.h"
#include "Controle.h"
#include <Arduino.h>
#include <EEPROM.h>


/**
* Metodo inicialização do modulo
*/
void Controle::IinicializaModulo(){
  Serial.begin(velocidade_serial);
  Serial.println("## -- Iniciou Setup -- ##");
  for(int i =0; i< numero_celulas; i++){
    Serial.print("Celula = ");
    
    Serial.print(i);
  }
  float f = 10.5;
  EEPROM.begin();
  EEPROM.put(0, f);
  calibraInicio();
  Serial.println("## -- Fim Setup -- ##");
}

/*
* Calibração inicial.
*/
void Controle::calibraInicio(){
  Serial.print("teste inicio");
}
