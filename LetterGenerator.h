#ifndef LETTER_GENERATOR_H
#define LETTER_GENERATOR_H

#include <Arduino.h>

class LetterGenerator {
  public:
    static Arduino_h::byte *getBytes(char firstChar, char secondChar);

  private:
    static const Arduino_h::byte chars[][4];
    enum CharIndex {
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
      NUM_9,
      SPACE,
      TotalCharacters
    };
};

#endif
