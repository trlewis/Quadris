/*
 * MainMenuScreen.cpp
 *
 *  Created on: Sep 19, 2012
 *      Author: econobeing5
 */

#include <string>
#include <cmath>

#include "SDL/SDL.h"

#include "MainMenuScreen.h"
#include "TetrisScreen.h"
#include "BitmapFont.h"

void MainMenuScreen::init()
{
	angle = 0;
	font = new BitmapFont("resources/bluebluewithlayout.png");
	selected_item = 0;
	menu_items.push_back("Start Game");
	menu_items.push_back("Credits");
	menu_items.push_back("Exit");
}

void MainMenuScreen::cleanup()
{
	delete font;
}

void MainMenuScreen::pause()
{

}

void MainMenuScreen::unpause()
{

}

void MainMenuScreen::handleEvents(StateManager* state_manager)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_UP:
					if(selected_item == 0)
						selected_item = menu_items.size()-1;
					else
						selected_item--;
					//angle = 0;
					break;
				case SDLK_DOWN:
					if(selected_item == menu_items.size()-1)
						selected_item = 0;
					else
						selected_item++;
					//angle = 0;
					break;
				case SDLK_ESCAPE:
					state_manager->quit();
					break;
				case SDLK_RETURN:
					handleChoice(state_manager);
					break;
				default:
					break;
			}
		}
		if(e.type == SDL_QUIT)
			state_manager->quit();
	}
}

void MainMenuScreen::update(StateManager* state_manager)
{
	angle += angle_delta;
}

void MainMenuScreen::draw(StateManager* state_manager)
{
	SDL_FillRect(state_manager->screen, &state_manager->screen->clip_rect,0);

	int x = 10 + (int)(std::abs(bounce_size * std::sin(angle)));
	int y = font->getLineSpacing() + font->getHeight();

	for(Uint32 i = 0 ; i < menu_items.size() ; i++)
	{
		if(i == selected_item)
			font->drawString(menu_items[i],state_manager->screen,x,y);
		else
			font->drawString(menu_items[i],state_manager->screen,10,y);

		y += font->getLineSpacing() + font->getHeight();
	}

	SDL_Flip(state_manager->screen);
}

void MainMenuScreen::handleChoice(StateManager* state_manager)
{
	std::string choice = menu_items[selected_item];

	if(choice == "Start Game")
	{
		TetrisScreen* ts = new TetrisScreen();
		ts->init();
		state_manager->pushState((GameState*)ts);
	}
	else if (choice == "Exit")
	{
		state_manager->quit();
	}
}

