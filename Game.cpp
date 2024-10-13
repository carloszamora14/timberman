#include "Game.h"

const float TIMER_INCREMENT = 0.3f;
const float MIN_TIMER_VALUE = 0.0f;
const float MAX_TIMER_VALUE = 8.0f;

Arduino_h::byte Game::treeChars[4][8] = {
    {B00000, B00000, B00110, B00100, B00100, B00100, B11111, B11111},
    {B11111, B11111, B00100, B00100, B00100, B01100, B00000, B00000},
    {B00000, B00000, B00000, B00000, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B00000, B00000, B00000, B00000},
};

Arduino_h::byte Game::playerChars[2][8] = {
    {B00000, B00000, B11100, B11100, B00000, B00000, B11111, B11111},
    {B11111, B11111, B00000, B00000, B11100, B11100, B00000, B00000},
};

Game::Game(LiquidCrystal_I2C &lcdRef)
    : lcd(lcdRef), gameOver(false), xPos(0), timer(4.0f), paused(true), score(0), barsToFill(0) {}

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

  grid[STARTING_ROW][xPos] = Player;
  grid[HEIGHT - 1][0] = NumberLeft;
  grid[HEIGHT - 1][1] = NumberRight;

  updateScore();
  updateTimer(0.0f);
}

void Game::initializeChars() {
  for (int i = 0; i < 4; i++) {
    lcd.createChar(i, treeChars[i]);
  }

  lcd.createChar(Player, playerChars[xPos]);
}

void Game::generateBranch(int row) {
  if (random(BRANCH_PROB) % 3 != 0) {
    int branchPosition = random(WIDTH);

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
    grid[STARTING_ROW][xPos] = Black;
    tone(BUZZER_PIN, 500, 500);
  } else {
    score++;
    tone(BUZZER_PIN, 1000, 100);
    lcd.createChar(Player, playerChars[xPos]);
    grid[STARTING_ROW][xPos] = Player;
  }
}

void Game::updateFrame(int xMovement) {
  updateTimer(TIMER_INCREMENT);
  updateScore();

  for (int i = STARTING_ROW; i < ENDING_ROW - 1; i++) {
    grid[i][0] = grid[i + 1][0];
    grid[i][1] = grid[i + 1][1];
  }

  handlePlayerMovement(xMovement);

  grid[ENDING_ROW - 1][0] = TrunkLeft;
  grid[ENDING_ROW - 1][1] = TrunkRight;

  generateBranch(ENDING_ROW - 1);
}

void Game::updateScore() {
  Arduino_h::byte left[8];
  Arduino_h::byte right[8];

  NumberGeneration::GenerateScoreBytes(score, left, right);
  lcd.createChar(NumberLeft, left);
  lcd.createChar(NumberRight, right);
}

void Game::updateTimer(float delta) {
  timer += delta;

  if (timer < MIN_TIMER_VALUE) {
    timer = 0.0f;
    gameOver = true;
    tone(BUZZER_PIN, 500, 500);
  } else if (timer > MAX_TIMER_VALUE) {
    timer = MAX_TIMER_VALUE;
  }

  Arduino_h::byte bar[8] = {0};
  int newBarsToFill = round(2 * timer - (timer > MAX_TIMER_VALUE / 2 ? MAX_TIMER_VALUE : 0));

  if (newBarsToFill != barsToFill) {
    for (int i = 0; i < newBarsToFill; i++) {
      bar[i] = B11111;
    }

    lcd.createChar(TimerBar, bar);

    if (timer <= MAX_TIMER_VALUE / 2) {
      grid[0][0] = TimerBar;
      grid[0][1] = Blank;
    } else {
      grid[0][0] = Black;
      grid[0][1] = TimerBar;
    }

    barsToFill = newBarsToFill;
  }
}

void Game::display() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      lcd.setCursor(i, j);
      lcd.write(grid[i][j]);
    }
  }
}
