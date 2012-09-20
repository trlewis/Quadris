/*
 * Timer.h
 *
 *  Created on: Sep 6, 2012
 *      Author: econobeing
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	int getTicks();

	bool isStarted();
	bool isPaused();
private:
	int startTicks;
	int pausedTicks;
	bool paused;
	bool started;
};


#endif /* TIMER_H_ */
