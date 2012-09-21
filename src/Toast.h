/*
 * Toast.h
 *
 * Author: Travis Lewis
 * Created on: 20 Sep 2012
 */

#ifndef TOAST_H_
#define TOAST_H_

#include <string>

#include "SDL/SDL.h"

#include "BitmapFont.h"

/**
 * \brief A quick popup message.
 *
 * A Toast is a short string that will be displayed for a short period of time.
 * Similar to a toast in android, or how damage numbers is shown in RPGs. For
 * now it only supports fading out from fully opaque, although the speed at
 * which it fades is customizable.
 */
class Toast
{
public:
	Toast(BitmapFont* bmf, std::string message, const int x = 0,
		const int y = 0, const unsigned int dalpha = 0);

	void setLocation(const int x, const int y);
	void setMovement(const float dx, const float dy);
	void setFade(unsigned int da);
	int getX();
	int getY();
	float getDX();
	float getDY();
	unsigned int getFade();

	int timesDrawn(); 	//number of times drawToast has been called
	bool isDone();		//whether or not the toast is done (gone invisible)

	void drawToast(SDL_Surface* dest);
private:
	float xpos, ypos; 		//location to draw
	float dxpos, dypos;		//movement per frame in pixels
	std::string text;		//text of toast
	Uint8 alpha;			//current alpha of image
	unsigned int dalpha;	//alpha delta amount to subtract
	int frames;				//times drawn (# times toast called)
	bool done;				//if the image has an alpha of zero
	SDL_Surface* img;		//image to draw
};

#endif /* TOAST_H_ */
