#include "GameScreen.h"
#include "BaseScreen.h"
#include "GameOverScreen.h"
#include "LetterGenerator.h"
#include "MainMenu.h"
#include "PauseScreen.h"
#include <Arduino.h>

const float TIMER_INCREMENT = 0.3f;
const float MIN_TIMER_VALUE = 0.0f;
const float MAX_TIMER_VALUE = 8.0f;

const Arduino_h::byte GameScreen::treeChars[4][8] = {
    {B00000, B00000, B00110, B00100, B00100, B00100, B11111, B11111},
    {B11111, B11111, B00100, B00100, B00100, B01100, B00000, B00000},
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000},
};

const Arduino_h::byte GameScreen::playerChars[2][8] = {
    {B00000, B00000, B11100, B11100, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B11100, B11100, B00000, B00000},
};

GameScreen::GameScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : Screen(lcd, stateManager), gameOver(false), xPos(0), timer(4.0f), paused(true), score(0), barsToFill(0), shouldUpdateGrid(false), lastUpdated(0) {
  initializeCharacters();
  setup();
}

void GameScreen::handleInput() {
  if (gameOver) {
    shouldUpdateGrid = false;
    return;
  }

  unsigned long currentTime = millis();
  int buttonValue = digitalRead(JOYSTICK_SW_PIN);
  int xValue = 0;

  if (analogRead(JOYSTICK_X_PIN) < JOYSTICK_THRESHOLD_LOW) {
    xValue = -1;
  } else if (analogRead(JOYSTICK_X_PIN) > JOYSTICK_THRESHOLD_HIGH) {
    xValue = 1;
  }

  if (xValue != 0) {
    if (lastUpdated == 0 || paused) {
      Serial.println("Starting");
      lastUpdated = currentTime;
      lastTime = currentTime;
      paused = false;
    }

    if ((millis() - lastUpdated) < 15) {
      Serial.println(millis() - lastUpdated);
      return;
    }

    shouldUpdateGrid = true;
    handlePlayerMovement(xValue);
  }

  if ((currentTime - stateManager.lastDebounceTime) > stateManager.debounceDelay) {
    if (buttonValue == LOW) {
      tone(500, 500);
      paused = true;
      BaseScreen *pauseScreen = new PauseScreen(lcd, stateManager);

      stateManager.lastDebounceTime = currentTime;
      stateManager.changeState(pauseScreen);
    }
  }
}

void GameScreen::setup() {
  memset(displayBuffer, Game::LCDChars::Blank, sizeof(displayBuffer));

  for (int i = STARTING_ROW; i < ENDING_ROW; i++) {
    displayBuffer[i][0] = Game::LCDChars::TrunkLeft;
    displayBuffer[i][1] = Game::LCDChars::TrunkRight;

    // To avoid instant death
    if (i > STARTING_ROW) {
      generateBranch(i);
    }
  }

  displayBuffer[STARTING_ROW][xPos] = Game::LCDChars::Player;
  displayBuffer[HEIGHT - 1][0] = Game::LCDChars::NumberLeft;
  displayBuffer[HEIGHT - 1][1] = Game::LCDChars::NumberRight;

  updateScore();
  updateTimer(0.0f);
}

void GameScreen::initializeCharacters() {
  for (int i = 0; i < 4; i++) {
    lcd.createChar(i, (uint8_t *)treeChars[i]);
  }

  lcd.createChar(Game::LCDChars::Player, (uint8_t *)playerChars[xPos]);
}

void GameScreen::generateBranch(int row) {
  if (random(BRANCH_PROB) % 3 != 0) {
    int branchPosition = random(WIDTH);

    if (branchPosition == 0 && displayBuffer[row - 1][1] != Game::LCDChars::BranchRight) {
      displayBuffer[row][0] = Game::LCDChars::BranchLeft;
    } else if (branchPosition == 1 && displayBuffer[row - 1][0] != Game::LCDChars::BranchLeft) {
      displayBuffer[row][1] = Game::LCDChars::BranchRight;
    }
  }
}

void GameScreen::handlePlayerMovement(int xValue) {
  int newXPos = xPos + xValue;

  if (newXPos >= 0 && newXPos < WIDTH) {
    xPos = newXPos;
  }

  if (displayBuffer[STARTING_ROW + 1][xPos] == (xPos == 0 ? Game::LCDChars::BranchLeft : Game::LCDChars::BranchRight)) {
    gameOver = true;
    displayBuffer[STARTING_ROW][xPos] = Game::LCDChars::Black;
    tone(BUZZER_PIN, 500, 500);
  } else {
    if (score < 9999) {
      score++;
    }
    tone(BUZZER_PIN, 1000, 100);
    lcd.createChar(Game::LCDChars::Player, (uint8_t *)playerChars[xPos]);
    displayBuffer[STARTING_ROW][xPos] = Game::LCDChars::Player;
  }
}

void GameScreen::update() {
  if (!shouldUpdateGrid && (paused || gameOver)) {
    return;
  }

  unsigned long currentTime = millis();
  float decrement = (currentTime - lastTime) / 1000.0f;
  updateTimer(-decrement);

  if (shouldUpdateGrid) {
    updateTimer(TIMER_INCREMENT);
    updateScore();

    for (int i = STARTING_ROW; i < ENDING_ROW - 1; i++) {
      for (int j = 0; j < WIDTH; j++) {
        if (i == STARTING_ROW && j == xPos) {
          continue;
        }

        displayBuffer[i][j] = displayBuffer[i + 1][j];
      }
    }

    displayBuffer[ENDING_ROW - 1][0] = Game::LCDChars::TrunkLeft;
    displayBuffer[ENDING_ROW - 1][1] = Game::LCDChars::TrunkRight;

    generateBranch(ENDING_ROW - 1);
    shouldUpdateGrid = false;
  }

  lastTime = millis();

  if (gameOver) {
    BaseScreen *gameOverScreen = new GameOverScreen(lcd, stateManager, score);
    stateManager.lastDebounceTime = currentTime;
    stateManager.changeState(gameOverScreen);
  } else {
    display();
  }
}

void GameScreen::updateScore() {
  char numberStr[5];
  sprintf(numberStr, "%04d", score);

  lcd.createChar(Game::LCDChars::NumberLeft, LetterGenerator::getBytes(numberStr[0], numberStr[1]));
  lcd.createChar(Game::LCDChars::NumberRight, LetterGenerator::getBytes(numberStr[2], numberStr[3]));
}

void GameScreen::updateTimer(float delta) {
  Serial.print("timer increment: ");
  Serial.println(delta);
  timer += delta;

  if (timer < MIN_TIMER_VALUE) {
    timer = 0.0f;
    gameOver = true;
    tone(BUZZER_PIN, 500, 500);
  } else if (timer > MAX_TIMER_VALUE) {
    timer = MAX_TIMER_VALUE;
  }

  Arduino_h::byte bar[8] = {0};
  int newBarsToFill = round(2 * timer - (timer > MAX_TIMER_VALUE / 2 ? MAX_TIMER_VALUE : 0));

  if (newBarsToFill != barsToFill) {
    for (int i = 0; i < newBarsToFill; i++) {
      bar[i] = B11111;
    }

    lcd.createChar(Game::LCDChars::TimerBar, bar);

    if (timer <= MAX_TIMER_VALUE / 2) {
      displayBuffer[0][0] = Game::LCDChars::TimerBar;
      displayBuffer[0][1] = Game::LCDChars::Blank;
    } else {
      displayBuffer[0][0] = Game::LCDChars::Black;
      displayBuffer[0][1] = Game::LCDChars::TimerBar;
    }

    barsToFill = newBarsToFill;
  }
}

void GameScreen::resume() {
  initializeCharacters();
  display();
}
