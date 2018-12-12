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
    //CELULAS
    ObjCelula getCelula(int posicao);
    void setCelula(ObjCelula objeto, int posicao);
    float getTensaoBanco();
  private:
    //Numero de celulas do banco
    int _quantidade_celulas = 0;
    ObjCelula* celulas;
};
#endif
