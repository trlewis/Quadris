/*
 * BitmapFont.cpp
 *
 * Author: Travis Lewis
 * Version: 7 Sep 2012
 */

//TODO: delete this when done! (and all cout calls)
#include <iostream>

#include <string>

#include "SDL/SDL.h"

#include "BitmapFont.hpp"
#include "HelperFunctions.hpp"

const std::string BitmapFont::WHITESPACE_CHARS = " -/\r\n";

BitmapFont::BitmapFont(std::string filename)
{
	source_img = loadImage(filename);

	if(source_img == NULL)
		std::cout << "bitmap source file '" << filename << "' not found\n";

	color_key = SDL_MapRGB(source_img->format, 255, 0, 255);

	processImage();
}

void BitmapFont::drawString(std::string text, SDL_Surface* dest,
		const int x, const int y)
{
	SDL_Rect loc;	//location to draw
	loc.x = x;
	loc.y = y;
	int letter = 0;	// the ascii value of the letter to be drawn

	for(Uint32 i = 0 ; i < text.size() ; i++)
	{
		letter = (int)text[i];
		SDL_BlitSurface(source_img, &letters[letter], dest, &loc);

		loc.x += char_space;
		loc.x += letters[letter].w;

		// if the next char will be off the surface, quit drawing
		if(loc.x > dest->w)
			break;
	}
}

void BitmapFont::drawString(std::string text, SDL_Surface* dest, const int x,
		const int y,const int width, const int height)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	drawString(text, dest, rect);
}

void BitmapFont::drawString(std::string text, SDL_Surface* dest,
		SDL_Rect& box)
{
	SDL_Rect loc;	// where to draw the letter
	loc.x = box.x;
	loc.y = box.y;

	int letter = 0;	// the ascii value of the letter to be drawn

	for(Uint32 i = 0 ; i < text.size() ; i++)
	{
		letter = (int)text[i];

		//if the current word is too long to fit within the width,
		//break to a new line.
		if(pixelsToWhitespace(text.substr(i)) > (box.w - (loc.x - box.x)))
		{
			loc.x = box.x;
			loc.y += source_img->h;
			loc.y += line_space;
		}

		//if the current character is a line break char (\r or \n),
		//then break to a new line
		if(text[i] == '\r' || text[i] == '\n')
		{
			loc.x = box.x;
			loc.y += source_img->h;
			loc.y += line_space;
			continue; //if it isn't visible, don't bother drawing it
		}

		if((loc.y + source_img->h) > (box.y + box.h))
			break;

		//draw the character
		SDL_BlitSurface(source_img, &letters[letter], dest, &loc);

		//update position
		loc.x += (letters[letter].w + char_space);
	}
}

int BitmapFont::getStringWidth(std::string text)
{
	int width=0;
	int letter=0;//ascii value of the letter

	for(Uint32 i = 0 ; i < text.size() ; i++)
	{
		letter = (int)text[i];
		width += letters[letter].w;
	}
	width += (char_space * text.size()); //hopefully that's a little faster

	return width;
}

int BitmapFont::getHeight() { return source_img->h; }
int BitmapFont::getLineSpacing() { return line_space; }
int BitmapFont::getCharSpacing() { return char_space; }
bool BitmapFont::isKeyed() { return keyed; }
Uint32 BitmapFont::getKey() { return color_key; }

void BitmapFont::processImage()
{
	Uint32 delim_color = getPixel32(source_img, 0, 0);
	Uint32 color;
	int left = START_PIXEL;
	int count = 0;	// used for accessing letters[]

	for(int x = START_PIXEL ; x < source_img->w ; x++)
	{
		color = getPixel32(source_img, x, 0);
		if(color == delim_color && count < NUM_ASCII_CHARS)
		{
			letters[count].x = left;
			letters[count].y = 0;
			letters[count].w = x - left;
			letters[count].h = source_img->h;

			left = x+1;
			count++;
		}

		if(count >= NUM_ASCII_CHARS)
			break;
	}

	Uint8 red = 0;
	Uint8 green = 0;
	Uint8 blue = 0;

	color = getPixel32(source_img, 0, 1);
	SDL_GetRGB(color, source_img->format, &red, &green, &blue);

	char_space = red;
	line_space = green;

	if(blue > 0)
	{
		keyed = true;
		color_key = getPixel32(source_img, 0, 2);
		SDL_SetColorKey(source_img, SDL_SRCCOLORKEY, color_key);
	}
}

int BitmapFont::pixelsToWhitespace(std::string text)
{
	int pixels = 0;
	int letter = 0;	//ascii value of the current letter in the string

	for(Uint32 i = 0 ; i < text.size() ; i++)
	{
		letter = (int)text[i];
		pixels += (letters[letter].w + char_space);
		if(WHITESPACE_CHARS.find(text[i]) != std::string::npos)
			break;
	}
	return pixels;
}

