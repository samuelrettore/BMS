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
  verificaReferenciaCalculo();
  Serial.println("## -- Fim Setup -- ##");
}

//verifica referencias de leitura do calculo
void Controle::verificaReferenciaCalculo(){
  //Inicializa Numero de celulas
  ObjCelula celulas[numero_celulas];
  for(int i=0; i<numero_celulas;i++){
      //Verifica se tem referencia registrada na EEprom
      float x;
      EEPROM.get(i, x);

      //Cria Objeto.
      ObjCelula obj;
      obj.setNumeroCelula(i+1);
      obj.setReferencia(x);
      obj.setLeituraTensao(3.98);
      celulas[i] = obj;
  }

  for(int i=0; i<numero_celulas;i++){
    ObjCelula objj = celulas[i];
    Serial.print("numero Celula = ");
    Serial.println(objj.getNumeroCelula());
    Serial.print("referencia = ");
    Serial.println(objj.getReferencia());
    Serial.print("Tensao = ");
    Serial.println(objj.getLeituraTensao());
    Serial.println();
  }

}


/*
* Ativa rede / DHCP
*/
void Controle::ativaRedeDHCP(){
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);
  Serial.println("Ativando DHCP");
  byte mac[] = {
    0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
  };

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Erro ao configurar via DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // print your local IP address:
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Endereço gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("Endereço DNS: ");
  Serial.println(Ethernet.dnsServerIP());
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
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS2
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS3
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS4
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS5
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS6
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS7
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS8
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS9
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS10
  numero_celulas++;
  Serial.print("Celula ");
  Serial.print(numero_celulas);
  Serial.println(" configurada...");
  #endif
  delay(300);
  Serial.print("Total de Celulas configuradas = ");
  Serial.println(numero_celulas);
  delay(1000);
}
