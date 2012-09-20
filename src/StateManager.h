/*
 * ScreenManager.h
 *
 *  Created on: Sep 11, 2012
 *      Author: econobeing
 */

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include <vector>

#include "SDL/SDL.h"

//#include "GameState.h"
//TODO: understand why this line fixes the errors it does.
class GameState;

class StateManager
{
public:
	bool init(const char* title, int width = DEFAULT_SCREEN_WIDTH,
			int height = DEFAULT_SCREEN_HEIGHT, int bpp = DEFAULT_BPP,
			bool full_screen = false);

	void cleanup();	//get rid of everything and exit the program

	//changestate() replaces the current state on top of the states stack
	void changeState(GameState* state);
	void pushState(GameState* state);
	void popState();

	void handleEvents();
	void update();
	void draw();

	bool isRunning() {return running; }
	void quit() {running = false; }

	SDL_Surface* screen;
private:
	// ENUMS
//	enum GameState { UNINITIALIZED, SHOWING_SPLASH, SHOWING_MENU, PLAYING,
//		PAUSED, EXITING};

	// FIELDS
	static const int DEFAULT_SCREEN_WIDTH = 800;
	static const int DEFAULT_SCREEN_HEIGHT = 600;
	static const int DEFAULT_BPP = 32;

	//static GameState current_state;
	std::vector<GameState*> states;
	bool running;
	bool fullscreen;
};


#endif /* SCREENMANAGER_H_ */
