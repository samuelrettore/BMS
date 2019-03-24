#ifndef BancoBateria_h
#define BancoBateria_h
#include "Arduino.h"
#include "ObjCelula.h"

class BancoBateria {
  public:
    //construtor
    int getQuantidadeCelulas();
    void setQuantidadeCelulas(int numero_celula);
    void inicializaBanco();
    void imprimeDados();
    int getPercentual();
    //CELULAS
    ObjCelula getCelula(int posicao);
    void setCelula(ObjCelula objeto, int posicao);
    float getTensaoBanco();
  private:
    //Numero de celulas do banco
    int _quantidade_celulas = 0;
    float _percentual;
    ObjCelula* celulas;
};
#endif
