#include <Arduino.h>
#include "controle/Controle.h"
#include "objetos/BancoBateria.h"
//Objeto Controle
Controle ctrl;
BancoBateria* bateria = new BancoBateria();

void setup() {
  ctrl.inicializaModulo(bateria);
}

void loop() {
  //Herança teste
  //ctrl.dizOI();
  ctrl.ciloProcessamento();
  float x = ctrl.lePorta(0);
  Serial.print("Leitura = ");
  Serial.println(x);
  delay(3000);
}
