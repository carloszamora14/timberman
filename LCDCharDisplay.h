#ifndef LCDCHARDISPLAY_H
#define LCDCHARDISPLAY_H

#include <LiquidCrystal_I2C.h>

class LCDCharDisplay {
  public:
    enum LCDChar {
      A,
      B,
      C,
      D,
      E,
      F,
      G,
      H,
      I,
      J,
      K,
      L,
      M,
      N,
      O,
      P,
      Q,
      R,
      S,
      T,
      U,
      V,
      W,
      X,
      Y,
      Z,
      NUM_0,
      NUM_1,
      NUM_2,
      NUM_3,
      NUM_4,
      NUM_5,
      NUM_6,
      NUM_7,
      NUM_8,
      NUM_9
    };

    LCDCharDisplay(LiquidCrystal_I2C &lcd);
    void displayCombinedChars(LCDChar char1, LCDChar char2, int x, int y, bool invert = false);

  private:
    LiquidCrystal_I2C &lcd;
    static const byte chars[36][4];

    void initializeChars();
};

#endif
