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

void ObjCelula::setReferencia(float referencia){
  _referencia = referencia;
}

float ObjCelula::getReferencia(){
  return _referencia;
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
