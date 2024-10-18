#ifndef HIGH_SCORES_SCREEN_H
#define HIGH_SCORES_SCREEN_H

#include "BaseScreen.h"
#include "JSONHandler.h"
#include "Screen.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace HighScores {
enum LCDChars {
  Hashtag,
  Position,
  NameLeft,
  NameRight,
  ScoreLeft,
  ScoreRight,
  TitleLeft,
  TitleRight,
  ArrowDown = 60,
  ArrowUp = 62,
  Line = 124,
  Blank = 254,
  Black = 255
};
}

class HighScoresScreen : public Screen<HighScores::LCDChars> {
  public:
    HighScoresScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager);

    void handleInput() override;
    void update() override;

  private:
    static const int maxEntries = 10;
    static const Arduino_h::byte hashtag[8];
    int selectedIndex;
    JSONHandler jsonHandler;
    void initializeCharacters();
    void setup();
    void selectOption();
    void updateCharacters();
};

#endif
