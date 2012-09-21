/*
 * GameScreen.h
 *
 *  Created on: Sep 11, 2012
 *      Author: econobeing
 */

#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

#include "StateManager.h"

//NOTE: gamestates could call other gamestates! this could be useful for
//something like a neat moving/animated background for a tetris game!

/** A GameState is a generic object to be used in conjunction with a
 * StateManager. Each GameState could represent opening splash screens,
 * a main menu, a gameplay screen, etc.
 */
class GameState
{
public:
	/** \brief Initializes the GameState. */
	virtual void init() = 0;

	/** \brief Cleans up all the assets the GameState loaded. */
	virtual void cleanup() = 0;


	/** \brief Pauses the GameState. */
	virtual void pause() = 0;

	/** \brief Unpauses the GameState. */
	virtual void unpause() = 0;

	/** \brief Handles events, key presses, closing the window, etc. */
	virtual void handleEvents(StateManager* state_manager) = 0;

	/** \brief Updates the GameState. Sprite movement, etc */
	virtual void update(StateManager* state_manager) = 0;

	/** \brief Draws the GameState to the screen contained in the given
	 * Statemanager.
	 */
	virtual void draw(StateManager* state_manager) = 0;

	/** \brief Changes the current GameState.
	 *
	 * Tells the given StateManager to change it's current state to the
	 * given GameState.
	 */
	void changeState(StateManager* state_manager, GameState* state)
	{ state_manager->changeState(state); }

	//below line gets rid of the warning. but i'm not sure what it's doing...
//	virtual ~GameState(){}
protected:
	bool paused; //!< Whether or not the GameState is paused.
};


#endif /* GAMESCREEN_H_ */
