#include "RegisterScoreScreen.h"
#include "BaseScreen.h"
#include "JSONHandler.h"
#include "LetterGenerator.h"
#include "MainMenu.h"
#include "StateManager.h"

const Arduino_h::byte RegisterScoreScreen::arrows[2][4] = {
    {B01000, B00100, B01000, B00000},
    {B00010, B00100, B00010, B00000}};

const Arduino_h::byte RegisterScoreScreen::questionMark[8] = {
    B00010,
    B00001,
    B10101,
    B00010,
    B00000,
    B00000,
    B00000,
    B00000};

const Arduino_h::byte RegisterScoreScreen::space[4] = {
    B00000,
    B00000,
    B00000,
    B00000};

RegisterScoreScreen::RegisterScoreScreen(LiquidCrystal_I2C &lcd, StateManager &stateManager, int score)
    : Screen(lcd, stateManager), selectedIndex(0), lastDebounce(0), score(score) {
  for (int i = 0; i < nameSize - 1; i++) {
    charIndex[i] = 0;
    name[i] = charSet[charIndex[i]];
  }
  name[4] = '\0';
  initializeCharacters();
  setup();
}

void RegisterScoreScreen::handleInput() {
  unsigned long currentTime = millis();
  int buttonValue = digitalRead(JOYSTICK_SW_PIN);
  int xValue = 0, yValue = 0;

  if ((currentTime - lastDebounce) > debounce) {
    if (analogRead(JOYSTICK_Y_PIN) < JOYSTICK_THRESHOLD_LOW) {
      yValue = -1;
    } else if (analogRead(JOYSTICK_Y_PIN) > JOYSTICK_THRESHOLD_HIGH) {
      yValue = 1;
    }

    if (analogRead(JOYSTICK_X_PIN) < JOYSTICK_THRESHOLD_LOW) {
      xValue = -1;
    } else if (analogRead(JOYSTICK_X_PIN) > JOYSTICK_THRESHOLD_HIGH) {
      xValue = 1;
    }

    if (yValue != 0) {
      lastDebounce = currentTime;
      tone(BUZZER_PIN, 1500, 100);
      incrementCharacter(yValue);
      updateLetters();
    } else if (xValue != 0) {
      lastDebounce = currentTime;
      tone(BUZZER_PIN, 1000, 100);
      selectedIndex = (selectedIndex + xValue + 5) % 5;
      updateArrows();
    }

    if (yValue != 0 || xValue != 0) {
      display();
      Serial.println(selectedIndex);
    }
  }

  if ((currentTime - stateManager.lastDebounceTime) > stateManager.debounceDelay) {
    if (buttonValue == LOW && selectedIndex == 4) {
      tone(BUZZER_PIN, 1100, 200);
      JSONHandler jsonHandler("/scores.json");
      if (!SD.begin()) {
        Serial.println("SD card initialization failed");
        return;
      }

      if (jsonHandler.readScores()) {
        jsonHandler.printScores();
      }

      if (jsonHandler.shouldSaveScore(score)) {
        jsonHandler.compareAndStore(name, score);
        Serial.println("New score saved");
        jsonHandler.printScores();
      }

      BaseScreen *menu = new MainMenu(lcd, stateManager);
      stateManager.lastDebounceTime = currentTime;
      stateManager.changeState(menu);
    } else if (buttonValue == LOW) {
      tone(BUZZER_PIN, 750, 200);
    }
  }
}

void RegisterScoreScreen::update() {}

void RegisterScoreScreen::updateLetters() {
  lcd.createChar(ScoreMenu::LCDChars::NameLeft, LetterGenerator::getBytes(name[0], name[1]));
  lcd.createChar(ScoreMenu::LCDChars::NameRight, LetterGenerator::getBytes(name[2], name[3]));
}

void RegisterScoreScreen::initializeCharacters() {
  char numberStr[5];
  sprintf(numberStr, "%04d", score);

  lcd.createChar(ScoreMenu::LCDChars::ScoreLeft, LetterGenerator::getBytes(numberStr[0], numberStr[1]));
  lcd.createChar(ScoreMenu::LCDChars::ScoreRight, LetterGenerator::getBytes(numberStr[2], numberStr[3]));

  updateLetters();
  updateArrows();

  lcd.createChar(ScoreMenu::LCDChars::QuestionMark, (uint8_t *)RegisterScoreScreen::questionMark);

  lcd.createChar(ScoreMenu::LCDChars::Confirmation, LetterGenerator::getBytes('O', 'K'));
}

void RegisterScoreScreen::updateArrows() {
  if (selectedIndex == 4) {
    for (int i = 0; i < WIDTH; i++) {
      displayBuffer[8][i] = ScoreMenu::LCDChars::Blank;
      displayBuffer[10][i] = ScoreMenu::LCDChars::Blank;

      displayBuffer[1][i] = ScoreMenu::LCDChars::Line;
      displayBuffer[3][i] = ScoreMenu::LCDChars::Line;
    }
  } else {
    for (int i = 0; i < WIDTH; i++) {
      displayBuffer[1][i] = ScoreMenu::LCDChars::Blank;
      displayBuffer[3][i] = ScoreMenu::LCDChars::Blank;
    }

    displayBuffer[8][0] = selectedIndex <= 1 ? ScoreMenu::LCDChars::DownArrow : ScoreMenu::LCDChars::Blank;
    displayBuffer[8][1] = selectedIndex <= 1 ? ScoreMenu::LCDChars::Blank : ScoreMenu::LCDChars::DownArrow;
    displayBuffer[10][0] = selectedIndex <= 1 ? ScoreMenu::LCDChars::TopArrow : ScoreMenu::LCDChars::Blank;
    displayBuffer[10][1] = selectedIndex <= 1 ? ScoreMenu::LCDChars::Blank : ScoreMenu::LCDChars::TopArrow;

    Arduino_h::byte aux[8];

    if (selectedIndex % 2 == 0) {
      memcpy(aux, RegisterScoreScreen::arrows[0], 4);
      memcpy(aux + 4, RegisterScoreScreen::space, 4);
      lcd.createChar(ScoreMenu::LCDChars::TopArrow, aux);

      memcpy(aux, RegisterScoreScreen::arrows[1], 4);
      memcpy(aux + 4, RegisterScoreScreen::space, 4);
      lcd.createChar(ScoreMenu::LCDChars::DownArrow, aux);
    } else {
      memcpy(aux, RegisterScoreScreen::space, 4);
      memcpy(aux + 4, RegisterScoreScreen::arrows[0], 4);
      lcd.createChar(ScoreMenu::LCDChars::TopArrow, aux);

      memcpy(aux, RegisterScoreScreen::space, 4);
      memcpy(aux + 4, RegisterScoreScreen::arrows[1], 4);
      lcd.createChar(ScoreMenu::LCDChars::DownArrow, aux);
    }
  }
}

void RegisterScoreScreen::setup() {
  memset(displayBuffer, ScoreMenu::LCDChars::Blank, sizeof(displayBuffer));

  displayBuffer[2][0] = ScoreMenu::LCDChars::Confirmation;
  displayBuffer[2][1] = ScoreMenu::LCDChars::QuestionMark;
  displayBuffer[8][0] = ScoreMenu::LCDChars::DownArrow;
  displayBuffer[9][0] = ScoreMenu::LCDChars::NameLeft;
  displayBuffer[9][1] = ScoreMenu::LCDChars::NameRight;
  displayBuffer[10][0] = ScoreMenu::LCDChars::TopArrow;
  displayBuffer[12][0] = ScoreMenu::LCDChars::ScoreLeft;
  displayBuffer[12][1] = ScoreMenu::LCDChars::ScoreRight;
}

void RegisterScoreScreen::incrementCharacter(int direction) {
  if (selectedIndex == 4) {
    return;
  }
  charIndex[selectedIndex] += direction;

  if (charIndex[selectedIndex] < 0) {
    charIndex[selectedIndex] = 25;
  } else if (charIndex[selectedIndex] > 25) {
    charIndex[selectedIndex] = 0;
  }

  name[selectedIndex] = charSet[charIndex[selectedIndex]];
}
