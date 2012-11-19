
#include "StateManager.hpp"
#include "TetrisScreen.hpp"
#include "MainMenuScreen.hpp"

int main(int argc, char* args[])
{
    StateManager sm;
    sm.init("Quadris! Awesome!!");

//    TetrisScreen* ts = new TetrisScreen();
//    ts->init();
//    sm.changeState((GameState*)ts);
    MainMenuScreen* mms = new MainMenuScreen();
    mms->init();
    sm.changeState((GameState*)mms);

    while(sm.isRunning())
    {
        sm.handleEvents();
        sm.update();
        sm.draw();
    }

    sm.cleanup();
    return 0;
}
