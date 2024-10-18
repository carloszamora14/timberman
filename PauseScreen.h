#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

#include "BaseScreen.h"
#include "GameScreen.h"
#include "Screen.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace PauseMenu {
enum LCDChars {
  ResumeLeft,
  ResumeRight,
  QuitLeft,
  QuitRight,
  Line = 124,
  Blank = 254,
  Black = 255
};

struct Option {
    int row;
    const char name[5];
    LCDChars chars[2];
};
} // namespace PauseMenu

class PauseScreen : public Screen<PauseMenu::LCDChars> {
  public:
    PauseScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager);

    void handleInput() override;
    void update() override;

  private:
    enum Options {
      Resume,
      Quit
    };

    static const int menuSize = 2;
    static const PauseMenu::Option menuOptions[menuSize];
    int selectedIndex;
    void initializeCharacters();
    void setup();
    void selectOption();
};

#endif
