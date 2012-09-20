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
class GameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void unpause() = 0;

	virtual void handleEvents(StateManager* state_manager) = 0;
	virtual void update(StateManager* state_manager) = 0;
	virtual void draw(StateManager* state_manager) = 0;

	void changeState(StateManager* state_manager, GameState* state)
	{ state_manager->changeState(state); }

	//below line gets rid of the warning. but i'm not sure what it's doing...
//	virtual ~GameState(){}
protected:
	bool paused;
};


#endif /* GAMESCREEN_H_ */
