#include "LCDCharDisplay.h"
#include <Arduino.h>

const byte LCDCharDisplay::chars[36][4] = {
    {B11110, B00101, B11110, B00000}, // A
    {B11111, B10101, B01010, B00000}, // B
    {B01110, B10001, B10001, B00000}, // C
    {B11111, B10001, B01110, B00000}, // D
    {B11111, B10101, B10001, B00000}, // E
    {B11111, B00101, B00001, B00000}, // F
    {B01110, B10001, B11101, B00000}, // G
    {B11111, B00100, B11111, B00000}, // H
    {B10001, B11111, B10001, B00000}, // I
    {B01000, B10000, B11111, B00000}, // J
    {B11111, B01010, B10001, B00000}, // K
    {B11111, B10000, B10000, B00000}, // L
    {B11111, B00010, B11111, B00000}, // M
    {B11111, B00110, B11111, B00000}, // N
    {B01110, B10001, B01110, B00000}, // O
    {B11111, B00101, B00010, B00000}, // P
    {B01110, B11001, B11110, B00000}, // Q
    {B10111, B01101, B10010, B00000}, // R
    {B10010, B10101, B01001, B00000}, // S
    {B00001, B11111, B00001, B00000}, // T
    {B11111, B10000, B11111, B00000}, // U
    {B00111, B11000, B00111, B00000}, // V
    {B11111, B01100, B11111, B00000}, // W
    {B11011, B00100, B11011, B00000}, // X
    {B00001, B11110, B00001, B00000}, // Y
    {B11001, B10101, B10011, B00000}, // Z
    {B11111, B10001, B11111, B00000}, // 0
    {B10010, B11111, B10000, B00000}, // 1
    {B11101, B10101, B10111, B00000}, // 2
    {B10101, B10101, B11111, B00000}, // 3
    {B00111, B00100, B11111, B00000}, // 4
    {B10111, B10101, B11101, B00000}, // 5
    {B11111, B10101, B11101, B00000}, // 6
    {B00001, B00001, B11111, B00000}, // 7
    {B11111, B10101, B11111, B00000}, // 8
    {B10111, B10101, B11111, B00000}  // 9
};

LCDCharDisplay::LCDCharDisplay(LiquidCrystal_I2C &lcdRef) : lcd(lcdRef) {}

void LCDCharDisplay::displayCombinedChars(LCDChar char1, LCDChar char2, int x, int y, bool invert) {
  byte combined[8];

  memcpy(combined, chars[char1], 4);
  memcpy(combined + 4, chars[char2], 4);

  if (invert) {
    for (int i = 0; i < 8; i++) {
      combined[i] = ~combined[i] & B11111;
    }
  }

  lcd.createChar(0, combined);
  lcd.setCursor(x, y);
  lcd.write(0);
}
