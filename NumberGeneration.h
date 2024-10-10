#ifndef NUMBER_GENERATION_H
#define NUMBER_GENERATION_H

#include <Arduino.h>

class NumberGeneration {
  public:
    static byte *GenerateByteNumber(int a, int b);
    static void GenerateScoreBytes(int score, byte *left, byte *right);

  private:
    static const byte numbers[10][4];
};

#endif
