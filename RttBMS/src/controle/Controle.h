#ifndef Controle_h
#define Controle_h

#include <Arduino.h>
//#include "../objetos/ObjCelula.h"
#include "../objetos/BancoBateria.h"

class Controle {
public:
  void inicializaModulo(BancoBateria* bateria);
  void calibraInicio();
  void ativaRedeDHCP();
  void verificaReferenciaCalculo();
  void ciloProcessamento();
private:
  BancoBateria* _bateria;
  int vbat_tensao = 0;
};
  #endif
