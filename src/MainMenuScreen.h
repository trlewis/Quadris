/*
 * MainMenuScreen.h
 *
 *  Created on: Sep 19, 2012
 *      Author: econobeing5
 */

#ifndef MAINMENUSCREEN_H_
#define MAINMENUSCREEN_H_

#include "SDL/SDL.h"

#include "BitmapFont.h"
#include "GameState.h"
#include "StateManager.h"

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

private:
    static const float angle_delta = 0.05;
    static const int bounce_size = 20;

    float angle;
    BitmapFont* font;

    std::vector<std::string> menu_items;
    Uint8 selected_item;

    void handleChoice(StateManager* state_manager);
};

#endif /* MAINMENUSCREEN_H_ */
