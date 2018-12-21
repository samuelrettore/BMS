#ifndef ControlaIO_h
#define ControlaIO_h

#include <Arduino.h>


class ControlaIO {
public:
  float lePortaShield25v(uint8_t portaAnalogica);
  float lePortaCalculoResistor(uint8_t portaAnalogica);
  float lePortaDivisor(uint8_t portaAnalogica);
private:

};
#endif
