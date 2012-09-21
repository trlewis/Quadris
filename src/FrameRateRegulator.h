/*
 * FrameRateRegulator.h
 *
 * Author: Travis Lewis
 * Created on: 6 Sep 2012
 */

#ifndef FRAMERATEREGULATOR_H_
#define FRAMERATEREGULATOR_H_

#include "Timer.h"

/**
 * \brief A simple Timer for regulating frame rate.
 *
 * Simply specify the FPS you would like to use, call startFrame() at the
 * beginning of each game cycle, and endFrame() at the end of each cycle.
 */
class FrameRateRegulator: protected Timer
{
public:
	/**
	 * \brief Creates a FrameRateRegulator with the default FPS of 60. */
	FrameRateRegulator() { init(DEFAULT_FPS);}

	/**
	 * \brief Creats a FrameRateRegulator with a given FPS.
	 * @param fps The FPS to regulate.
	 */
	FrameRateRegulator(const int fps) {init(fps);}

	/** The default FPS FrameRateRegulator uses. */
    const static int DEFAULT_FPS = 60;

    /**
     * \brief Starts the timer for the current frame. */
	void startFrame() { this->start(); }

	/**
	 * \brief Ends the current frame.
	 *
	 * If this is called before the projected amount of time each frame is
	 * regulated at passes, SDL_Delay() is called with the remaining amount.
	 */
	void endFrame()
	{
		if(getTicks() < (1000/fps))
			SDL_Delay((1000/fps) - getTicks());
		frame++;
	}

	/**
	 * \brief Gets the current frame, increments on every call to
	 * endFrame(). */
	int getFrame() { return frame; }

	/** \brief Gets the current FPS the FrameRateRegulator is set to. */
	int getCap() {return fps;}

	/** \brief Sets the FrameRateRegulator to use the given FPS. */
	void setCap(const int fps)
	{
		if(fps > 0)
			this->fps = fps;
	}

private:
	int fps;	// frames per second.
	int frame;	// how many frames have been regulated.

	//constructors call this.
	void init(const int fps)
	{
		this->fps = fps;
		frame = 0;
	}
};


#endif /* FRAMERATEREGULATOR_H_ */
