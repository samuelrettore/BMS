#ifndef ControlaIO_h
#define ControlaIO_h

#include <Arduino.h>


class ControlaIO {
public:
  float lePorta(uint8_t portaAnalogica);
  float lePorta2(uint8_t portaAnalogica);
  float lePorta3(uint8_t portaAnalogica);
  float lePortaDivisor(uint8_t portaAnalogica);
private:

};
#endif
