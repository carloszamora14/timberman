#ifndef NUMBER_GENERATION_H
#define NUMBER_GENERATION_H

#include <Arduino.h>

class NumberGeneration {
  public:
    static Arduino_h::byte *GenerateByteNumber(int a, int b);
    static void GenerateScoreBytes(int score, Arduino_h::byte *left, Arduino_h::byte *right);

  private:
    static const Arduino_h::byte numbers[10][4];
};

#endif
