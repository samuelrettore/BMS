#include "../Config.h"
#include "Controle.h"
//Placa de REde
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
//EEPROM
#include <EEPROM.h>

EthernetClient client;

/**
* Metodo inicialização do modulo
*/
void Controle::IinicializaModulo(){
  Serial.begin(velocidade_serial);
  Serial.println("## -- Iniciou Setup -- ##");
  calibraInicio();
  ativaRedeDHCP();

  //for(int i =0; i< numero_celulas; i++){
  //  Serial.print("Celula = ");
  //    Serial.print(i);
  //}
  //float f = 10.5;
  //EEPROM.begin();
  //EEPROM.put(0, f);

  Serial.println("## -- Fim Setup -- ##");
}

/*
* Calibração inicial.
*/
void Controle::ativaRedeDHCP(){
  #define ETH_CS  10
  #define SD_CS  4
  Serial.println("Ativando DHCP");
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Erro ao configurar via DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // print your local IP address:
  Serial.print("Endereço IP: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
  Serial.println();
}

/*
* Calibração inicial.
*/
void Controle::calibraInicio(){
  Serial.println("Inicia Calibracao");

  #ifdef aRefS1
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS2
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS3
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS4
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS5
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS6
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS7
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS8
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS9
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);

  #ifdef aRefS10
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" ativa...");
  #endif
  delay(300);
  Serial.print("Total de Celulas atvas = ");
  Serial.println(numero_celulas);
  delay(1000);

}
