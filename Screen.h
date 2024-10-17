#ifndef SCREEN_H
#define SCREEN_H

#include "BaseScreen.h"
#include "StateManager.h"
#include <LiquidCrystal_I2C.h>

#define JOYSTICK_X_PIN 2
#define JOYSTICK_Y_PIN 4
#define JOYSTICK_SW_PIN 15

#define JOYSTICK_THRESHOLD_LOW 1000
#define JOYSTICK_THRESHOLD_HIGH 2500
#define LOW 0

#define BUZZER_PIN 17
#define CHIP_SELECT 5

#define HEIGHT 16
#define WIDTH 2

template <typename CharType>
class Screen : public BaseScreen {
  protected:
    LiquidCrystal_I2C &lcd;
    StateManager &stateManager;
    CharType displayBuffer[HEIGHT][WIDTH];

  public:
    Screen(LiquidCrystal_I2C &lcd, StateManager &stateManager);
    virtual void display() override;
    virtual void resume() override;
};

#include "Screen.tpp"

#endif
