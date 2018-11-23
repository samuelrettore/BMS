#include <Arduino.h>
#include "controle/Controle.h"
#include "objetos/BancoBateria.h"
//Objeto Controle
Controle ctrl;
BancoBateria bateria;

void setup() {
  ctrl.inicializaModulo(bateria);
}

void loop() {
  ctrl.ciloProcessamento();
}
