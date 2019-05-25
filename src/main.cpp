#include <Arduino.h>
#include "controle/Controle.h"
#include "objetos/BancoBateria.h"


#define MQTTCLIENT_QOS2 1
//Objeto Controle
Controle ctrl;
BancoBateria* bateria = new BancoBateria();
unsigned long time_cliclo = 0;
unsigned long time_mqtt = 0;
unsigned long time_network = 0;

void setup() {
  ctrl.inicializaModulo(bateria);
}

void loop() {
  //Serial.println("Loop");
  //Loop 1s -> 500ms
  if(millis() > time_cliclo + 500){
    time_cliclo = millis();
    ctrl.ciloProcessamento();
  }
  //Atualiza dados no MQTT -> 10 segundos = 10000
  if(millis() > time_mqtt + 5000){
    time_mqtt = millis();
    ctrl.MqttEnviaDados();
  }
  //Verifica Rede conexoes -> 2 minutos = 120000
  if(millis()>time_network+120000){
    time_network = millis();
    ctrl.verificaRede();
  }
}
