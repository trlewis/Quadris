/*
 * BitmapFont.h
 *
 * Author: Travis Lewis
 * Created on: 6 Sep 2012
 */

#ifndef BITMAPFONT_H_
#define BITMAPFONT_H_

#include "SDL/SDL.h"

/**
 * \brief A font for drawing strings of text using bitmap characters.
 *
 * BitmapFonts process a source image (of whatever formats SDL_image can
 * handle), and allow users to draw strings of text on screen. This object
 * expects the image to have a particular format to successfully process it.
 */
//TODO: add a link to the image^ ...and make the image...
class BitmapFont
{
public:
	// FUNCTIONS

	/**
	 * \brief Creates a BitmapFont from the file given.
	 * @param filename The path to the source image.
	 */
	BitmapFont(std::string filename);

	/**
	 * \brief Draws a linear string on screen.
	 *
	 * Draws a linear (not multi-line) string of text on screen on the surface
	 * given and at the location given.
	 * @param text The text to draw.
	 * @param dest The surface to draw the string on.
	 * @param x The X location to draw at on the given surface.
	 * @param y The Y location to draw at on the given surface.
	 */
	void drawString(std::string text, SDL_Surface* dest,
			const int x, const int y);

	/**
	 * \brief Draws text on a surface in a given area.
	 *
	 * Draws a string of text on the surface using the bounds given. Does not
	 * draw what will not fit within the bounds. Breaks lines on spaces or
	 * certain characters.
	 *
	 * @param text The text to draw.
	 * @param dest The surface to draw the string on.
	 * @param x The left side of the bounding area.
	 * @param y The top side of the bounding area.
	 * @param width The width of the bounding area.
	 * @param height The height of the bounding area.
	 */
	void drawString(std::string text, SDL_Surface* dest, const int x,
			const int y,const int width, const int height);

	/**
	 * \brief Draws text on a surface in a given area.
	 *
	 * Draws a string of text on the surface using the bounds given. Does not
	 * draw what will not fit within the bounds. Breaks lines on spaces or
	 * certain characters.
	 * @param text The text to draw.
	 * @param dest The surface to draw the string on.
	 * @param box The bounding area to draw the text inside of.
	 */
	void drawString(std::string text, SDL_Surface* dest, SDL_Rect& box);

	/**
	 * \brief Gets the width of a linear string of text.
	 * @param text The line of text to measure.
	 * @return The number of pixels wide the drawn string would be.
	 */
	int getStringWidth(std::string text);

	/**
	 * \brief Gets the height of the font.
	 * @return The height of the font in pixels, not including line spacing.
	 */
	int getHeight();

	/** \brief Gets the spacing between lines.
	 * @return The number of pixels between lines, not including font height.
	 */
	int getLineSpacing();

	/**
	 * \brief Gets the spacing between characters.
	 * @return The number of pixels between characters.
	 */
	int getCharSpacing();

	/**
	 * \brief Whether or not the source image is keyed.
	 * @return True if the source image is keyed
	 */
	bool isKeyed();

	/**
	 * \brief Gets the key used with the source image.
	 * @return RGB=(255,0,255) if the image is not keyed.
	 */
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
	bool keyed;			// whether or not the image is keyed
	Uint32 color_key;	// color key used on the image

	// FUNCTIONS

	// does all the dirty work of setting the font up.
	void processImage();

	// finds out how many pixels are between the start of the string and the next
	// breaking character
	int pixelsToWhitespace(std::string text);
};

#endif /* BITMAPFONT_H_ */
