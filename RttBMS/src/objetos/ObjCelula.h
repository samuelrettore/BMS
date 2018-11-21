#ifndef ObjCelula_h
#define ObjCelula_h

#include "Arduino.h"

class ObjCelula {
  public:
    float getLeitura();
    void setLeitura(float leitura);

  private:
    float _leitura;
};
#endif
