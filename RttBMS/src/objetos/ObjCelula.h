#ifndef ObjCelula_h
#define ObjCelula_h

#include "Arduino.h"

class ObjCelula {
  public:
    float getLeituraTensao();
    float getLeituraTensaoAnterior();
    void setLeituraTensao(float leitura);
    int getNumeroCelula();
    void setNumeroCelula(int numero_celula);
    void setPortaInput(uint8_t porta);
    uint8_t getPortaInput();

  private:
    //Numero da celula
    int _numero_celula;
    uint8_t _porta_input;
    float _leitura_tensao;
    float _tensao_anterior;
};
#endif
