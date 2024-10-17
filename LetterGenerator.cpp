#include "LetterGenerator.h"

#include <Arduino.h>

const byte LetterGenerator::chars[TotalCharacters][4] = {
    {B11111, B00101, B11111, B00000},
    {B11111, B10101, B01010, B00000},
    {B01110, B10001, B10001, B00000},
    {B11111, B10001, B01110, B00000},
    {B11111, B10101, B10101, B00000},
    {B11111, B00101, B00101, B00000},
    {B01110, B10001, B11100, B00000},
    {B11111, B00100, B11111, B00000},
    {B10001, B11111, B10001, B00000},
    {B11000, B10000, B11111, B00000},
    {B11111, B00100, B11011, B00000},
    {B11111, B10000, B10000, B00000},
    {B11111, B00010, B11111, B00000},
    {B11111, B00001, B11111, B00000},
    {B11111, B10001, B11111, B00000},
    {B11111, B00101, B00111, B00000},
    {B00111, B00101, B11111, B00000},
    {B11111, B00101, B11010, B00000},
    {B10010, B10101, B01001, B00000},
    {B00001, B11111, B00001, B00000},
    {B11111, B10000, B11111, B00000},
    {B01111, B10000, B01111, B00000},
    {B11111, B01000, B11111, B00000},
    {B11011, B00100, B11011, B00000},
    {B00111, B11100, B00111, B00000},
    {B11001, B10101, B10011, B00000},
    {B01110, B10001, B01110, B00000},
    {B10010, B11111, B10000, B00000},
    {B10010, B11001, B10110, B00000},
    {B10001, B10101, B01010, B00000},
    {B00111, B00100, B11111, B00000},
    {B10011, B10101, B01101, B00000},
    {B11110, B10101, B11101, B00000},
    {B00001, B00101, B11111, B00000},
    {B11011, B10101, B11011, B00000},
    {B00111, B00101, B11111, B00000},
    {B00000, B00000, B00000, B00000}};

Arduino_h::byte *LetterGenerator::getBytes(char firstChar, char secondChar) {
  static byte result[8] = {0};

  auto mapCharToBytes = [&](char c, Arduino_h::byte *output) {
    if (c >= 'A' && c <= 'Z') {
      memcpy(output, chars[c - 'A'], 4);
    } else if (c >= '0' && c <= '9') {
      memcpy(output, chars[NUM_0 + (c - '0')], 4);
    } else if (c == ' ') {
      memcpy(output, chars[SPACE], 4);
    }
  };

  mapCharToBytes(firstChar, result);
  mapCharToBytes(secondChar, result + 4);

  return result;
}
