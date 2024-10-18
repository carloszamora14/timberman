#ifndef REGISTER_SCORE_SCREEN_H
#define REGISTER_SCORE_SCREEN_H

#include "BaseScreen.h"
#include "Screen.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace ScoreMenu {
enum LCDChars {
  ScoreLeft,
  ScoreRight,
  NameLeft,
  NameRight,
  TopArrow,
  DownArrow,
  Confirmation,
  QuestionMark,
  Line = 124,
  Blank = 254,
  Black = 255
};
}

class RegisterScoreScreen : public Screen<ScoreMenu::LCDChars> {
  public:
    RegisterScoreScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager, int score);

    void handleInput() override;
    void update() override;

  private:
    static const int nameSize = 5;
    static const int debounce = 250;
    static const Arduino_h::byte arrows[2][4];
    static const Arduino_h::byte questionMark[8];
    static const Arduino_h::byte space[4];
    unsigned long lastDebounce;
    char name[nameSize];
    int selectedIndex;
    int score;
    int charIndex[nameSize];
    const char charSet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    void initializeCharacters();
    void setup();
    void updateLetters();
    void updateArrows();
    void selectCharacter();
    void incrementCharacter(int direction);
};

#endif
