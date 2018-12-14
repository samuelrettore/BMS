#ifndef ControlaIO_h
#define ControlaIO_h

#include <Arduino.h>


class ControlaIO {
public:
  float lePorta(uint8_t portaAnalogica);
  float lePorta_calculoresitor(uint8_t portaAnalogica);
  float lePortaDivisor(uint8_t portaAnalogica);
private:

};
#endif
