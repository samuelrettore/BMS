#include "Arduino.h"
#include "BancoBateria.h"
#include "ObjCelula.h"

void BancoBateria::setQuantidadeCelulas(int numero_celula){
  _quantidade_celulas =  numero_celula;
}

int BancoBateria::getQuantidadeCelulas(){
  return _quantidade_celulas;
}

void BancoBateria::inicializaBanco(){
  if(_quantidade_celulas >0 ){
    celulas = new ObjCelula[_quantidade_celulas];
  }
}

ObjCelula BancoBateria::getCelula(int posicao){
  return celulas[posicao];
}

void BancoBateria::setCelula(ObjCelula objeto, int posicao){
  celulas[posicao] = objeto;
}
