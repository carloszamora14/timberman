#define BAUDRATE 115200

#include "DualCore.h"
DualCore DCESP32;

void setup(void) {
  Serial.begin(BAUDRATE);
  DCESP32.ConfigCores();

  Serial.println(F("Se han configurado correctamente los dos nucleos "));
}

void loop(void) {}
