#include "../Config.h"
#include "Controle.h"
#include "../objetos/BancoBateria.h"
//Placa de REde
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
EthernetClient client;

//Construtor
Controle::Controle(){
}

/**
* Metodo inicialização do modulo
*/
void Controle::inicializaModulo(BancoBateria* bateria){
  _bateria = bateria;
  Serial.begin(VELOCIDADE_SERIAL_ARDUINO);
  Serial.println("## -- Iniciou Setup -- ##");
  calibraInicio();
  ativaRedeDHCP();
  Serial.println("## -- Fim Setup -- ##");
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
  _bateria->setQuantidadeCelulas(QUANTIDADE_CELULAS);
  for(int i = 1; i<=_bateria->getQuantidadeCelulas();i++){
    Serial.print("Celula ");
    Serial.print(i);
    Serial.println(" configurada...");
    delay(300);
  }

  Serial.print("Total de Celulas configuradas = ");
  Serial.println(_bateria->getQuantidadeCelulas());
  delay(300);

  //Inicializa banco de Bateria -> constroi celulas.
  _bateria->inicializaBanco();
  delay(2000);

  //Seta Primeira porta como A1
  int porta_i = A1;

  //Porta Digital inicia 31
  int porta_digital = 31;

  //Inicializa celulas com valores
  Serial.println("Configura portas de entrada e Saida e cria Objetos do banco.");

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    // //Verifica se tem referencia registrada na EEprom
    int numero_cel = i+1;
    //Cria Objeto.
    ObjCelula obj;
    obj.setNumeroCelula(numero_cel);
    obj.setLeituraTensao(0.00);
    obj.setPortaInput(porta_i);
    obj.setPortaControle(porta_digital);

    //Porta digital
    Serial.print("Setando porta analogica ");
    Serial.print(porta_i);
    Serial.println(" entrada.");
    //Ativa Input
    pinMode(porta_i, INPUT);

    Serial.print("Setando porta Digital ");
    Serial.print(porta_digital);
    Serial.println(" saida nivel baixo(low).");
    //Ativa em modo baixo
    pinMode(porta_digital, OUTPUT);
    digitalWrite(porta_digital, LOW);

    _bateria->setCelula(obj, i);
    porta_i++;
    porta_digital++;
    delay(500);
  }
  delay(1000);
}

/*
verifica referencias de leitura do calculo
*/
void Controle::ciloProcessamento(){
  _bateria->imprimeDados();

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    ObjCelula objj = _bateria->getCelula(i);
    int f = objj.getNumeroCelula();
    float x = lePorta(i);
    Serial.print("Leitura 1 porta (");
    Serial.print(f);
    Serial.print(") = ");
    Serial.println(x);

    x = lePorta2(i);
    Serial.print("Leitura 2 porta (");
    Serial.print(f);
    Serial.print(") = ");
    Serial.println(x);
  }
  delay(1000);
}
