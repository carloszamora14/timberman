#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "BaseScreen.h"
#include "Screen.h"
#include "StateManager.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class GameScreen;

namespace Menu {
enum LCDChars {
  LogoLeft,
  LogoRight,
  PlayLeft,
  PlayRight,
  CredLeft,
  CredRight,
  HighLeft,
  HighRight,
  Line = 124,
  Blank = 254,
  Black = 255
};

struct Option {
    int row;
    const char name[5];
    LCDChars chars[2];
};
} // namespace Menu

class MainMenu : public Screen<Menu::LCDChars> {
  public:
    MainMenu(LiquidCrystal_I2C &lcd, StateManager &stateManager);

    void handleInput() override;
    void update() override;

  private:
    enum Options {
      Play,
      Credits,
      HighScores
    };

    static const int menuSize = 3;
    static const Menu::Option menuOptions[menuSize];
    static const Arduino_h::byte logo[2][8];
    int selectedIndex;
    void initializeCharacters();
    void setup();
    void selectOption();
};

#endif
