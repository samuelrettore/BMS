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

float BancoBateria::getTensaoBanco(){
  float total = 0.0;
  for(int i=0; i<getQuantidadeCelulas();i++){
    ObjCelula obj = getCelula(i);
    total =+ obj.getLeituraTensao();
  }
  return total;
}

/*
Imprime dado das celulas e do Banco.
*/
void BancoBateria::imprimeDados(){
  Serial.println("#----- Dados celulas ----- ###");
  int portaLe = 8;
  for(int i=0; i<getQuantidadeCelulas();i++){
    ObjCelula objj = getCelula(i);
    Serial.print("Numero Celula = ");
    Serial.println(objj.getNumeroCelula());
    Serial.print("Tensao anterior = ");
    Serial.println(objj.getLeituraTensaoAnterior(),5);
    Serial.print("Tensao = ");
    Serial.println(objj.getLeituraTensao(),5);
    Serial.print("Porta Leitura = A");
    Serial.println(portaLe);
    Serial.print("Porta Controle = ");
    Serial.println(objj.getPortaControle());
    Serial.print("Percentual = ");
    Serial.print(objj.getPercentual());
    Serial.println("%");

    Serial.println("#----- Fim dados celulas ----- ###");
    Serial.println();
    portaLe++;
  }
  Serial.println("#----- Dados banco de baterias ----- ###");
  Serial.print("Quantidade de celulas = ");
  Serial.println(getQuantidadeCelulas());
  Serial.println();
  Serial.print("Total de tensao = ");
  Serial.println(getTensaoBanco(),5);
  Serial.println("#----- Fim dados banco de baterias ----- ###");
}
