/*
 * FrameRateRegulator.h
 *
 *  Created on: Sep 6, 2012
 *      Author: econobeing
 */

#ifndef FRAMERATEREGULATOR_H_
#define FRAMERATEREGULATOR_H_

#include "Timer.h"

class FrameRateRegulator: protected Timer
{
public:

	FrameRateRegulator() { init(DEFAULT_FPS);}
	FrameRateRegulator(const int fps) {init(fps);}
    const static int DEFAULT_FPS = 60;

	void startFrame() { this->start(); }
	void endFrame()
	{
		if(getTicks() < (1000/fps))
			SDL_Delay((1000/fps) - getTicks());
		frame++;
	}

	int getFrame() { return frame; }

	int getCap() {return fps;}

	void setCap(const int fps)
	{
		if(fps > 0)
			this->fps = fps;
	}

private:

	int fps;
	int frame;

	void init(const int fps)
	{
		this->fps = fps;
		frame = 0;
	}
};


#endif /* FRAMERATEREGULATOR_H_ */
