#include "../Config.h"
#include "Controle.h"

//Le Dados porta Analogica analogReader
float ControlaIO::lePorta(int portaAnalogica) {
  float total=0;
  for (int i=0; i<AMOSTRAS; i++) {
    total += 1.0 * analogRead(portaAnalogica);
  }
  return total / (float)AMOSTRAS;
}
