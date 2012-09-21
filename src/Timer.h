/*
 * Timer.h
 *
 * Author: Travis Lewis
 * Created on: 6 Sep 2012
 *
 * Credit goes to Lazy Foo, this source came directly from the timer tutorial.
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "SDL/SDL.h"

/**
 * \brief A simple Timer that will tell you the time since it was started. Also
 * features a pause/unpause feature.
 *
 * Credit goes to Lazy Foo, this source came directly from the timer tutorial.
 */
class Timer
{
public:

	/**
	 * \brief Creates a new timer, does not start it.
	 */
	Timer()
	{
		startTicks = 0;
		pausedTicks = 0;
		paused = false;
		started = false;
	}

	/** \brief Starts the timer. */
	void start()
	{
		started = true;
		paused = false;
		startTicks = SDL_GetTicks();
	}

	/** \brief Stops the timer.
	 *
	 * Cannot be unpaused after this is called. start() is the only way to get
	 * the Timer to start keeping track of ticks again.
	 */
	void stop()
	{
		started = false;
		paused = false;
	}

	/** \brief Pauses the timer. */
	void pause()
	{
		if(started && !paused)
		{
			paused = true;
			pausedTicks = SDL_GetTicks() - startTicks;
		}
	}

	/** \brief Unpauses the timer. */
	void unpause()
	{
		if(paused)
		{
			paused = false;
			startTicks = SDL_GetTicks() - pausedTicks;
			pausedTicks = 0;
		}
	}

	/**
	 * \brief Gets time since start() was called.
	 *
	 * @return The number of milliseconds since start() was called.
	 */
	int getTicks()
	{
		if(started)
		{
			if(paused)
				return pausedTicks;
			else
				return SDL_GetTicks() - startTicks;
		}
		return 0; //return 0 if Timer isn't running.
	}

	/** Gets whether or not the Timer has been started. */
	bool isStarted() { return started; }

	/** Gets whether or not the Timer is currently paused. */
	bool isPaused() { return paused; }
private:
	int startTicks;		//!< milliseconds since start() was called
	int pausedTicks;	//!< milliseconds Timer has been paused
	bool paused;		//!< true if Timer is paused.
	bool started;		//!< true if Timer has been started.
};


#endif /* TIMER_H_ */
