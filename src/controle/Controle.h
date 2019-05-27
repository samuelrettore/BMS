#ifndef Controle_h
#define Controle_h

#include <Arduino.h>
//#include "../objetos/ObjCelula.h"
#include "../objetos/BancoBateria.h"
#include "../Config.h"
#include "ControlaIO.h"
#include <MQTT.h>

class Controle : public ControlaIO {
public:
  Controle();
  void inicializaModulo(BancoBateria* bateria);
  void calibraInicio();
  void ativaRedeDHCP();
  void ativaMQTT();
  void ciloProcessamento();
  void MqttEnviaDados();
  void verificaRede();
  //static void processaMessage(MqttClient::MessageData& md);
  static void processaMessage(MQTTClient *client, char topic[], char payload[], int payload_length);

private:
  void atualizaDadosLeitura();
  void controlaSaidas();
  //static void MqttCallback();
  static void MqttSendMessage(String mensagem);
  BancoBateria* _bateria;
  int vbat_tensao = 0;
};
#endif
