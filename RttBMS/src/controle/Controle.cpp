#include "../Config.h"
#include "Controle.h"
#include "../objetos/BancoBateria.h"
//Placa de REde
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
//MQTT
#include <MqttClient.h>
#include <ArduinoJson.h>

//Global
EthernetClient network;
MqttClient *mqtt = NULL;
//PubSubClient mqttClient;

//Dependencia MQTT separar objeto
// ============== Object to supply system functions ================================
class System: public MqttClient::System {
public:
  unsigned long millis() const {
    return ::millis();
  }
};


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
  //ativaRedeDHCP();
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
  delay(300);
}

/*
* Ativa rede / DHCP
*/
void Controle::ativaMQTT(){
  Serial.println("Ativando MQTT");
  // Setup MqttClient
  MqttClient::System *mqttSystem = new System;
  MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
  MqttClient::Network * mqttNetwork = new MqttClient::NetworkClientImpl<Client>(network, *mqttSystem);
  //// Make 128 bytes send buffer
  MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
  //// Make 128 bytes receive buffer
  MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
  //// Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
  //// Configure client options
  MqttClient::Options mqttOptions;
  ////// Set command timeout to 10 seconds
  mqttOptions.commandTimeoutMs = 10000;
  //// Make client object
  mqtt = new MqttClient (
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers
  );

  // //OLD
  // mqttClient.setClient(client);
  // mqttClient.setServer(BROKER_MQTT, BROKER_PORT);
  // //mqttClient.setCallback(MqttCallback);
  // mqttClient.connect(ID_MQTT);
  delay(300);
}


void Controle::MqttSendMessage(String topico, String mensagem){
  //Verifica se esta conectado.
  if (!mqtt->isConnected()) {
    // Close connection if exists
    network.stop();
    // Re-establish TCP connection with MQTT broker
    network.connect(BROKER_MQTT, BROKER_PORT);
    // Start new MQTT connection
    Serial.print("Conectando a ");
    Serial.println(BROKER_MQTT);
    MqttClient::ConnectResult connectResult; // Connect
    {
      MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
      options.MQTTVersion = 4;
      options.clientID.cstring = (char*)ID_MQTT;
      options.cleansession = true;
      options.keepAliveInterval = 15; // 15 seconds
      options.will.qos = MqttClient::QOS1;
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        Serial.print("Erro de conexao : ");
        Serial.println(rc);
        return;
      }
    }
    {
      // Add subscribe here if need
      //Not Subscribe
    }
  } else {
    {
      const char* buf = mensagem.c_str();
      MqttClient::Message message;
      message.qos = MqttClient::QOS1;
      message.retained = false;
      message.dup = false;
      message.payload = (void*) buf;
      message.payloadLen = strlen(buf) + 1;
      MqttClient::Error::type rc = mqtt->publish(topico.c_str(), message);
      if (rc != MqttClient::Error::SUCCESS) {
        Serial.print("Erro publicacao : ");
        Serial.println(rc);
        delay(1000);
      }
    }
    // Idle for 30 seconds
    mqtt->yield(30000L);
  }
}

static void Controle::MqttCallback(){

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
    Serial.println(" saida nivel baixo(low).");
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
    float tread = lePortaDivisor(objj.getPortaInput());
    objj.setLeituraTensao(tread);
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
verifica referencias de leitura do calculo
*/
void Controle::ciloProcessamento(){
  atualizaDadosLeitura();
  controlaSaidas();
}
