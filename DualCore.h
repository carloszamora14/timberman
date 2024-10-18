#ifndef DUAL_CORE_H
#define DUAL_CORE_H

#include "Audio.h"
#include "BaseScreen.h"
#include "FS.h"
#include "MainMenu.h"
#include "SD.h"
#include "StateManager.h"
#include <LiquidCrystal_I2C.h>

// microSD Card Reader connections
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

// I2S Connections
#define I2S_DOUT 25
#define I2S_BCLK 26
#define I2S_LRC 27

#define NUCLEO_SECUNDARIO 0X00 // Núcleo secundario.
#define NUCLEO_PRINCIPAL 0X01  // Núcleo principalmente utilizado por el IDE en el loop original.

LiquidCrystal_I2C lcd(0x27, 16, 2);
StateManager stateManager;
Audio audio;
bool switched = false;
unsigned long startTime;
unsigned long introDuration = 26000;

/* INSTANCIAS FREE-RTOS */
TaskHandle_t Hilo1;
TaskHandle_t Hilo2;

class DualCoreESP32 {

  public:
    void ConfigCores(void);

  private:
    static void Loop1(void *pvParameters);
    static void Loop2(void *pvParameters);
};

void DualCoreESP32 ::ConfigCores(void) {

  xTaskCreatePinnedToCore(
      this->Loop1,     /* Función que controla la tarea. */
      "Game",          /* Etiqueta de la tarea.          */
      8192,            /* Tamaño en memoria RAM.         */
      NULL,            /* Parametros de la tarea.        */
      3,               /* Prioridad de la tarea.         */
      &Hilo1,          /* Seguimiento de la tarea.       */
      NUCLEO_PRINCIPAL /* Número de núcleo.              */
  );

  xTaskCreatePinnedToCore(
      this->Loop2,      /* Función que controla la tarea. */
      "Music",          /* Etiqueta de la tarea.          */
      4096,             /* Tamaño en memoria RAM.         */
      NULL,             /* Parametros de la tarea.        */
      2,                /* Prioridad de la tarea.         */
      &Hilo2,           /* Seguimiento de la tarea.       */
      NUCLEO_SECUNDARIO /* Número de núcleo.              */
  );
}

void DualCoreESP32 ::Loop1(void *pvParameters) {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(JOYSTICK_SW_PIN, INPUT_PULLUP);
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  BaseScreen *menu = new MainMenu(lcd, stateManager);
  stateManager.changeState(menu);

  for (;;) {
    stateManager.handleInput();
    stateManager.update();
  }
}

void DualCoreESP32 ::Loop2(void *pvParameters) {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  if (!SD.begin(SD_CS)) {
    Serial.println("Error accessing microSD card!");
    return;
  }

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(12);
  audio.connecttoFS(SD, "/soundtrack-loop.mp3");
  audio.setFileLoop(true);

  for (;;) {
    audio.loop();
    vTaskDelay(100);
  }
}

#endif
