#include "Config.h"
#include "Controle.h"
#include <Arduino.h>


/**
* Metodo inicialização do modulo
*/
void Controle::IinicializaModulo(){
  Serial.begin(velocidade_serial);
  Serial.println("## -- Iniciou Setup -- ##");

  calibraInicio();
  Serial.println("## -- Fim Setup -- ##");
}

/*
* Calibração inicial.
*/
void Controle::calibraInicio(){
  Serial.print("teste inicio");
}