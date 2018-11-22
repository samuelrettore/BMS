#ifndef ObjCelula_h
#define ObjCelula_h

#include "Arduino.h"

class ObjCelula {
  public:
    float getLeituraTensao();
    float getLeituraTensaoAnterior();
    void setLeituraTensao(float leitura);
    float getReferencia();
    void setReferencia(float referencia);
    int getNumeroCelula();
    void setNumeroCelula(int numero_celula);

  private:
    //Numero da celula
    int _numero_celula;
    float _referencia;
    float _leitura_tensao;
    float _tensao_anterior;
};
#endif
