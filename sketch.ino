#define BAUDRATE 115200

#include "DualCore.h"
DualCoreESP32 DCESP32;

void setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(BAUDRATE);
  DCESP32.ConfigCores();

  Serial.println(F("Se han configurado correctamente los dos nucleos "));
}

void loop(void) {}
