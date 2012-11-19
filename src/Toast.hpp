/*
 * Toast.hpp
 *
 * Author: Travis Lewis
 * Created on: 20 Sep 2012
 */

#ifndef TOAST_HPP_
#define TOAST_HPP_

#include <string>

#include "SDL/SDL.h"

#include "BitmapFont.hpp"

/** \brief A quick popup message.
 *
 * A Toast is a short string that will be displayed for a short period of time.
 * Similar to a toast in android, or how damage numbers is shown in RPGs. For
 * now it only supports fading out from fully opaque, although the speed at
 * which it fades is customizable.
 */
class Toast
{
public:
	/** \brief Creates a new Toast with specified information.
	 * @param bmf The font to use to draw the message.
	 * @param message The text that is going to be displayed.
	 * @param x The X location to draw the Toast.
	 * @param y The Y location to draw the Toast.
	 * @param dalpha How fast to fade the message. Alpha goes from 255 to 0,
	 * 		  and dalpha gets subtracted from the Toast's current alpha
	 * 		  every time drawToast is called.
	 */
	Toast(BitmapFont* bmf, std::string message, const int x = 0,
		const int y = 0, const unsigned int dalpha = 0);

	/** \brief Set the location of the toast.
	 * @param x The X location to draw the Toast.
	 * @param y The Y location to draw the Toast.
	 */
	void setLocation(const int x, const int y);

	/** \brief Sets the speed/direction the toast will move.
	 *
	 * The x/y position of the Toast will be changed by dx/dy on every call
	 * to drawToast.
	 * @param dx How fast to move the Toast in the X direction.
	 * @param dy How fast to move the Toast in the Y direction.
	 */
	void setMovement(const float dx, const float dy);

	/** \brief Sets the fade speed.
	 *
	 * On every call to drawToast da is subtracted from the Toast's current
	 * alpha, the higher the number the faster it fades. Each Toast's alpha
	 * goes 255 -> 0.
	 * @param da How much to decrease the Toast's alpha every frame.
	 */
	void setFade(unsigned int da);

	/** \brief Gets the Toast's current X position. */
	int getX();

	/** \brief Gets the Toast's current Y position. */
	int getY();

	/** \brief Gets the Toast's speed in the X direction. */
	float getDX();

	/** \brief Gets the Toast's speed in the Y direction. */
	float getDY();

	/** \brief Gets the speed at which the Toast fades. */
	unsigned int getFade();

	/** \brief Gets the number of times drawToast has been called.*/
	int timesDrawn();

	/** \brief Whether or not the Toast's alpha is 0 (transparent) */
	bool isDone();

	/** \brief Draws the toast.
	 *
	 * Draws the Toast on the given surface at the position currently stored
	 * in the Toast.
	 * @param The surface to draw on.
	 */
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
