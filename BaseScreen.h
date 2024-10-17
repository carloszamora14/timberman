#ifndef BASE_SCREEN_H
#define BASE_SCREEN_H

class BaseScreen {
  public:
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void display() = 0;
    virtual void resume() = 0;
    virtual ~BaseScreen() {}
};

#endif
