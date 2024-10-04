#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define WIDTH 2
#define HEIGHT 16
#define STARTING_ROW 2
#define ENDING_ROW 14
#define BRANCH_PROB 3

class Game {
  public:
    Game(LiquidCrystal_I2C &lcd);
    bool gameOver;
    float timer;
    void setup();
    void initializeChars();
    void display();
    void updateTimer(float delta);
    void updateFrame(int xMovement);
    bool started;

  private:
    enum Character {
      PlayerDeath = 0,
      PlayerLeft,
      PlayerRight,
      BranchLeft,
      BranchRight,
      TrunkLeft,
      TrunkRight,
      Blank
    };

    static byte chars[8][8];

    LiquidCrystal_I2C &lcd;
    Character grid[HEIGHT][WIDTH];
    int xPos;
    void generateBranch(int row);
    void handlePlayerMovement(int xMovement);
};

#endif
