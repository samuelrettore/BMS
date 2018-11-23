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

/*
Imprime dado das celulas e do Banco.
*/
void BancoBateria::imprimeDados(){
  Serial.println("#----- Dados banco de baterias ----- ###");
  Serial.print("Quantidade de celulas = ");
  Serial.println(getQuantidadeCelulas());
  Serial.println();
  Serial.println("#----- Dados celulas ----- ###");
  for(int i=0; i<getQuantidadeCelulas();i++){
    ObjCelula objj = getCelula(i);
    Serial.print("Numero Celula = ");
    Serial.println(objj.getNumeroCelula());
    Serial.print("referencia = ");
    Serial.println(objj.getReferencia());
    Serial.print("Tensao = ");
    Serial.println(objj.getLeituraTensao());
    Serial.print("Tensao anterior = ");
    Serial.println(objj.getLeituraTensaoAnterior());
    Serial.println("#----- Fim dados celulas ----- ###");
    Serial.println();
  }
  Serial.println("#----- Fim dados banco de baterias ----- ###");
}
