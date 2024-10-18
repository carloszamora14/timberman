#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include "BaseScreen.h"
#include "Screen.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace GameOver {
enum LCDChars {
  GameLeft,
  GameRight,
  OverLeft,
  OverRight,
  ContLeft,
  ContRight,
  Line = 124,
  Blank = 254,
  Black = 255
};
}

class GameOverScreen : public Screen<GameOver::LCDChars> {
  public:
    GameOverScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager, int score);

    void handleInput() override;
    void update() override;

  private:
    void initializeCharacters();
    void setup();
    void selectOption();
    int score;
    bool isHighScore;
    bool avoidAdvancing;
};

#endif
