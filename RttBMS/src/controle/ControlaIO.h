#ifndef ControlaIO_h
#define ControlaIO_h

#include <Arduino.h>


class ControlaIO {
public:
  float lePorta(int portaAnalogica);
  float eepromLeReferencia(int endereco);
  float eepromCalculaReferenciaGrava(int endereco, float referencia_inicial);
  void zeraEEPROM();
  void gravaEEPROM(int endereco, float valor);
private:
  //BancoBateria* _bateria;
  //int vbat_tensao = 0;
};
#endif
