#include "GameOverScreen.h"
#include "BaseScreen.h"
#include "JSONHandler.h"
#include "LetterGenerator.h"
#include "MainMenu.h"
#include "RegisterScoreScreen.h"
#include "StateManager.h"

GameOverScreen::GameOverScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager, int score)
    : Screen(lcd, stateManager), score(score), isHighScore(false), avoidAdvancing(true) {
  initializeCharacters();
  setup();
}

void GameOverScreen::initializeCharacters() {
  lcd.createChar(0, LetterGenerator::getBytes('G', 'A'));
  lcd.createChar(1, LetterGenerator::getBytes('M', 'E'));
  lcd.createChar(2, LetterGenerator::getBytes('O', 'V'));
  lcd.createChar(3, LetterGenerator::getBytes('E', 'R'));

  lcd.createChar(4, LetterGenerator::getBytes('C', 'O'));
  lcd.createChar(5, LetterGenerator::getBytes('N', 'T'));
}

void GameOverScreen::setup() {
  memset(displayBuffer, GameOver::LCDChars::Blank, sizeof(displayBuffer));

  displayBuffer[2][0] = GameOver::LCDChars::ContLeft;
  displayBuffer[2][1] = GameOver::LCDChars::ContRight;
  displayBuffer[1][0] = GameOver::LCDChars::Line;
  displayBuffer[1][1] = GameOver::LCDChars::Line;
  displayBuffer[3][0] = GameOver::LCDChars::Line;
  displayBuffer[3][1] = GameOver::LCDChars::Line;

  displayBuffer[9][0] = GameOver::LCDChars::GameLeft;
  displayBuffer[9][1] = GameOver::LCDChars::GameRight;
  displayBuffer[8][0] = GameOver::LCDChars::OverLeft;
  displayBuffer[8][1] = GameOver::LCDChars::OverRight;

  while (!Serial) {
    ;
  }

  if (!SD.begin(CHIP_SELECT)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  JSONHandler jsonHandler("/scores.json");

  if (jsonHandler.readScores()) {
    jsonHandler.printScores();
  }

  if (jsonHandler.shouldSaveScore(score)) {
    isHighScore = true;
  }

  avoidAdvancing = false;
}

void GameOverScreen::handleInput() {
  unsigned long currentTime = millis();
  int buttonValue = digitalRead(JOYSTICK_SW_PIN);

  if (!avoidAdvancing && (currentTime - stateManager.lastDebounceTime) > stateManager.debounceDelay) {
    if (buttonValue == LOW) {
      stateManager.lastDebounceTime = currentTime;
      selectOption();
    }
  }
}

void GameOverScreen::selectOption() {
  tone(BUZZER_PIN, 1500, 100);
  BaseScreen *targetScreen = nullptr;

  if (isHighScore) {
    targetScreen = new RegisterScoreScreen(lcd, stateManager, score);
  } else {
    targetScreen = new MainMenu(lcd, stateManager);
  }

  stateManager.lastDebounceTime = millis();
  if (targetScreen) {
    stateManager.changeState(targetScreen);
  }
}

void GameOverScreen::update() {}
