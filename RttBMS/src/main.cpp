#include <Arduino.h>
#include "controle/Controle.h"
#include "objetos/BancoBateria.h"
//Objeto Controle
Controle ctrl;
BancoBateria* bateria = new BancoBateria();
unsigned long time_1 = 0;

void setup() {
  ctrl.inicializaModulo(bateria);
}

void loop() {
  //Loop 1s
  if(millis() > time_1 + 500){
    time_1 = millis();
    ctrl.ciloProcessamento();
    //Atualiza dados no MQTT
  }
}
