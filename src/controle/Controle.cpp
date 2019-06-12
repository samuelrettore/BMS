#include "../Config.h"
#include "Controle.h"
#include "../objetos/BancoBateria.h"
//Placa de REde
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Arduino.h>
//MQTT
#include <MQTT.h>

//Json
#include <ArduinoJson.h>
//Date Time
#include <NTPClient.h>

//Global
EthernetClient netClient;
EthernetUDP Udp;
//NTPClient
int16_t utc = 3;
//NTP CLient
//NTPClient timeClient(Udp);
//3600000 = 1 hora
//7200000 =  2 horas
//10800000 = 3 horas
NTPClient timeClient(Udp,NTPSERVER_1,0,3600000); //intervalor 1 hora

//--MQTT
MQTTClient mqtt(350);
//JsonDOcumens
//DynamicJsonDocument doc(400);
//DynamicJsonDocument doc2(400);
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
  ativaRede();
  ativaMQTT();
  pinMode(LED_PLACA,OUTPUT);
  Serial.println("## -- Fim Setup -- ##");
}

/*
* Ativa rede / DHCP
*/
void Controle::ativaRede(){
  Serial.println("Ativando Rede");
  byte mac[] = {
    0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
  };

  //Testa se deve usar DHCP
  if(DHCP){
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
  }else{
    //Endereco IP
    IPAddress ip(192, 168, 0, 99);
    // DNS
    IPAddress dnServer(192, 168, 0, 1);
    // Roteador
    IPAddress gateway(192, 168, 0, 1);
    // Subrede
    IPAddress subnet(255, 255, 255, 0);
    //Cria Conexao
    Ethernet.begin(mac, ip, dnServer, gateway, subnet);
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
  Serial.print("##Data e Hora = ");
  Serial.println(timeClient.getFormattedTime());
  delay(2000);
}

/*
* Ativa MQTT
*/
void Controle::ativaMQTT(){
  if(!mqtt.connected()){
    Serial.print("Conectando MQTT a ");
    Serial.println(BROKER_MQTT);
    mqtt.begin(BROKER_MQTT, BROKER_PORT ,netClient);
    mqtt.setOptions(10, false, 15000);
    mqtt.onMessageAdvanced(processaMessage);
    String id_mqtt = (String)ID_MQTT+MQTT_KEY;
    if(mqtt.connect(id_mqtt.c_str())) {
      Serial.println("Conectado");
      #ifdef MQTT_SONOFF1
      String sonoff1 = (String)MQTT_KEY+MQTT_SONOFF1;
      Serial.print("Subscribe em ");
      Serial.println(sonoff1);
      //Subscribe sensor 1 - qos 0
      mqtt.subscribe(sonoff1, 0);
      #endif

      #ifdef MQTT_SONOFF2
      String sonoff2 = (String)MQTT_KEY+MQTT_SONOFF2;
      Serial.print("Subscribe em ");
      Serial.println(sonoff2);
      //Subscribe sensor 2 - qos 0
      mqtt.subscribe(sonoff2, 0);
      #endif
    }
    delay(3000);
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

  //Inicializa celulas com valores
  Serial.println("Configura portas de entrada e Saida e cria Objetos do banco.");
  int pinos_entrada[] = INPUT_PORT;
  int pinos_saida[] = OUTPUT_PORT;

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    // //Verifica se tem referencia registrada na EEprom
    int numero_cel = i+1;
    //Cria Objeto.
    ObjCelula obj;
    obj.setNumeroCelula(numero_cel);
    obj.setLeituraTensao(0.00);
    obj.setPortaInput(pinos_entrada[i]);
    obj.setPortaControle(pinos_saida[i]);
    //Porta digital
    Serial.print("Setando porta analogica GPI(O)");
    Serial.print(obj.getPortaInput());
    Serial.print(" entrada --> porta de controle ");
    //Ativa Input
    pinMode(pinos_entrada[i], INPUT);
    Serial.print(obj.getPortaControle());
    Serial.println(" saida nivel baixo(LOW).");
    //Ativa em modo baixo
    pinMode(pinos_saida[i], OUTPUT);
    digitalWrite(pinos_saida[i], LOW);

    _bateria->setCelula(obj, i);
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

  long unix_time = timeClient.getEpochTime();
  StaticJsonDocument<400> doc;
  JsonObject root = doc.to<JsonObject>();
  //Bateria
  root["codigo"] = 0;
  root["bat_qtcells"] = _bateria->getQuantidadeCelulas();
  root["bat_per"] = _bateria->getPercentual();
  root["bat_vbat"] = _bateria->getTensaoBanco();
  root["bat_vmin"] = _bateria->getTensaoMinima();
  root["bat_vmax"] = _bateria->getTensaoMaxima();
  root["time"] = unix_time;

  String mensagem;
  //root.printTo(mensagem);
  serializeJson(root,mensagem);
  MqttSendMessage(mensagem);

  for(int i=0; i<_bateria->getQuantidadeCelulas();i++){
    //Busca Objeto
    ObjCelula obj_i = _bateria->getCelula(i);
    // //Coleta tensao eporta
    JsonObject root = doc.to<JsonObject>();
    //Celulas
    root["codigo"] = 1;
    root["cel_num"] = i+1;
    root["cel_vcel"] = obj_i.getLeituraTensao();
    root["cel_per"] = obj_i.getPercentual();
    root["cel_vmin"] = obj_i.getTensaoMinima();
    root["cel_vmax"] = obj_i.getTensaoMaxima();
    root["time"] = unix_time;
    String mensagem;
    //root.printTo(mensagem);
    serializeJson(root, mensagem);
    MqttSendMessage(mensagem);
  }
}

void Controle::processaMessage(MQTTClient *client, char topic[], char payload[], int payload_length) {
  Serial.println("Mensagem Recebida : " + String(topic) + " - " + String(payload)+" - "+payload_length);
  String topic_comp =  (String)MQTT_KEY+MQTT_SONOFF1;
  String topic_comp2 =  (String)MQTT_KEY+MQTT_SONOFF2;
  if(String(topic) == topic_comp ||
  String(topic) == topic_comp2){
    //DeserializationError err  = deserializeJson(doc,payload);
    DynamicJsonDocument doc(400);
    auto error = deserializeJson(doc,payload);
    if(!error){
      bool codigo = false;
      Serial.println("Deserializacao OK");
      DynamicJsonDocument doc2(400);
      JsonObject root = doc2.to<JsonObject>();
      //Energia concessionaria
      //{"Time":"2019-05-15T16:30:39","ENERGY":{"TotalStartTime":"2019-05-01T19:28:55","Total":8.191,"Yesterday":0.828,"To
      //day":0.548,"Period":0,"Power":29,"ApparentPower":52,"ReactivePower":44,"Factor":0.56,"Voltage":219,"Current":0.239}}
      if(String(topic) == topic_comp){
        root["codigo"] = 2;
        codigo = true;
      }else if (String(topic) == topic_comp2){
        root["codigo"] = 3;
        codigo = true;
      }
      root["Time"] = doc["Time"];
      root["TotalStartTime"] = doc["ENERGY"]["TotalStartTime"];
      root["Total"] = doc["ENERGY"]["Total"];
      root["Today"] = doc["ENERGY"]["Today"];
      root["Period"] = doc["ENERGY"]["Period"];
      root["Power"] = doc["ENERGY"]["Power"];
      root["ApparentPower"] = doc["ENERGY"]["ApparentPower"];
      root["ReactivePower"] = doc["ENERGY"]["ReactivePower"];
      root["Factor"] = doc["ENERGY"]["Factor"];
      root["Voltage"] = doc["ENERGY"]["Voltage"];
      root["Current"] = doc["ENERGY"]["Current"];
      root["time"] =  timeClient.getEpochTime();
      String mensagem;
      serializeJson(root, mensagem);
      if(codigo){
        MqttSendMessage(mensagem);
      }else{
        Serial.print("Erro ao determinar o Sensor");
        delay(3000);
      }
    }else{
      Serial.print("Deserializacao ERROR");
    }
  }
}

/*
Envia Mensagem MQTT
*/
void Controle::MqttSendMessage(String mensagem){
  //Verifica se esta OK.
  if(mqtt.connected()){
    digitalWrite(LED_PLACA,HIGH);
    Serial.print("Mensagem  = ");
    Serial.print(mensagem);
    Serial.print(", tamanho ----> ");
    unsigned int tamanho = mensagem.length();
    Serial.println(tamanho);
    // if(tamanho > 80){
    String topico = (String)MQTT_KEY+MQTT_DATA;
    if(mqtt.publish(topico, mensagem, false, 1)){
      Serial.println("Mensagem MQTT enviada");
    }else{
      Serial.println("--------->Mensagem MQTT NAO enviada");
    }
    // }else{
    //   Serial.println("--------->Mensagem MQTT NAO enviada, muito pequena");
    // }
    digitalWrite(LED_PLACA,LOW);
  }
}

/*
Verifica rede a cada 2 minutos.
tenta reconectar.
*/
void Controle::verificaRede(){
  Serial.println("Verifica Rede 2 miutos");
  //Caso DHCP renova IP
  if(DHCP){
    Serial.println("Renew IP");
    Ethernet.maintain();
  }
  // print your local IP address:
  Serial.print("Endereco IP: ");
  Serial.println(Ethernet.localIP());
  if(!mqtt.connected()){
    mqtt.disconnect();
    ativaMQTT();
  }
}

/*
verifica referencias de leitura do calculo
*/
void Controle::ciloProcessamento(){
  atualizaDadosLeitura();
  controlaSaidas();
}

/*
verifica referencias de leitura do calculo
*/
void Controle::forcaNTP(){
  if(mqtt.connected()){
    Serial.print("Forca atualizacao Data e Hora = " );
    timeClient.forceUpdate();
    Serial.println(timeClient.getFormattedTime());
  }
}
