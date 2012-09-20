/**
 * Created by: Travis Lewis
 * Date: 18 Sep 2012
 *
 */

#include <iostream> //TODO: delete this when done!
#include <vector>
#include <sstream>

#include "SDL/SDL.h"

#include "HelperFunctions.h"
#include "BitmapFont.h"

#include "TetrisScreen.h"



void TetrisScreen::init()
{
    blocks.clear();
    board = TetrisBoard();

    drop_time = DEFAULT_DROP_TIME;
    drop_delta = DEFAULT_DROP_DIFF;
    frr = FrameRateRegulator();
    drop_timer = Timer();
    drop_timer.start();

    loadBlocks();
    blocksize = blocks.at(0)->w; //get the block size from one of the blocks...

    Uint32 rmask ,gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x00000000;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0x00000000;
    #endif
    //^use 0 for the amask so the entire ghost surface will be alpha..tized

    ghost_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, 4*blocksize,
        4*blocksize, 32, rmask, gmask, bmask, amask);
    SDL_SetAlpha(ghost_surface, SDL_SRCALPHA, SDL_ALPHA_OPAQUE/2);
    ghost_mask = SDL_MapRGB(ghost_surface->format, 255, 0 , 255);
    SDL_SetColorKey(ghost_surface, SDL_SRCCOLORKEY, ghost_mask);


    font = new BitmapFont("resources/bluebluewithlayout.png");
    //start timer at the end here.
}

void TetrisScreen::cleanup()
{
    while(!blocks.empty())
    {
        SDL_FreeSurface(*blocks.end());
        blocks.pop_back();
    }
}

void TetrisScreen::pause()
{
    drop_timer.pause();
}

void TetrisScreen::unpause()
{
    frr.startFrame();
    drop_timer.unpause();
}


void TetrisScreen::handleEvents(StateManager* state_manager)
{
    SDL_Event e;
    if(!board.isGameOver())
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        board.moveLeft(); break;
                    case SDLK_RIGHT:
                        board.moveRight(); break;
                    case SDLK_DOWN:
                        board.moveDown(); break;
                    case SDLK_UP:
                        board.hardDrop(); break;
                    case SDLK_z:
                        board.rotateCCW(); break;
                    case SDLK_x:
                        board.rotateCW(); break;
                    case SDLK_a:
                        board.holdPiece(); break;
                    case SDLK_ESCAPE:
                    	state_manager->popState(); break;
                        //state_manager->quit(); break;
                    default:
                        break;
                }
            }
            //TODO: delete this!
//            std::cout << board.toString() << "\n\n";
//            std::cout << board.getStats() << "\n";
        }
    }
    else
    {
    	while(SDL_PollEvent(&e))
    	{
    		if(e.type == SDL_KEYDOWN)
    		{
    			if(e.key.keysym.sym == SDLK_ESCAPE)
    				//state_manager->quit();
    				state_manager->popState();
    		}
    		else if(e.type == SDL_QUIT)
    			//state_manager->quit();
    			state_manager->popState();
    	}
    }
}

void TetrisScreen::update(StateManager* state_manager)
{
//	std::cout << "timer ticks: " << drop_timer.getTicks() << std::endl;
//	std::cout << "drop_time=" << drop_time << std::endl;
	if(drop_timer.getTicks() >= drop_time && !board.isGameOver())
	{
		board.moveDown();
		drop_timer.start();
	}
}

void TetrisScreen::draw(StateManager* state_manager)
{
	frr.startFrame();

    SDL_FillRect(state_manager->screen, &state_manager->screen->clip_rect,0);

    Uint32 gray = SDL_MapRGB(state_manager->screen->format, 35,35,35);
    SDL_Rect rect;

    rect.x = BOARD_OFFSETX * blocksize;
    rect.y = 0;
    rect.w = board.getWidth() * blocksize;
    rect.h = (board.getHeight() - TetrisBoard::NUM_HIDDEN_ROWS) * blocksize;
    SDL_FillRect(state_manager->screen, &rect, gray);

    //draw the board
    std::vector<std::vector<TetrisPiece::PieceType> > board_blocks;
    board_blocks = board.getBoard();
    int xblocks=0, yblocks=0; //blocks
    int xpix=0, ypix=0; //pixels

    //for each row
    for(Uint32 y = 0 ; y < board_blocks.size() ; y++)
    {
        //for each block in the row.
        for(Uint32 x = 0 ; x < board_blocks[y].size() ; x++)
        {
            if(getColor(board_blocks[y][x]) < 0)
                continue;
            else
            {
                yblocks = board.getHeight() - y - 1 -
                    TetrisBoard::NUM_HIDDEN_ROWS; // in blocks
                ypix = yblocks * blocksize;

                xblocks = BOARD_OFFSETX + x; //in blocks
                xpix = xblocks * blocksize;

                applySurface(xpix, ypix, blocks[getColor(board_blocks[y][x])],
                             state_manager->screen);
            }
        }
    }

    //draw the hold piece
    TetrisPiece* temp = board.getHold();
    std::vector<Point> pblocks;

    rect.x = blocksize; rect.y = blocksize;
    rect.w = TetrisPiece::BLOCKS_PER_PIECE * blocksize;
    rect.h = rect.w;
    SDL_FillRect(state_manager->screen, &rect, gray);

    if(temp->getPieceType() != TetrisPiece::EMPTY_PIECE)
        drawPiece(temp, state_manager->screen, blocksize,blocksize*5);


    //draw the current piece
    temp = board.getActivePiece();

    yblocks = board.getHeight() - temp->getLocation().y - TetrisBoard::NUM_HIDDEN_ROWS;
    xblocks = BOARD_OFFSETX + temp->getLocation().x;
    drawPiece(temp, state_manager->screen,
        xblocks * blocksize, yblocks * blocksize);

    pblocks.clear();
    pblocks = temp->getBlocks();


    //draw the ghost piece
    temp = board.getGhost();
    pblocks = temp->getBlocks();

    SDL_FillRect(ghost_surface, &ghost_surface->clip_rect, ghost_mask);
    yblocks = board.getHeight() - temp->getLocation().y - 1 - 3 - TetrisBoard::NUM_HIDDEN_ROWS;
    xblocks = BOARD_OFFSETX + temp->getLocation().x;
    drawPiece(temp, ghost_surface, 0, ghost_surface->h);
    applySurface(xblocks*blocksize,yblocks*blocksize,ghost_surface,state_manager->screen);


    //draw the next pieces
    rect.x = blocksize * (NEXT_OFFSETX + board.getWidth());
    rect.y = blocksize * NEXT_OFFSETY;
    rect.w = blocksize * TetrisPiece::BLOCKS_PER_PIECE;
    rect.h = 3 * blocksize * (TetrisPiece::BLOCKS_PER_PIECE + 1);
    SDL_FillRect(state_manager->screen, &rect, gray);

    std::vector<TetrisPiece*> bag = board.getBag();
    xblocks = board.getWidth() + NEXT_OFFSETX;
    for(int i = 0 ; i < 3 ; i++)
    {
        yblocks = NEXT_OFFSETY + (4*(i+1)) + i;
        drawPiece(bag.at(i),state_manager->screen,xblocks*blocksize,yblocks*blocksize);
    }


    //draw the score/lines/level
    std::stringstream ss;
    rect.x = blocksize;
    rect.y = 2*blocksize + 4*blocksize;
    ss << "Lines: " << board.getLines();
    font->drawString(ss.str(), state_manager->screen, rect.x,rect.y);
    ss.str("");

    rect.y += font->getLineSpacing() + font->getHeight();
    ss << "Score: " << board.getScore();
    font->drawString(ss.str(),state_manager->screen,rect.x,rect.y);
    ss.str("");

    rect.y += font->getLineSpacing() + font->getHeight();
    ss << "Level: " << board.getLevel();
    font->drawString(ss.str(),state_manager->screen,rect.x,rect.y);

    SDL_Flip(state_manager->screen);

    frr.endFrame();
}

void TetrisScreen::loadBlocks()
{
    std::stringstream ss;

    blocks.push_back(loadImage("resources/block_cyan.png")); // I
    blocks.push_back(loadImage("resources/block_yellow.png")); // O
    blocks.push_back(loadImage("resources/block_purple.png")); // T
    blocks.push_back(loadImage("resources/block_green.png")); // S
    blocks.push_back(loadImage("resources/block_red.png")); // Z
    blocks.push_back(loadImage("resources/block_blue.png")); // J
    blocks.push_back(loadImage("resources/block_orange.png")); // L
    blocks.push_back(loadImage("resources/block_gray.png")); // gray
}

int TetrisScreen::getColor(TetrisPiece::PieceType type)
{
    switch(type)
    {
        case TetrisPiece::I:
            return IBLOCK; break;
        case TetrisPiece::O:
            return OBLOCK; break;
        case TetrisPiece::T:
            return TBLOCK; break;
        case TetrisPiece::S:
            return SBLOCK; break;
        case TetrisPiece::Z:
            return ZBLOCK; break;
        case TetrisPiece::J:
            return JBLOCK; break;
        case TetrisPiece::L:
            return LBLOCK; break;
//        default:
//            return GRAYBLOCK; break;
    }
    return -1;
}

void TetrisScreen::drawPiece(TetrisPiece* piece, SDL_Surface* dest,
    const int offx, const int offy)
{
    std::vector<Point> pblocks = piece->getBlocks();

    int yblocks, xblocks, ypix, xpix;
    int color = getColor(piece->getPieceType());

    for(std::vector<Point>::iterator it = pblocks.begin(),
        end = pblocks.end() ; it != end ; ++it)
    {
        //yblocks = 3 - it->y;
        yblocks = it->y + 1;
        ypix = offy - (yblocks * blocksize);

        xblocks = it->x;
        xpix = offx + (xblocks * blocksize);

        applySurface(xpix, ypix, blocks[color], dest);
    }
}

