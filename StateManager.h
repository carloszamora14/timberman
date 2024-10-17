#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "BaseScreen.h"

class StateManager {
  private:
    BaseScreen *currentState;
    BaseScreen *previousState;

  public:
    StateManager();
    ~StateManager();

    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

    void changeState(BaseScreen *newState);
    void resumePreviousState();
    void handleInput();
    void update();
};

#endif
