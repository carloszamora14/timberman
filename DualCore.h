#ifndef DualCore_h
#define DualCore_h

#include "Audio.h"
#include "FS.h"
#include "SD.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Game.h"

// microSD Card Reader connections
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18

// I2S Connections
#define I2S_DOUT 25
#define I2S_BCLK 26
#define I2S_LRC 27

#define JOYSTICK_X_PIN 2
#define JOYSTICK_Y_PIN 4
#define JOYSTICK_SW_PIN 15
#define CHIP_SELECT_PIN 5

#define JOYSTICK_THRESHOLD_LOW 1000
#define JOYSTICK_THRESHOLD_HIGH 2500

Audio audio;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Game game(lcd);

bool switched = false, started = false;
int xMovement = 0;
unsigned long lastUpdateTime, currentTime;

#define NUCLEO_SECUNDARIO 0X00 // Núcleo secundario.
#define NUCLEO_PRINCIPAL 0X01  // Núcleo principalmente utilizado por el IDE en el loop original.

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
  lcd.init();
  lcd.backlight();

  game.setup();
  game.initializeChars();

  for (;;) {
    game.display();

    if (analogRead(JOYSTICK_X_PIN) < JOYSTICK_THRESHOLD_LOW) {
      xMovement = -1;
    } else if (analogRead(JOYSTICK_X_PIN) > JOYSTICK_THRESHOLD_HIGH) {
      xMovement = 1;
    } else {
      xMovement = 0;
    }

    currentTime = millis();

    if (!game.gameOver) {
      if (xMovement != 0) {
        if (game.paused) {
          lastUpdateTime = currentTime;
          game.paused = false;
        }

        game.updateFrame(xMovement);
      }

      if (!game.paused) {
        float delta = (currentTime - lastUpdateTime) / 1000.0f;
        game.updateTimer(-delta);
        lastUpdateTime = currentTime;
      }
    }

    vTaskDelay(10);
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

  audio.setVolume(10);

  audio.connecttoFS(SD, "/soundtrack-start.mp3");

  for (;;) {
    audio.loop();

    uint32_t audioPos = audio.getFilePos();

    if (audioPos == 0 && !switched && started) {
      Serial.println("looping");
      switched = true;
      audio.connecttoFS(SD, "/soundtrack-loop.mp3");
      audio.setFileLoop(true);
    }

    if (!started) {
      started = true;
    }

    vTaskDelay(100);
  }
}

#endif
