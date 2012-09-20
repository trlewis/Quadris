/*
 * HelperFunctions.h
 *
 *  Created on: Sep 6, 2012
 *      Author: econobeing
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include <string>

SDL_Surface* loadImage(std::string filename);

bool colorKeyImg(SDL_Surface* img, const Uint8 r,
		const Uint8 g, const Uint8 b);

void applySurface(int x, int y, SDL_Surface* source, SDL_Surface* dest);

void clearScreen(SDL_Surface* screen);

void clearScreen(SDL_Surface* screen, const Uint8 r, const Uint8 g,
		const Uint8 b);

Uint32 getPixel32(SDL_Surface* img, const int x, const int y);

#endif /* HELPERFUNCTIONS_H_ */
