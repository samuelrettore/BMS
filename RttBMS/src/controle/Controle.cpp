#include "Config.h"
#include "Controle.h"
#include <Arduino.h>
#include <EEPROM.h>


/**
* Metodo inicialização do modulo
*/
void Controle::IinicializaModulo(){
  Serial.begin(velocidade_serial);
  Serial.println("## -- Iniciou Setup -- ##");
  calibraInicio();

  for(int i =0; i< numero_celulas; i++){
    Serial.print("Celula = ");
    Serial.print(i);
  }
  float f = 10.5;
  EEPROM.begin();
  EEPROM.put(0, f);

  Serial.println("## -- Fim Setup -- ##");
}

/*
* Calibração inicial.
*/
void Controle::calibraInicio(){
  Serial.println("Inicia Calibracao");

  #ifndef aRefS1
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS2
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS3
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS4
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS5
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS6
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS7
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS8
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS9
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  #ifndef aRefS10
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println("ativa...");
  #endif

  delay(1000);
}
