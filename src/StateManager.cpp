/*
 * ScreenManager.cpp
 *
 *  Created on: Sep 11, 2012
 *      Author: econobeing
 */

//TODO: delete this and all cout calls when done testing!
#include <iostream>

#include "SDL/SDL.h"

#include "StateManager.h"
#include "GameState.h"

bool StateManager::init(const char* title, int width,int height, int bpp,
		bool full_screen)
{
	if(SDL_Init(SDL_INIT_EVERYTHING == -1))
		return false;

	fullscreen = full_screen;

	screen = NULL;
	if(!fullscreen)
		screen = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);
	else
		screen = SDL_SetVideoMode(width, height, bpp,
				SDL_HWSURFACE|SDL_FULLSCREEN);

	if(screen == NULL)
		return false;

	SDL_WM_SetCaption(title, NULL);
	running = true;

	std::cout << "finished initializing state manager\n";

	return true;
}

void StateManager::cleanup()
{
	while(!states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	if(fullscreen)
		screen = SDL_SetVideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
				DEFAULT_BPP, 0);

	running = false;
	SDL_Quit();
}

void StateManager::changeState(GameState* state)
{
	if(!states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	states.push_back(state);
}

void StateManager::pushState(GameState* state)
{
	if(!states.empty())
		states.back()->pause();

	states.push_back(state);
}

void StateManager::popState()
{
	if(!states.empty())
	{
		states.back()->cleanup();
		states.pop_back();
	}

	if(!states.empty())
		states.back()->unpause();
}

void StateManager::handleEvents()
{
//	std::cout << "handleEvents() starting in StateManager\n";
	states.back()->handleEvents(this);
//	std::cout << "handleEvents() ending in StateManager\n";
}

void StateManager::update()
{
//	std::cout << "update() starting in StateManager, " << "#states=" << states.size() << std::endl;
	states.back()->update(this);
//	std::cout << "update() ending in StateManager\n";
}

void StateManager::draw()
{
//	std::cout << "draw() starting in StateManager\n";
	states.back()->draw(this);
//	std::cout << "draw() ending in StateManager\n";
}

