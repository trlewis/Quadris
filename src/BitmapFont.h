/*
 * BitmapFont.h
 *
 *  Created on: Sep 6, 2012
 *      Author: econobeing
 */

#ifndef BITMAPFONT_H_
#define BITMAPFONT_H_

#include "SDL/SDL.h"

class BitmapFont
{
public:
	// FUNCTIONS
	BitmapFont(std::string filename);

	void drawString(std::string text, SDL_Surface* dest,
			const int x, const int y);

	void drawString(std::string text, SDL_Surface* dest, const int x,
			const int y,const int width, const int height);

	void drawString(std::string text, SDL_Surface* dest, SDL_Rect& box);

	int getStringWidth(std::string text);

	int getHeight();        //height of the font in px
	int getLineSpacing();   //px between lines (not including font size)
	int getCharSpacing();   //px between characters.
	bool isKeyed();
	Uint32 getKey();	//returns (255,0,255) if not keyed
private:
	// FIELDS
	static const int START_PIXEL = 3;
	static const int NUM_ASCII_CHARS = 256;
	static const std::string WHITESPACE_CHARS;	//declared in .cpp file

	SDL_Surface* source_img;
	SDL_Rect letters[NUM_ASCII_CHARS];

	int line_space;		// pixels between lines
	int char_space;		// pixels between characters
	bool keyed;
	Uint32 color_key;
	// we don't need a field for word spacing since a space is actually       s
	// a character, which has its width defined in the source image.

	// FUNCTIONS
	void processImage();
	int pixelsToWhitespace(std::string text);
};

#endif /* BITMAPFONT_H_ */
