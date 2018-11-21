#include <Arduino.h>
/**
* Projeto desenvolvido por Samuel Rettore
* Baseado no projeto https://www.youtube.com/watch?v=0L6wfxrfS9M&t=1s
* BMSDuino @autor Claudio - Souzam
*/
#include "controle/Controle.h"
#include "objetos/ObjCelula.h"
int led = 13;

//Objeto Controle
Controle ctrl;

int i = 0;

void setup() {
  ctrl.IinicializaModulo();
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);
  Serial.print("teste = ");
  Serial.println(i++);
  // put your main code here, to run repeatedly:
}
