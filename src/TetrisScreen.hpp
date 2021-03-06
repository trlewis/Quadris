/*
 * TetrisScreen.hpp
 *
 * Author: Travis Lewis
 * Created on: 18 Sep 2012
 */

#ifndef TETRIS_SCREEN_HPP_
#define TETRIS_SCREEN_HPP_

#include "SDL/SDL.h"

#include "HelperFunctions.hpp"

#include "TetrisBoard.hpp"
#include "TreadmillBoard.hpp"

#include "GameState.hpp"
#include "StateManager.hpp"

#include "Timer.h"
#include "FrameRateRegulator.h"

#include "BitmapFont.hpp"
#include "Toast.hpp"

class TetrisScreen: public GameState
{
public:
    void init();
    void cleanup();

    void pause();
    void unpause();

    void handleEvents(StateManager* state_manager);
    void update(StateManager* state_manager);
    void draw(StateManager* state_manager);
private:
	static const int DEFAULT_DROP_TIME    = 1000;
	static const int DEFAULT_DROP_DIFF    = 90;//45;

	static const int IBLOCK=0, OBLOCK=1, TBLOCK=2, SBLOCK=3, ZBLOCK=4,
        JBLOCK=5, LBLOCK=6, GRAYBLOCK=7;

    static const int BOARD_OFFSETX = 7;
    static const int HOLD_OFFSETX = 1;
    static const int HOLD_OFFSETY = 1;
    static const int STATS_OFFSETX = 1;
    static const int NEXT_OFFSETX = 9; //not counting board width
    static const int NEXT_OFFSETY = 2;

    std::vector<SDL_Surface*> blocks;
    int blocksize; //in px, used for layout of graphics
    int drop_time, drop_delta;

    //TODO: figure out why you can't just do (TetrisBoard)TreadmillBoard()
    //		in the cpp file. but instead have to declare board as a
    //		TetrisBoard or TreadmillBoard in the header to get what you want.
    TetrisBoard board; //the actual gameplay stuff
    //TreadmillBoard board;

    Timer drop_timer; //times drops
    FrameRateRegulator frr; //regulates frame rate

    BitmapFont* font;

    SDL_Surface* ghost_surface;
    Uint32 ghost_mask;

    std::vector<Toast> my_toasts;

    void loadBlocks();
    //colors:
    //I=cyan, O=yellow, T=purple, S=green, Z=red, J=blue, L=orange
    int getColor(TetrisPiece::PieceType type);

    //the offsets are in pixels, origin is at bottom left of desired
    //area
    void drawPiece(TetrisPiece* piece, SDL_Surface* dest,
        const int offx, const int offy);

    void pushToast(std::string message);
};

#endif
