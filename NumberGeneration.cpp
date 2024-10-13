#include "NumberGeneration.h"

const Arduino_h::byte NumberGeneration::numbers[10][4] = {
    {B01110, B10001, B01110, B00000},
    {B10010, B11111, B10000, B00000},
    {B10010, B11001, B10110, B00000},
    {B10001, B10101, B01010, B00000},
    {B00111, B00100, B11111, B00000},
    {B10011, B10101, B01101, B00000},
    {B11110, B10101, B11101, B00000},
    {B00001, B00101, B11111, B00000},
    {B11011, B10101, B11011, B00000},
    {B00111, B00101, B11111, B00000}};

byte *NumberGeneration::GenerateByteNumber(int a, int b) {
  static Arduino_h::byte res[8];
  memcpy(res, numbers[a], 4);
  memcpy(res + 4, numbers[b], 4);
  return res;
}

void NumberGeneration::GenerateScoreBytes(int score, Arduino_h::byte *left, Arduino_h::byte *right) {
  score %= 10000;

  int a = score / 1000;
  int b = (score / 100) % 10;
  int c = (score / 10) % 10;
  int d = score % 10;

  memcpy(left, GenerateByteNumber(a, b), 8);
  memcpy(right, GenerateByteNumber(c, d), 8);
}
