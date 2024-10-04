#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "Game.h"
#include <LiquidCrystal_I2C.h>

int xMovement = 0;
unsigned long lastUpdateTime = 0, currentTime;

const int pinX = 2;
const int pinY = 4;
const int pinSW = 15;
const int chipSelect = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Game game(lcd);

// void setup(void) {
//   Serial.begin(115200);
//   lcd.init();
//   lcd.backlight();
//   game.setup();
//   game.initializeChars();
// }

#define BUZZER_PIN 17 // Define the pin for the buzzer

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as output
}

void loop() {
  // tone(BUZZER_PIN, 1000); // Generate a 1 kHz tone
  // delay(1000);            // Wait for 1 second
  // noTone(BUZZER_PIN);     // Stop the tone
  // delay(1000);            // Wait for 1 second
}

// void loop(void) {
//   // game.display();

//   // if (analogRead(pinX) < 1000) {
//   //   xMovement = -1;
//   // } else if (analogRead(pinX) > 2500) {
//   //   xMovement = 1;
//   // } else {
//   //   xMovement = 0;
//   // }

//   // currentTime = millis();

//   // if (!game.gameOver) {
//   //   if (xMovement != 0) {
//   //     if (!game.started) {
//   //       lastUpdateTime = millis();
//   //       game.started = true;
//   //     }

//   //     game.updateFrame(xMovement);
//   //   }

//   //   if (game.started) {
//   //     float delta = (currentTime - lastUpdateTime) / 1000.0f;
//   //     game.updateTimer(-delta);

//   //     lastUpdateTime = currentTime;

//   //     Serial.print("Time left: ");
//   //     Serial.print(game.timer);
//   //     Serial.println(" seconds.");

//   //     if (game.gameOver) {
//   //       Serial.println("Game over!");
//   //     }
//   //   }
//   // }

//   delay(10);
// }
