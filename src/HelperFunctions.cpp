/*
 * Helperfunctions.cpp
 *
 * Author: Travis Lewis
 * Created on: 6 Sep 2012
 */

#include <iostream>
#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "HelperFunctions.h"


SDL_Surface* loadImage(std::string filename)
{
	SDL_Surface* loadedImg = NULL;
	SDL_Surface* optimizedImg = NULL;
	loadedImg = IMG_Load(filename.c_str());

	if(loadedImg != NULL){
		optimizedImg = SDL_DisplayFormat(loadedImg);

		std::cout << "found: " + filename << std::endl;
	}
	else
		std::cout << "could not find: " + filename << std::endl;

	SDL_FreeSurface(loadedImg);
	return optimizedImg;
}

bool colorKeyImg(SDL_Surface* img, const Uint8 r, const Uint8 g, const Uint8 b)
{
	if(img == NULL)
		return false;

	Uint32 colorkey = SDL_MapRGB(img->format, r, g, b);
	SDL_SetColorKey(img, SDL_SRCCOLORKEY, colorkey);

	return true;
}

void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* dest)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source, NULL, dest, &offset);
}

void clearScreen(SDL_Surface* screen)
{
	SDL_FillRect(screen, &screen->clip_rect,
			SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
}

void clearScreen(SDL_Surface* screen, const Uint8 r, const Uint8 g,
		const Uint8 b)
{
	SDL_FillRect(screen, &screen->clip_rect,
			SDL_MapRGB(screen->format, r, g, b));
}

Uint32 getPixel32(SDL_Surface* img, const int x, const int y)
{
	Uint32* pixels = (Uint32*)img->pixels;
	return pixels[(y * img->w) + x];
}

