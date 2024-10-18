#include "MainMenu.h"
#include "CreditsScreen.h"
#include "GameScreen.h"
#include "HighScoresScreen.h"
#include "LetterGenerator.h"
#include <Arduino.h>

const Menu::Option MainMenu::menuOptions[menuSize] = {
    {10, "PLAY", {Menu::LCDChars::PlayLeft, Menu::LCDChars::PlayRight}},
    {7, "CRED", {Menu::LCDChars::CredLeft, Menu::LCDChars::CredRight}},
    {4, "HIGH", {Menu::LCDChars::HighLeft, Menu::LCDChars::HighRight}}};

const Arduino_h::byte MainMenu::logo[2][8] = {
    {B11111, B11111, B11000, B11000, B00000, B01110, B11111, B10001},
    {B10001, B11111, B01110, B00000, B01110, B11011, B11001, B11101}};

MainMenu::MainMenu(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : Screen(lcd, stateManager), selectedIndex(0) {
  initializeCharacters();
  setup();
}

void MainMenu::handleInput() {
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

void MainMenu::update() {
  for (int i = 0; i < menuSize; i++) {
    for (int j = 0; j < WIDTH; j++) {
      int row = menuOptions[i].row;
      bool isSelected = (i == selectedIndex);

      displayBuffer[row - 1][j] = isSelected ? Menu::LCDChars::Line : Menu::LCDChars::Blank;
      displayBuffer[row + 1][j] = isSelected ? Menu::LCDChars::Line : Menu::LCDChars::Blank;
    }
  }

  display();
}

void MainMenu::initializeCharacters() {
  lcd.createChar(Menu::LCDChars::LogoLeft, (uint8_t *)MainMenu::logo[0]);
  lcd.createChar(Menu::LCDChars::LogoRight, (uint8_t *)MainMenu::logo[1]);

  for (int i = 0; i < menuSize; i++) {
    lcd.createChar(
        menuOptions[i].chars[0], // Left character
        LetterGenerator::getBytes(menuOptions[i].name[0], menuOptions[i].name[1]));
    lcd.createChar(
        menuOptions[i].chars[1], // Right character
        LetterGenerator::getBytes(menuOptions[i].name[2], menuOptions[i].name[3]));
  }
}

void MainMenu::setup() {
  memset(displayBuffer, Menu::LCDChars::Blank, sizeof(displayBuffer));

  displayBuffer[14][0] = Menu::LCDChars::LogoLeft;
  displayBuffer[14][1] = Menu::LCDChars::LogoRight;

  for (int i = 0; i < menuSize; i++) {
    for (int j = 0; j < WIDTH; j++) {
      displayBuffer[menuOptions[i].row][j] = menuOptions[i].chars[j];
    }
  }
}

void MainMenu::selectOption() {
  tone(BUZZER_PIN, 1500, 100);
  BaseScreen *targetScreen = nullptr;

  switch (selectedIndex) {
  case Play:
    targetScreen = new GameScreen(lcd, stateManager);
    break;
  case Credits:
    targetScreen = new CreditsScreen(lcd, stateManager);
    break;
  case HighScores:
    targetScreen = new HighScoresScreen(lcd, stateManager);
    break;
  }

  stateManager.lastDebounceTime = millis();
  if (targetScreen) {
    stateManager.changeState(targetScreen);
  }
}
