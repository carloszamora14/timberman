#ifndef GAME_H
#define GAME_H

#include "NumberGeneration.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define WIDTH 2
#define HEIGHT 16
#define STARTING_ROW 2
#define ENDING_ROW 14
#define BRANCH_PROB 100
#define BUZZER_PIN 17

class Game {
  public:
    Game(LiquidCrystal_I2C &lcd);
    void setup();
    void initializeChars();
    void display();
    void updateTimer(float delta);
    void updateFrame(int xMovement);
    bool gameOver;
    bool paused;
    float timer;
    int score;

  private:
    enum Character {
      BranchLeft,
      BranchRight,
      TrunkLeft,
      TrunkRight,
      Player,
      NumberLeft,
      NumberRight,
      TimerBar,
      Blank = 254,
      Black = 255,
    };

    static Arduino_h::byte treeChars[4][8];
    static Arduino_h::byte playerChars[2][8];

    LiquidCrystal_I2C &lcd;
    Character grid[HEIGHT][WIDTH];
    void generateBranch(int row);
    void handlePlayerMovement(int xMovement);
    void updateScore();
    int xPos;
    int barsToFill;
};

#endif
