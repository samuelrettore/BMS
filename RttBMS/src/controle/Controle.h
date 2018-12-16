#ifndef Controle_h
#define Controle_h

#include <Arduino.h>
//#include "../objetos/ObjCelula.h"
#include "../objetos/BancoBateria.h"
#include "../Config.h"
#include "ControlaIO.h"

class Controle : public ControlaIO {
public:
  Controle();
  void inicializaModulo(BancoBateria* bateria);
  void calibraInicio();
  void ativaRedeDHCP();
  void ativaMQTT();
  void ciloProcessamento();
  void MqttEnviaDados();
private:
  void atualizaDadosLeitura();
  void controlaSaidas();
  static void MqttCallback();
  void MqttSendMessage(String topico, String mensagem);
  BancoBateria* _bateria;
  int vbat_tensao = 0;
  int i = 0;
};
#endif
