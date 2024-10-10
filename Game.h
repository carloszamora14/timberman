#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <NumberGeneration.h>

#define WIDTH 2
#define HEIGHT 16
#define STARTING_ROW 2
#define ENDING_ROW 14
#define BRANCH_PROB 3

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

    static byte treeChars[4][8];
    static byte playerChars[2][8];

    LiquidCrystal_I2C &lcd;
    Character grid[HEIGHT][WIDTH];
    void generateBranch(int row);
    void handlePlayerMovement(int xMovement);
    void updateScore();
    int xPos;
};

#endif
