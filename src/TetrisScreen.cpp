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
#include "Toast.h"

#include "TetrisScreen.h"
#include "TreadmillBoard.h"


void TetrisScreen::init()
{
    blocks.clear();
    //board = TetrisBoard();
    board = TreadmillBoard();

    drop_time = DEFAULT_DROP_TIME;
    drop_delta = DEFAULT_DROP_DIFF;
    frr = FrameRateRegulator();
    drop_timer = Timer();
    drop_timer.start();

    loadBlocks();
    blocksize = blocks.at(0)->w; //get the block size from one of the blocks...

    ghost_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, 4*blocksize,
    	4*blocksize, 32,0,0,0,0);

    SDL_SetAlpha(ghost_surface, SDL_SRCALPHA, 128);
    ghost_mask = SDL_MapRGB(ghost_surface->format, 255, 0 , 255);
    SDL_SetColorKey(ghost_surface, SDL_SRCCOLORKEY, ghost_mask);


    font = new BitmapFont("resources/bluebluewithlayout.png");
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

    if(board.isGameOver())
    {
    	while(SDL_PollEvent(&e))
    	{
    		if(e.type == SDL_KEYDOWN)
    		{
    			if(e.key.keysym.sym == SDLK_ESCAPE)
    				state_manager->quit();
    		}
    		else if(e.type == SDL_QUIT)
    			state_manager->quit();
    	}
    	return;
    }

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
				state_manager->quit(); break;
			case SDLK_SPACE: {
				std::stringstream abc;
				abc << drop_timer.getTicks();
				pushToast(abc.str());
				break; }
			default:
				break;
			}
		}
	}

    //handle board events
    while(board.hasEvents())
    {
    	TetrisBoard::BoardEvent be = board.getEvent();
    	switch(be)
    	{
    	case TetrisBoard::DOUBLE_LINE:
    		pushToast("Double Line"); break;
    	case TetrisBoard::TRIPLE_LINE:
    		pushToast("TRIPLE LINE"); break;
    	case TetrisBoard::QUAD_LINE:
    		pushToast("QUAD LINE!!"); break;
    	case TetrisBoard::LEVEL_UP: {
    		if(board.getLevel() < 15)
    			drop_time -= drop_delta;
    		pushToast("Level Up");
    		Toast* t;
    		t = &my_toasts.at(my_toasts.size()-1);
    		t->setLocation(t->getX(), t->getY() - (font->getHeight() * 2));
    		break; }
    	default:
    		break;
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
    for(unsigned int y = 0 ; y < board_blocks.size() ; y++)
    {
        //for each block in the row.
        for(unsigned int x = 0 ; x < board_blocks[y].size() ; x++)
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

    //TODO: DELETE THIS
//    SDL_Rect frect; frect.x = 350; frect.y = 300;
//    frect.w = 200; frect.h = 400;
//    std::string str = "Test string that i'm writing so that i can see if transparency works without generating another image i'm thinking that it will but we'll see so yeah here goes.";
//    font->drawString(str,state_manager->screen,frect);

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

    //draw toasts
    if(my_toasts.size() > 0)
    {
    	bool isdead;
    	do{
    		isdead = false;
    		for(unsigned int i = 0 ; i < my_toasts.size() ; i++)
    		{
    			if(my_toasts[i].isDone())
    			{
    				isdead = true;
    				std::vector<Toast>::iterator it = my_toasts.begin();
    				it += i;
    				my_toasts.erase(it);
    				break;
    			}
    		}
    	} while(isdead);

    	for(unsigned int i = 0 ; i < my_toasts.size() ; i++)
    		my_toasts[i].drawToast(state_manager->screen);

//		for(std::vector<Toast>::iterator i = my_toasts.begin(),
//			end = my_toasts.end(); i != end; ++i)
//		{
//			if(i->isDone())
//			{
//				my_toasts.erase(i--);
//				continue;
//			}
//			i->drawToast(state_manager->screen);
//		}
    }

    SDL_Flip(state_manager->screen);

    frr.endFrame();
}

void TetrisScreen::loadBlocks()
{
    //std::stringstream ss;

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
        default:
        	break;
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
        yblocks = it->y + 1;
        ypix = offy - (yblocks * blocksize);

        xblocks = it->x;
        xpix = offx + (xblocks * blocksize);

        applySurface(xpix, ypix, blocks[color], dest);
    }
}

void TetrisScreen::pushToast(std::string message)
{
	Toast t(font, message, 0, 300, 6);

	int boardleft = BOARD_OFFSETX * blocksize;
	int boardright = boardleft + (board.getWidth() * blocksize);
	int mwidth = font->getStringWidth(message);
	t.setLocation(((boardleft + boardright)/2) - (mwidth/2),t.getY());
	t.setMovement(0,-1.5);
	my_toasts.push_back(t);
}

