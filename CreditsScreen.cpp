#include "CreditsScreen.h"
#include "BaseScreen.h"
#include "MainMenu.h"
#include "StateManager.h"

CreditsScreen::CreditsScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : Screen(lcd, stateManager), currentLine(0), lastUpdateTime(0) {}

void CreditsScreen::handleInput() {
  unsigned long currentTime = millis();
  int buttonValue = digitalRead(JOYSTICK_SW_PIN);

  if ((currentTime - stateManager.lastDebounceTime) >= stateManager.debounceDelay) {
    if (buttonValue == LOW) {
      tone(BUZZER_PIN, 1500, 100);
      BaseScreen *target = new MainMenu(lcd, stateManager);

      stateManager.lastDebounceTime = currentTime;
      stateManager.changeState(target);
    }
  }
}

void CreditsScreen::display() {
  lcd.clear();

  for (int i = 0; i < WIDTH; ++i) {
    int lineIndex = (currentLine + i) % totalLines;
    lcd.setCursor(0, i);
    lcd.print(credits[lineIndex]);
  }
}

void CreditsScreen::update() {
  unsigned long currentTime = millis();

  if (currentTime - lastUpdateTime >= scrollDelay) {
    currentLine++;
    if (currentLine >= totalLines) {
      currentLine = 0;
    }

    lastUpdateTime = currentTime;

    display();
  }
}
