#ifndef Controle_h
#define Controle_h

#include <Arduino.h>
#include "../objetos/ObjCelula.h"
#include <EEPROM.h>

class Controle {
public:
  void IinicializaModulo();
  void calibraInicio();
  void ativaRedeDHCP();
private:
  int numero_celulas = 0;
  int vbat_tensao = 0;
};
  #endif
