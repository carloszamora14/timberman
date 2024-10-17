#include "Screen.h"

template <typename CharType>
Screen<CharType>::Screen(LiquidCrystal_I2C &lcd, StateManager &stateManager)
    : lcd(lcd), stateManager(stateManager) {
}

template <typename CharType>
void Screen<CharType>::display() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      lcd.setCursor(i, j);
      lcd.write(displayBuffer[i][j]);
    }
  }
}

template <typename CharType>
void Screen<CharType>::resume() {}
