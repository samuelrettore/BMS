#include "../Config.h"
#include "Controle.h"
#include "../objetos/BancoBateria.h"
//Placa de REde
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
//MQTT
#include <MqttClient.h>
//Json
#include <ArduinoJson.h>
//Date Time
#include <NTPClient.h>

//Global
MqttClient *mqtt = NULL;
EthernetClient netClient;
EthernetUDP udp;
//NTPClient
int16_t utc = 3;
//NTP CLient
NTPClient timeClient(udp, NTPSERVER);

// IPStack ipstack(client);
// MQTT::Client<IPStack, Countdown, 150, 1> client_mqtt = MQTT::Client<IPStack, Countdown, 150, 1>(ipstack);

//Construtor
Controle::Controle(){
}

// ============== Object to supply system functions ================================
class System: public MqttClient::System {
public:
  unsigned long millis() const {
    return ::millis();
  }
};


/**
* Metodo inicialização do modulo
*/
void Controle::inicializaModulo(BancoBateria* bateria){
  _bateria = bateria;
  Serial.begin(VELOCIDADE_SERIAL_ARDUINO);
  Serial.println("## -- Iniciou Setup -- ##");
  calibraInicio();
  ativaRedeDHCP();
  configuraMQTT();
  ativaMQTT();
  Serial.println("## -- Fim Setup -- ##");
}

/*
* Ativa rede / DHCP
*/
void Controle::ativaRedeDHCP(){
  //pinMode(SDCARD_CS, OUTPUT);
  //digitalWrite(SDCARD_CS, HIGH);
  Serial.println("Ativando DHCP");
  byte mac[] = {
    0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
  };
  // start the Ethernet connection:
  Serial.println("Inicializando Ethernet via DHCP:");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao configurar Ethernet usando DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet Shield Error. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cabo Desconectado.");
    }

  }
  // print your local IP address:
  Serial.print("Endereco IP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("Endereco gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("Endereco DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println();
  Serial.println("Ajusta NTP ");
  timeClient.begin();
  delay(300);
  while(!timeClient.update()) {
    Serial.println("Tentando Update Hora");
    timeClient.forceUpdate();
  }
  timeClient.forceUpdate();
  Serial.print("##Data e Hora = ");
  Serial.println(timeClient.getFormattedTime());
  delay(2000);

}

/**
* Configura MQTT
*/
void Controle::configuraMQTT(){
  // Setup MqttClient
  MqttClient::System *mqttSystem = new System;
  MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
  MqttClient::Network *mqttNetwork = new MqttClient::NetworkClientImpl<Client>(netClient, *mqttSystem);
  //// Make 128 bytes send buffer
  MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
  //// Make 128 bytes receive buffer
  MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<400>();
  //// Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<1>();
  //// Configure client options
  MqttClient::Options mqttOptions;
  ////// Set command timeout to 10 seconds
  mqttOptions.commandTimeoutMs = 10000;
  //// Make client object
  mqtt = new MqttClient (
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers
  );
}



// ============== Subscription callback ========================================
void Controle::processaMessage(MqttClient::MessageData& md) {
  Serial.println("Oeeeeeeeeeeeeeee");
  delay(2000);
  const MqttClient::Message& msg = md.message;
  char payload[msg.payloadLen + 1];
  memcpy(payload, msg.payload, msg.payloadLen);
  payload[msg.payloadLen] = '\0';
  Serial.println("Mensagem Chegou");
  Serial.print("Size = ");
  Serial.println(msg.payloadLen);
  delay(1000);
  Serial.print("Mensagem Subscribe = ");
  Serial.println(payload);

  // LOG_PRINTFLN(
  // 	"Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
  // 	msg.qos, msg.retained, msg.dup, msg.id, payload
  // );
}

/*
* Ativa MQTT
*/
void Controle::ativaMQTT(){
  if(!mqtt->isConnected()){
    netClient.stop();
    netClient.connect(BROKER_MQTT, BROKER_PORT);
    Serial.print("Conectando MQTT a ");
    Serial.println(BROKER_MQTT);
    MqttClient::ConnectResult connectResult;
    //Conect
    {
      MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
      //4=3.1.1
      options.MQTTVersion = 4;
      String id = (String)ID_MQTT+MQTT_KEY;
      options.clientID.cstring = (char*)id.c_str();
      //options.cleansession = false;
      options.keepAliveInterval = 15; // 15 seconds
      MqttClient::Error::type rc = mqtt->connect(options, connectResult);
      if (rc != MqttClient::Error::SUCCESS) {
        Serial.print("Connection error: ");
        Serial.println(rc);
        return;
      }
    }

    // //Subscribe
    // {
    //   String topico = (String)MQTT_KEY+MQTT_RESEND;
    //   //String topico = (String)MQTT_KEY+MQTT_BAT;
    //   Serial.println("Subscribe Topico = "+topico);
    //   MqttClient::Error::type rc = mqtt->subscribe(topico.c_str(), MqttClient::QOS0, processaMessage);
    //   Serial.print("RC = ");
    //   Serial.println(rc);
    //   delay(3000);
    //   if (rc != MqttClient::Error::SUCCESS) {
    //     Serial.print("Subscribe error:");
    //     Serial.println(rc);
    //     Serial.println("Drop connection");
    //     mqtt->disconnect();
    //     return;
    //   }
    // }
  }else{
    mqtt->yield(30000L);
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
    obj.setReferencia(RELACAO);

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
    float voltage = lePortaCalculoResistor(objj.getPortaInput());
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
Controla envio de dados da bateria ao MQTT via Json
*/
void Controle::MqttEnviaDados(){
  String topico = (String)MQTT_KEY+MQTT_BAT;
  long unix_time = timeClient.getEpochTime();
  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  root["cd"] = 0;
  root["qt"] = _bateria->getQuantidadeCelulas();
  root["pr"] = _bateria->getPercentual();
  root["vb"] = _bateria->getTensaoBanco();
  root["vn"] = _bateria->getTensaoMinima();
  root["vm"] = _bateria->getTensaoMaxima();
  root["tm"] = unix_time;

  String mensagem;
  //root.printTo(mensagem);
  serializeJson(root,mensagem);
  MqttSendMessage(topico,  mensagem);

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    //Busca Objeto
    ObjCelula obj_i = _bateria->getCelula(i);
    // //Coleta tensao eporta
    // //float tensao_i = obj_i.getLeituraTensao();
    StaticJsonDocument<200> doc;
    JsonObject root = doc.to<JsonObject>();
    root["cd"] = 1;
    root["nc"] = i+1;
    root["vc"] = obj_i.getLeituraTensao();
    root["pr"] = obj_i.getPercentual();
    root["cn"] = obj_i.getTensaoMinima();
    root["vm"] = obj_i.getTensaoMaxima();
    root["tm"] = unix_time;
    String mensagem;
    //root.printTo(mensagem);
    serializeJson(root, mensagem);
    MqttSendMessage(topico,  mensagem);
  }
}

/*
Envia Mensagem MQTT
*/
void Controle::MqttSendMessage(String topico, String mensagem){
  //Verifica se esta OK.
  if(mqtt->isConnected()){
    Serial.print("Mensagem  = ");
    Serial.println(mensagem);
    MqttClient::Message message;
    // Send and receive QoS 0 message
    char buf[100];
    strcpy(buf, mensagem.c_str());
    message.qos = MqttClient::QOS1;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadLen = strlen(buf);
    mqtt->publish(topico.c_str(), message);
    //mqtt->yield(30000L);
  }
}

/*
Verifica rede a cada 2 minutos.
tenta reconectar.
*/
void Controle::verificaRede(){
  Serial.println("Verifica Rede 2 miutos");
  // print your local IP address:
  Serial.print("Endereço IP: ");
  Serial.println(Ethernet.localIP());
  if(!mqtt->isConnected()){
    Serial.print("Renew IP");
    Ethernet.maintain();
    ativaMQTT();
  }
  Serial.print("Atualiza Data e Hora = " );
  timeClient.forceUpdate();
  Serial.println(timeClient.getFormattedTime());
  delay(300);
}

/*
verifica referencias de leitura do calculo
*/
void Controle::ciloProcessamento(){
  atualizaDadosLeitura();
  controlaSaidas();
}
