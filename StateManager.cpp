#include "StateManager.h"

StateManager::StateManager()
    : currentState(nullptr), previousState(nullptr), lastDebounceTime(0), debounceDelay(500) {}

StateManager::~StateManager() {
  delete currentState;
  delete previousState;
}

void StateManager::changeState(BaseScreen *newState) {
  if (currentState != nullptr) {
    delete previousState;
    previousState = currentState;
  }
  currentState = newState;
  currentState->display();
}

void StateManager::resumePreviousState() {
  if (previousState != nullptr) {
    delete currentState;
    currentState = previousState;
    previousState = nullptr;
    currentState->resume();
  }
}

void StateManager::handleInput() {
  if (currentState) {
    currentState->handleInput();
  }
}

void StateManager::update() {
  if (currentState) {
    currentState->update();
  }
}
