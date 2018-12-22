#ifndef ObjCelula_h
#define ObjCelula_h

#include "Arduino.h"
#include "../Config.h"

class ObjCelula {
  public:
    float getLeituraTensao();
    float getLeituraTensaoAnterior();
    void setLeituraTensao(float leitura);
    int getNumeroCelula();
    void setNumeroCelula(int numero_celula);
    void setPortaInput(int porta);
    int getPortaInput();
    void setPortaControle(int porta);
    int getPortaControle();
    int getPercentual();
    void setReferencia(float referencia);
    float getReferencia();
  private:
    //Numero da celula
    int _numero_celula;
    int _porta_input;
    int _porta_controle;
    float _referencia;
    float _leitura_tensao;
    float _tensao_anterior;
    float _percentual;
};
#endif
