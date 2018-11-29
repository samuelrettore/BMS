#include "Arduino.h"
#include "ObjCelula.h"

float ObjCelula::getLeituraTensao(){
  return _leitura_tensao;
}

void ObjCelula::setLeituraTensao(float leitura){
  //copia tensao anterior
  _tensao_anterior = _leitura_tensao;
  _leitura_tensao = leitura;
}

int ObjCelula::getNumeroCelula(){
  return _numero_celula;
};

void ObjCelula::setNumeroCelula(int numero_celula){
  _numero_celula = numero_celula;
}

float ObjCelula::getLeituraTensaoAnterior(){
  return _tensao_anterior;
}
