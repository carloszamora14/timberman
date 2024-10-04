#include "Game.h"

const float TIMER_INCREMENT = 0.3f;
const float MIN_TIMER_VALUE = 0.0f;
const float MAX_TIMER_VALUE = 10.0f;

// In the same order as the Character enum
byte Game::chars[8][8] = {
    {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111},
    {B00000, B00000, B11100, B11100, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B11100, B11100, B00000, B00000},
    {B00000, B00000, B00110, B00100, B00100, B00100, B11111, B11111},
    {B11111, B11111, B00100, B00100, B00100, B01100, B00000, B00000},
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000},
    {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00000}};

Game::Game(LiquidCrystal_I2C &lcdRef) : lcd(lcdRef), gameOver(false), xPos(0), timer(5.0f), started(false) {}

void Game::setup() {
  for (int i = 0; i < STARTING_ROW; i++) {
    grid[i][0] = Blank;
    grid[i][1] = Blank;
  }

  for (int i = STARTING_ROW; i < ENDING_ROW; i++) {
    grid[i][0] = TrunkLeft;
    grid[i][1] = TrunkRight;

    // To avoid instant death
    if (i > STARTING_ROW) {
      generateBranch(i);
    }
  }

  for (int i = ENDING_ROW; i < HEIGHT; i++) {
    grid[i][0] = Blank;
    grid[i][1] = Blank;
  }

  grid[STARTING_ROW][xPos] = (xPos == 0) ? PlayerLeft : PlayerRight;
}

void Game::initializeChars() {
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, chars[i]);
  }
}

void Game::generateBranch(int row) {
  if (random(BRANCH_PROB) == 0) {
    int branchPosition = random(0, WIDTH);

    if (branchPosition == 0 && grid[row - 1][1] != BranchRight) {
      grid[row][0] = BranchLeft;
    } else if (branchPosition == 1 && grid[row - 1][0] != BranchLeft) {
      grid[row][1] = BranchRight;
    }
  }
}

void Game::handlePlayerMovement(int xMovement) {
  int newXPos = xPos + xMovement;

  if (newXPos >= 0 && newXPos < WIDTH) {
    xPos = newXPos;
  }

  if (grid[STARTING_ROW][xPos] == (xPos == 0 ? BranchLeft : BranchRight)) {
    gameOver = true;
    grid[STARTING_ROW][xPos] = PlayerDeath;
  } else {
    grid[STARTING_ROW][xPos] = (xPos == 0) ? PlayerLeft : PlayerRight;
  }
}

void Game::updateFrame(int xMovement) {
  updateTimer(TIMER_INCREMENT);

  for (int i = STARTING_ROW; i < ENDING_ROW - 1; i++) {
    grid[i][0] = grid[i + 1][0];
    grid[i][1] = grid[i + 1][1];
  }

  handlePlayerMovement(xMovement);

  grid[ENDING_ROW - 1][0] = TrunkLeft;
  grid[ENDING_ROW - 1][1] = TrunkRight;

  generateBranch(ENDING_ROW - 1);
}

void Game::updateTimer(float delta) {
  timer += delta;

  if (timer < MIN_TIMER_VALUE) {
    gameOver = true;
  } else if (timer > MAX_TIMER_VALUE) {
    timer = MAX_TIMER_VALUE;
  }
}

void Game::display() {
  // initializeChars();

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      lcd.setCursor(i, j);
      lcd.write(grid[i][j]);
    }
  }
}
