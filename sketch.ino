#include "Game.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define BUZZER_PIN 17
#define JOYSTICK_X_PIN 2
#define JOYSTICK_Y_PIN 4
#define JOYSTICK_SW_PIN 15
#define CHIP_SELECT_PIN 5

#define JOYSTICK_THRESHOLD_LOW 1000
#define JOYSTICK_THRESHOLD_HIGH 2500

int xMovement = 0;
unsigned long lastUpdateTime, currentTime;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Game game(lcd);

void setup(void) {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  game.setup();
  game.initializeChars();
}

void loop(void) {
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

      tone(BUZZER_PIN, 1000, 100);
      game.updateFrame(xMovement);
    }

    if (!game.paused) {
      float delta = (currentTime - lastUpdateTime) / 1000.0f;
      game.updateTimer(-delta);
      lastUpdateTime = currentTime;

      if (game.gameOver) {
        tone(BUZZER_PIN, 500, 500);
      }
    }
  }

  delay(10);
}
