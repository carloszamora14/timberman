#include "HighScoresScreen.h"
#include "JSONHandler.h"
#include "LetterGenerator.h"
#include "MainMenu.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

const Arduino_h::byte HighScoresScreen::hashtag[8] = {
    B00000,
    B00000,
    B00000,
    B01010,
    B11111,
    B01010,
    B11111,
    B01010};

HighScoresScreen::HighScoresScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : Screen(lcd, stateManager), selectedIndex(0), jsonHandler("/scores.json") {
  if (!SD.begin()) {
    Serial.println("SD card initialization failed");
    return;
  }

  if (jsonHandler.readScores()) {
    jsonHandler.printScores();
  } else {
    return;
  }

  initializeCharacters();
  setup();
}

void HighScoresScreen::handleInput() {
  unsigned long currentTime = millis();
  int buttonValue = digitalRead(JOYSTICK_SW_PIN);
  int yValue = 0;

  if (analogRead(JOYSTICK_Y_PIN) < JOYSTICK_THRESHOLD_LOW) {
    yValue = 1;
  } else if (analogRead(JOYSTICK_Y_PIN) > JOYSTICK_THRESHOLD_HIGH) {
    yValue = -1;
  }

  if ((currentTime - stateManager.lastDebounceTime) > stateManager.debounceDelay) {
    if (yValue != 0) {
      if (yValue + selectedIndex >= 0 && yValue + selectedIndex < jsonHandler.size) {
        tone(BUZZER_PIN, 1000, 100);
        selectedIndex = selectedIndex + yValue;
        update();
      } else {
        tone(BUZZER_PIN, 600, 100);
      }
      stateManager.lastDebounceTime = millis();
    } else if (buttonValue == LOW) {
      selectOption();
    }
  }
}

void HighScoresScreen::update() {
  updateCharacters();
  display();
}

void HighScoresScreen::initializeCharacters() {
  lcd.createChar(HighScores::LCDChars::Hashtag, (uint8_t *)HighScoresScreen::hashtag);
  lcd.createChar(HighScores::LCDChars::TitleLeft, LetterGenerator::getBytes('H', 'I'));
  lcd.createChar(HighScores::LCDChars::TitleRight, LetterGenerator::getBytes('G', 'H'));

  updateCharacters();
}

void HighScoresScreen::updateCharacters() {
  displayBuffer[1][0] = selectedIndex == 0 ? HighScores::LCDChars::Blank : HighScores::LCDChars::ArrowUp;
  displayBuffer[1][1] = selectedIndex == jsonHandler.size - 1 ? HighScores::LCDChars::Blank : HighScores::LCDChars::ArrowDown;

  int currentScore = jsonHandler.scores[selectedIndex].score;
  char currentName[5];
  strcpy(currentName, jsonHandler.scores[selectedIndex].name);

  char numberStr[5];
  sprintf(numberStr, "%04d", currentScore);

  lcd.createChar(HighScores::LCDChars::ScoreLeft, LetterGenerator::getBytes(numberStr[0], numberStr[1]));
  lcd.createChar(HighScores::LCDChars::ScoreRight, LetterGenerator::getBytes(numberStr[2], numberStr[3]));

  lcd.createChar(HighScores::LCDChars::NameLeft, LetterGenerator::getBytes(currentName[0], currentName[1]));
  lcd.createChar(HighScores::LCDChars::NameRight, LetterGenerator::getBytes(currentName[2], currentName[3]));

  char positionStr[3];
  sprintf(positionStr, "%02d", selectedIndex + 1);

  lcd.createChar(HighScores::LCDChars::Position, LetterGenerator::getBytes(positionStr[0], positionStr[1]));
}

void HighScoresScreen::setup() {
  memset(displayBuffer, HighScores::LCDChars::Blank, sizeof(displayBuffer));

  displayBuffer[1][0] = HighScores::LCDChars::ArrowUp;
  displayBuffer[1][1] = HighScores::LCDChars::ArrowDown;

  displayBuffer[10][0] = HighScores::LCDChars::Hashtag;
  displayBuffer[10][1] = HighScores::LCDChars::Position;

  displayBuffer[8][0] = HighScores::LCDChars::NameLeft;
  displayBuffer[8][1] = HighScores::LCDChars::NameRight;

  displayBuffer[6][0] = HighScores::LCDChars::ScoreLeft;
  displayBuffer[6][1] = HighScores::LCDChars::ScoreRight;

  displayBuffer[14][0] = HighScores::LCDChars::TitleLeft;
  displayBuffer[14][1] = HighScores::LCDChars::TitleRight;
}

void HighScoresScreen::selectOption() {
  tone(BUZZER_PIN, 1500, 100);
  BaseScreen *targetScreen = new MainMenu(lcd, stateManager);

  stateManager.lastDebounceTime = millis();
  stateManager.changeState(targetScreen);
}
