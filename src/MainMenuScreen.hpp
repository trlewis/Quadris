/*
 * MainMenuScreen.hpp
 *
 * Author: Travis Lewis
 * Created on: 19 Sep 2012
 */

#ifndef MAINMENUSCREEN_HPP_
#define MAINMENUSCREEN_HPP_

#include "SDL/SDL.h"

#include "BitmapFont.hpp"
#include "GameState.hpp"
#include "StateManager.hpp"
#include "FrameRateRegulator.h"

class MainMenuScreen: public GameState
{
public:
    void init();
    void cleanup();

    void pause();
    void unpause();

    void handleEvents(StateManager* state_manager);
    void update(StateManager* state_manager);
    void draw(StateManager* state_manager);

    virtual ~MainMenuScreen() {}

private:
    static const float angle_delta = 0.10;
    static const int bounce_size = 20;

    float angle;
    BitmapFont* font;
    FrameRateRegulator frr;

    std::vector<std::string> menu_items;
    Uint8 selected_item;

    void handleChoice(StateManager* state_manager);
};

#endif /* MAINMENUSCREEN_H_ */
