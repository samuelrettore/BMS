#include "../Config.h"
#include "Controle.h"
#include "../objetos/BancoBateria.h"
//Placa de REde
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
//MQTT
#include <IPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>
//Json
#include <ArduinoJson.h>

//Global
EthernetClient client;
IPStack ipstack(client);
MQTT::Client<IPStack, Countdown, 150, 1> client_mqtt = MQTT::Client<IPStack, Countdown, 150, 1>(ipstack);


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
  ativaMQTT();
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
  //Timeout 30s, resposta 30s
  if (Ethernet.begin(mac,30000, 30000) == 0) {
    Serial.println("Erro ao configurar via DHCP");
    _status_rede = false;
  }
  // print your local IP address:
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Endereço gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("Endereço DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println();
  delay(300);
}

/*
* Ativa rede / DHCP
*/
void Controle::ativaMQTT(){
  if(_status_rede){
    int rc = ipstack.connect(BROKER_MQTT, BROKER_PORT);
    if (rc != 1){
      _status_mqtt = false;
      Serial.print("Erro conexao rc = ");
      Serial.println(rc);
    }
    if(_status_mqtt){
      Serial.print("Conectando MQTT a ");
      Serial.println(BROKER_MQTT);
      MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
      data.MQTTVersion = 4;
      data.clientID.cstring = (char*)ID_MQTT;
      //data.keepAliveInterval = 3;
      rc = client_mqtt.connect(data);
      if (rc != 0){
        _status_mqtt = false;
        Serial.print("erro MQTT rc = ");
        Serial.println(rc);
      }
      Serial.print("MQTT Conectador a ");
      Serial.println(BROKER_MQTT);
      _status_mqtt = true;
      delay(300);
    }
  }
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
    delay(100);
  }
  Serial.print("Total de Celulas configuradas = ");
  Serial.println(_bateria->getQuantidadeCelulas());
  delay(100);
  //Inicializa banco de Bateria -> constroi celulas.
  _bateria->inicializaBanco();
  delay(1000);
  //Seta Primeira porta como A1
  int porta_i = A8;
  int numero_porta  = 8;
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
    Serial.print("Setando porta analogica A");
    Serial.print(numero_porta);
    Serial.print(" entrada --> porta de controle ");
    //Ativa Input
    pinMode(porta_i, INPUT);

    Serial.print(porta_digital);
    Serial.println(" saida nivel baixo(LOW).");
    //Ativa em modo baixo
    pinMode(porta_digital, OUTPUT);
    digitalWrite(porta_digital, LOW);

    _bateria->setCelula(obj, i);
    porta_i++;
    porta_digital+=2;
    numero_porta++;
    delay(500);
  }
  delay(1000);
}

/*
* Faz leitura dados de input e atualiza dados da bateria
*e atualiza celulas
*/
void Controle::atualizaDadosLeitura(){
  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    ObjCelula objj = _bateria->getCelula(i);
    float voltage = lePortaDivisor(objj.getPortaInput());
    float voltage2 = lePortaCalculoResistor(objj.getPortaInput());
    //Debug
    Serial.print("Valor leitura Divisor normal =");
    Serial.print(voltage,5);
    Serial.print(", Valor leitura Divisor fator =");
    Serial.println(voltage2,5);
    delay(2000);
    objj.setLeituraTensao(voltage);
    //Atualiza Celula.
    _bateria->setCelula(objj,i);
  }
  //Imprime dados.
  _bateria->imprimeDados();
}

/*
Faz ontrole dos Mosfets para ligar e desligar
*/
void Controle::controlaSaidas(){
  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    //Busca Objeto
    ObjCelula obj_i = _bateria->getCelula(i);
    //Coleta tensao eporta
    float tensao_i = obj_i.getLeituraTensao();
    int porta_i = obj_i.getPortaControle();
    bool ligaResistencia = false;
    for(int x=0; x<_bateria->getQuantidadeCelulas();x++){
      //Busca Opbjeto
      ObjCelula obj_x = _bateria->getCelula(x);
      if(obj_i.getNumeroCelula() != obj_x.getNumeroCelula()){
        //Coleta dados comparacao
        float tensao_x = obj_x.getLeituraTensao();
        Serial.print("####### Compara celula ");
        Serial.print(i+1);
        Serial.print(" tensao = ");
        Serial.print(tensao_i,5);
        Serial.print(", com celula ");
        Serial.print(x+1);
        Serial.print(" tensao = ");
        tensao_x += BMS;
        Serial.print(tensao_x,5);
        Serial.print(", porta estado = ");
        Serial.println(digitalRead(porta_i));
        if( (tensao_i > tensao_x) && !ligaResistencia){
          ligaResistencia = true;
        }
      }
    }
    if(ligaResistencia && digitalRead(porta_i) == LOW){
      digitalWrite(porta_i, HIGH);
      Serial.print("------------------------- liga porta ");
      Serial.print(porta_i);
      Serial.println("-------------------------");
    }else if(digitalRead(porta_i) == HIGH){
      digitalWrite(porta_i, LOW);
    }
  }
}

/*
Controla envio de dados ao MQTT via Json
*/
void Controle::MqttEnviaDados(){
  StaticJsonBuffer<200> doc;
  JsonObject& root = doc.createObject();
  root["codigo"] = 0;
  root["qtcel"] = _bateria->getQuantidadeCelulas();
  root["seq"] = sequencial++;
  String mensagem;
  root.printTo(mensagem);
  MqttSendMessage(MQTT_TOPIC,  mensagem);

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    //Busca Objeto
    ObjCelula obj_i = _bateria->getCelula(i);
    // //Coleta tensao eporta
    // //float tensao_i = obj_i.getLeituraTensao();
    StaticJsonBuffer<200> doc;
    JsonObject& root = doc.createObject();
    root["codigo"] = 1;
    root["n_bat"] = i+1;
    root["v_bat"] = obj_i.getLeituraTensao();
    root["p_bat"] = obj_i.getPercentual();
    String mensagem;
    root.printTo(mensagem);
    MqttSendMessage(MQTT_TOPIC,  mensagem);
  }
}

/*
Envia Mensagem MQTT
*/
void Controle::MqttSendMessage(String topico, String mensagem){
  //Verifica se esta OK.
  if(_status_mqtt){
    Serial.print("Mensagem  = ");
    Serial.print(mensagem);
    MQTT::Message message;
    // Send and receive QoS 0 message
    char buf[100];
    strcpy(buf, mensagem.c_str());
    message.qos = MQTT::QOS1;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf);
    int rc = client_mqtt.publish(topico.c_str(), message);
    Serial.print(", retorno rx = ");
    Serial.println(rc);
    if(rc != 0){
      _status_mqtt = false;
      Serial.println("MQTT Qos1 erro publicacao.");
    }
  }
}

/*
Verifica rede a cada 2 minutos.
tenta reconectar.
*/
void Controle::verificaRede(){
  if(!_status_rede || !_status_mqtt){
    Serial.println("Verifica Rede 2 miutos");
    client.stop();
    Ethernet.maintain();
    ativaRedeDHCP();
    if(_status_rede){
      if(client_mqtt.isConnected()){
        client_mqtt.disconnect();
      }
      ativaMQTT();
    }
  }
}


/*
verifica referencias de leitura do calculo
*/
void Controle::ciloProcessamento(){
  atualizaDadosLeitura();
  controlaSaidas();
}
