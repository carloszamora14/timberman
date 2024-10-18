#include "PauseScreen.h"
#include "BaseScreen.h"
#include "LetterGenerator.h"
#include "MainMenu.h"

const PauseMenu::Option PauseScreen::menuOptions[PauseScreen::menuSize] = {
    {9, "CONT", {PauseMenu::ResumeLeft, PauseMenu::ResumeRight}},
    {6, "QUIT", {PauseMenu::QuitLeft, PauseMenu::QuitRight}},
};

PauseScreen::PauseScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : Screen(lcd, stateManager), selectedIndex(0) {
  initializeCharacters();
  setup();
}

void PauseScreen::initializeCharacters() {
  for (int i = 0; i < menuSize; i++) {
    lcd.createChar(
        menuOptions[i].chars[0], // Left character
        LetterGenerator::getBytes(menuOptions[i].name[0], menuOptions[i].name[1]));
    lcd.createChar(
        menuOptions[i].chars[1], // Right character
        LetterGenerator::getBytes(menuOptions[i].name[2], menuOptions[i].name[3]));
  }
}

void PauseScreen::setup() {
  memset(displayBuffer, PauseMenu::LCDChars::Blank, sizeof(displayBuffer));

  for (int i = 0; i < menuSize; i++) {
    for (int j = 0; j < WIDTH; j++) {
      displayBuffer[menuOptions[i].row][j] = menuOptions[i].chars[j];
    }
  }
}

void PauseScreen::handleInput() {
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
      tone(BUZZER_PIN, 1000, 100);
      selectedIndex = (selectedIndex + yValue + menuSize) % menuSize;
      stateManager.lastDebounceTime = millis();
    } else if (buttonValue == LOW) {
      selectOption();
    }
  }
}

void PauseScreen::selectOption() {
  tone(BUZZER_PIN, 1500, 100);
  BaseScreen *targetScreen = nullptr;
  bool resume = false;

  switch (selectedIndex) {
  case Resume:
    resume = true;
    targetScreen = new GameScreen(lcd, stateManager);
    break;
  case Quit:
    targetScreen = new MainMenu(lcd, stateManager);
    break;
  }

  stateManager.lastDebounceTime = millis();
  if (targetScreen) {
    if (resume) {
      stateManager.resumePreviousState();
    } else {
      stateManager.changeState(targetScreen);
    }
  }
}

void PauseScreen::update() {
  for (int i = 0; i < menuSize; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int row = menuOptions[i].row;
      bool isSelected = (i == selectedIndex);

      displayBuffer[row - 1][j] = isSelected ? PauseMenu::LCDChars::Line : PauseMenu::LCDChars::Blank;
      displayBuffer[row + 1][j] = isSelected ? PauseMenu::LCDChars::Line : PauseMenu::LCDChars::Blank;
    }
  }

  display();
}
