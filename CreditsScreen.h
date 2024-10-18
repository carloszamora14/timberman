#ifndef CREDITS_SCREEN_H
#define CREDITS_SCREEN_H

#include "Screen.h"

class CreditsScreen : public Screen<char> {
  private:
    const int totalLines = 27;
    int currentLine;
    unsigned long lastUpdateTime;
    const int scrollDelay = 500;

    const char *credits[27] = {
        " Videojuego LOG ",
        "                ",
        " Elaborado por: ",
        "                ",
        "  Damian Perez  ",
        "  Erik Roberto  ",
        "                ",
        " Ponce Ricarte  ",
        " Isaac Eduardo  ",
        "                ",
        " Santoyo Ochoa  ",
        "      Ivan      ",
        "                ",
        " Zamora Ortega  ",
        "  Carlos Uriel  ",
        "                ",
        "Material y guia;",
        "                ",
        "  Castellanos   ",
        "  Berjan Esli   ",
        "                ",
        "                ",
        "    Gracias     ",
        "   por jugar!   ",
        "                ",
        "                ",
        "                ",
    };

  public:
    CreditsScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager);
    void display() override;
    void handleInput() override;
    void update() override;
};

#endif
