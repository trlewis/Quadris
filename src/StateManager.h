/*
 * StateManager.h
 *
 * Author: Travis Lewis
 * Created on: 11 Sep 2012
 */

#ifndef SCREENMANAGER_H_
#define SCREENMANAGER_H_

#include <vector>

#include "SDL/SDL.h"

//#include "GameState.h"
//TODO: understand why this line fixes the errors it does.
class GameState;

/**
 * Manages a stack of GameStates. Tells the top state to update, draw, or
 * handle events. Also pushes/pops/changes the stack as its told.
 */
class StateManager
{
public:
	/**
	 * \brief Initializes the StateManager.
	 *
	 * Initializes using the properties passed to it.
	 * @param title The title of the window.
	 * @param width How wide to make the screen in pixels.
	 * @param height How tall to make the screen in pixels.
	 * @param bpp Bits-per-pixel of the screen.
	 * @param full_screen True if the screen should be full.
	 */
	bool init(const char* title, int width = DEFAULT_SCREEN_WIDTH,
			int height = DEFAULT_SCREEN_HEIGHT, int bpp = DEFAULT_BPP,
			bool full_screen = false);

	/**
	 * \brief Cleans up the stack.
	 *
	 * Tells all the states on the stack to clean up their assets and get
	 * ready to be deleted. This should be called after isRunning() returns
	 * false.
	 */
	void cleanup();

	/**
	 * \brief Changes the current state.
	 *
	 * Replaces the current GameState on top of the stack with the given
	 * GameState.
	 * @param state The replacement GameState.
	 */
	void changeState(GameState* state);

	/**
	 * \brief Pushes a state onto the stack.
	 *
	 * Pushes the given state on top of the stack, rather than replacing the
	 * existing top elemtn.
	 * @param state The GameState to push on top.
	 */
	void pushState(GameState* state);

	/** \brief Pops the top GameState off the stack */
	void popState();

	/**
	 * \brief Tells the current GameState to handle events.
	 *
	 * Calls handleEvents() on the GameState currently on top of the stack.
	 */
	void handleEvents();

	/**
	 * \brief Updates the current GameState.
	 *
	 * Calls update() on the GameState currently on top of the stack.
	 */
	void update();

	/**
	 * \brief Tells the current GameState to draw itself.
	 *
	 * Calls draw() on the GameState currently on top of the stack.
	 */
	void draw();

	/**
	 * \brief Whether or not the StateManager is running.
	 *
	 * @return False if cleanup() or quit() has been called.
	 */
	bool isRunning() { return running; }

	/**
	 * \brief Shuts down the StateManager.
	 *
	 * This should be used before calling cleanup(), and as a way for a
	 * GameState to tell the StateManager that the program should get
	 * ready to stop.
	 */
	void quit() { running = false; }

	/** The screen that the states will use to draw on. **/
	SDL_Surface* screen;
private:
	// FIELDS
	static const int DEFAULT_SCREEN_WIDTH = 800;
	static const int DEFAULT_SCREEN_HEIGHT = 600;
	static const int DEFAULT_BPP = 32;

	std::vector<GameState*> states;
	bool running;
	bool fullscreen;	//not sure if this is necessary...
};


#endif /* SCREENMANAGER_H_ */
