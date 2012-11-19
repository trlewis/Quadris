/*
 * Toast.cpp
 *
 * Author: Travis Lewis
 * Created on: 20 Sep 2012
 */

#include <string>

#include "SDL/SDL.h"

#include "BitmapFont.hpp"
#include "HelperFunctions.hpp"
#include "Toast.hpp"

Toast::Toast(BitmapFont* bmf, std::string message, const int x,
	const int y, const unsigned int d_alpha)
{
	xpos = (float)x;
	ypos = (float)y;
	dxpos = 0;
	dypos = 0;
	text = message;
	alpha = SDL_ALPHA_OPAQUE;
	dalpha = d_alpha;
	frames = 0;
	done = false;

	int img_width = bmf->getStringWidth(message);
	img = SDL_CreateRGBSurface(SDL_SRCALPHA, img_width, bmf->getHeight(), 32,
		0,0,0,0);

	SDL_SetColorKey(img, SDL_SRCCOLORKEY, bmf->getKey());
	SDL_FillRect(img, &img->clip_rect, bmf->getKey());

	bmf->drawString(message, img, 0, 0);
}

void Toast::setLocation(const int x, const int y)
{
	xpos = (float)x;
	ypos = (float)y;
}

void Toast::setMovement(const float dx, const float dy)
{
	dxpos = dx;
	dypos = dy;
}

void Toast::setFade(unsigned int da) { dalpha = da; }


int Toast::getX() { return (int)xpos; }
int Toast::getY() { return (int)ypos; }
float Toast::getDX() { return dxpos; }
float Toast::getDY() { return dypos; }
unsigned int Toast::getFade() { return dalpha; }

int Toast::timesDrawn() { return frames; }
bool Toast::isDone() { return done; }

void Toast::drawToast(SDL_Surface* dest)
{
	if(done)
		return;

	applySurface((int)xpos, (int)ypos, img, dest);
	frames++;
	if(dalpha > alpha)
		done = true;
	else
	{
		xpos += dxpos;
		ypos += dypos;
		alpha -= dalpha;
		SDL_SetAlpha(img, SDL_SRCALPHA, alpha);
	}
}

