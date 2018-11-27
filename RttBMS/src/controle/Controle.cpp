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
  Serial.begin(velocidade_serial);
  Serial.println("## -- Iniciou Setup -- ##");
  calibraInicio();
  ativaRedeDHCP();
  verificaReferenciaCalculo();
  Serial.println("## -- Fim Setup -- ##");
}

/*
verifica referencias de leitura do calculo
*/
void Controle::verificaReferenciaCalculo(){
  //Inicializa banco de Bateria -> constroi celulas.
  _bateria->inicializaBanco();
  int endereco_memoria = 0;
  //Verifica Versao
  float ver = eepromLeReferencia(0);
  Serial.print("Versao do software encontrada = ");
  Serial.print(ver);
  Serial.print(" versao config = ");
  Serial.println(versao);
  if(ver != versao){
    zeraEEPROM();
    gravaEEPROM(0, versao);
  }
  Serial.println();
  Serial.println();
  delay(2000);

  //Inicializa celulas com valores
  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    //Verifica se tem referencia registrada na EEprom
    int numero_cel = i+1;
    endereco_memoria+= sizeof(float);
    float ref = eepromLeReferencia(endereco_memoria);
    Serial.print(" REFERENCIA ");
    Serial.print(numero_cel);
    Serial.print(" EEPROM == ");
    Serial.println(ref);
    float comp = 0;
    if(ref<1){
      ref = eepromCalculaReferenciaGrava(endereco_memoria,_referencias_iniciais[i]);
    }
    //Cria Objeto.
    ObjCelula obj;
    obj.setNumeroCelula(numero_cel);
    obj.setReferencia(ref);
    obj.setLeituraTensao(0.00);
    _bateria->setCelula(obj, i);
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
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS1;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");

  #endif
  delay(300);

  #ifdef aRefS2
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS2;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS3
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS3;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS4
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS4;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS5
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS5;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS6
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS6;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS7
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS7;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS8
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS8;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS9
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS9;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);

  #ifdef aRefS10
  _referencias_iniciais[_bateria->getQuantidadeCelulas()] = aRefS10;
  _bateria->setQuantidadeCelulas(_bateria->getQuantidadeCelulas()+1);
  Serial.print("Celula ");
  Serial.print(_bateria->getQuantidadeCelulas());
  Serial.println(" configurada...");
  #endif
  delay(300);


  Serial.print("Total de Celulas configuradas = ");
  Serial.println(_bateria->getQuantidadeCelulas());
  delay(300);
  Serial.println("Configura portas de entrada e Saida");
  int porta_digital = 0;

  //Ativa Portas de ENtrada e Saida
  for(int i =1; i<=_bateria->getQuantidadeCelulas();i++){
    pinMode(i, INPUT);
    Serial.print("Setando porta analogica A");
    Serial.print(i);
    Serial.println(" entrada.");

    porta_digital = i+30;
    Serial.print("Setando porta Digital ");
    Serial.print(porta_digital);
    Serial.println(" saida nivel baixo(low).");
    pinMode(porta_digital, OUTPUT);
    digitalWrite(porta_digital, LOW);
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
    float ref  = objj.getReferencia();
    float x = lePorta(f);
    x = x/ref;
    Serial.print("Leitura = ");
    Serial.println(x);
  }
  delay(1000);
}
