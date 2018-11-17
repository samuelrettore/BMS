#include "Arduino.h"
#include "ObjCelula.h"

float ObjCelula::getLeitura(){
  return _leitura;
}

void ObjCelula::setLeitura(float leitura){
  _leitura = leitura;
}
