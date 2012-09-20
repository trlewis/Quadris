/*
 * TetrisBoard.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: econobeing
 */

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include <iostream>

#include <algorithm>

#include "TetrisBoard.h"


TetrisBoard::TetrisBoard()
{
	init(DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT);
}

TetrisBoard::TetrisBoard(const int width, const int height)
{
	init(width, height);
}

void TetrisBoard::moveLeft()
{
    TetrisPiece* test = my_piece->moveLeft();
    if(isWithinBounds(test) && !isOverlap(test))
    {
        my_piece = test;
        handleGhost();
        stat_left++;
    }
}

void TetrisBoard::moveRight()
{
    TetrisPiece* test = my_piece->moveRight();
    if(isWithinBounds(test) && !isOverlap(test))
    {
        my_piece = test;
        handleGhost();
        stat_right++;
    }
}

void TetrisBoard::moveDown()
{
    TetrisPiece* test = my_piece->moveDown();
    if(!isWithinBounds(test) || isOverlap(test))
    {
        placePiece();
    }
    else
        my_piece = test;

    handleGhost();
    stat_down++;
}

void TetrisBoard::hardDrop()
{
    int y = my_piece->getLocation().y;
    do
    {
        moveDown();
        stat_down--;
    }while (my_piece->getLocation().y < y);
    stat_hd++;
}

void TetrisBoard::rotateCCW()
{
    TetrisPiece* test = my_piece->rotateCCW();
    bool can_rotate = false;

    if(isWithinBounds(test))
    {
        if(isOverlap(test))
        {
            test = test->moveUp();
            if(!isOverlap(test) && isWithinBounds(test))
            	can_rotate = true;
        }
        else
        	can_rotate = true;
    }

    if(can_rotate)
    {
    	my_piece = test;
    	stat_ccw++;
    	my_events.push_back(TetrisBoard::ROTATE);
    }

//    if(isWithinBounds(test))
//    {
//        if(isOverlap(test))
//        {
//            test = test->moveUp();
//            if(!isOverlap(test) && isWithinBounds(test))
//            {
//            	my_piece = test;
//            	stat_ccw++;
//            }
//        }
//        else
//        {
//        	my_piece = test;
//        	stat_ccw++;
//        }
//    }
    handleGhost();
}

void TetrisBoard::rotateCW()
{
    TetrisPiece* test = my_piece->rotateCW();

    bool can_rotate = false;

    if(isWithinBounds(test))
    {
        if(isOverlap(test))
        {
            test = test->moveUp();
            if(!isOverlap(test) && isWithinBounds(test))
            	can_rotate = true;
        }
        else
        	can_rotate = true;
    }

    if(can_rotate)
    {
    	my_piece = test;
    	stat_cw++;
    	my_events.push_back(TetrisBoard::ROTATE);
    }

//    if(isWithinBounds(test))
//    {
//        if(isOverlap(test))
//        {
//            test = test->moveUp();
//            if(!isOverlap(test) && isWithinBounds(test))
//            {
//            	my_piece = test;
//            	stat_cw++;
//            }
//        }
//        else
//        {
//        	my_piece = test;
//        	stat_cw++;
//        }
//    }
    handleGhost();
}

void TetrisBoard::holdPiece()
{
    if(!held)
    {
        if(my_hold_piece->getPieceType() != TetrisPiece::EMPTY_PIECE)
        {
            TetrisPiece* temp = my_piece;
            my_piece = my_hold_piece;
            my_hold_piece = temp;
            my_piece = my_piece->setLocation(board_width/2, board_height-1);
            handleGhost();
            held = true;
        }
        else
        {
            my_hold_piece = my_piece;
            newPiece();
        }
        stat_holds++;
        my_events.push_back(TetrisBoard::HOLD);
    }
}

int TetrisBoard::getScore() { return score; }
int TetrisBoard::getLevel() { return level; }
int TetrisBoard::getLines() { return lines; }
int TetrisBoard::getWidth() { return board_width; }
int TetrisBoard::getHeight() { return board_height; }
bool TetrisBoard::isGameOver() { return game_over; }

std::vector<std::vector<TetrisPiece::PieceType> > TetrisBoard::getBoard()
{
    std::vector<std::vector<TetrisPiece::PieceType> > copy;
    copy.clear();


    //for each row
    for(std::vector<std::vector<TetrisPiece::PieceType>* >::iterator y
        = my_board.begin(), yend = my_board.end() ; y != yend ; ++y)
    {
        std::vector<TetrisPiece::PieceType> row;
        //for each block in the row
        for(std::vector<TetrisPiece::PieceType>::iterator x = (*y)->begin(),
            xend = (*y)->end(); x != xend ; ++x)
        {
            row.push_back(*x);
        }

        copy.push_back(row);
    }

    return copy;
}

TetrisPiece* TetrisBoard::getHold() { return my_hold_piece->getCopy(); }

TetrisPiece* TetrisBoard::getGhost() { return my_ghost->getCopy(); }

TetrisPiece* TetrisBoard::getActivePiece() { return my_piece->getCopy(); }

std::vector<TetrisPiece*> TetrisBoard::getBag()
{
    std::vector<TetrisPiece*> copy;
    copy.clear();

    for(std::vector<TetrisPiece*>::iterator it = my_bag.begin(),
        end = my_bag.end() ; it != end ; ++it)
    {
        copy.push_back((*it)->getCopy());
    }
    return copy;
}

std::string TetrisBoard::toString()
{
    std::stringstream ss;

    std::vector<Point> pieceblocks = my_piece->getBlocks();
    std::vector<Point> ghostblocks = my_ghost->getBlocks();

    bool found;
    //for each row
    for(int y = board_height - 2 ; y >= 0 ; y--)
    {
        //for each cell in the row
        for(int x = 0 ; x < board_width ; x++)
        {
            found = false;
            //if anything on the board is in this position
            if(my_board.at(y)->at(x) != TetrisPiece::EMPTY_PIECE)
            {
                ss << "X";
                found = true;
            }

            //if a block in the current piece is in this position
            if(!found)
            {
                for(int i = 0 ; i < pieceblocks.size() ; i++)
                {
                    if(pieceblocks.at(i).y + my_piece->getLocation().y < board_height)
                    {
                        if(pieceblocks.at(i).x + my_piece->getLocation().x == x &&
                        pieceblocks.at(i).y + my_piece->getLocation().y == y)
                        {
                           ss << "O";
                           found = true;
                        }
                    }

                }
            }

            //if a block in the ghost is in this position
            if(!found)
            {
                for(int i = 0 ; i < ghostblocks.size() ; i++)
                {
                    if(ghostblocks.at(i).x + my_ghost->getLocation().x == x &&
                       ghostblocks.at(i).y + my_ghost->getLocation().y == y)
                       {
                           ss << "G";
                           found = true;
                       }
                }
            }

            //else there's nothing
            if(!found)
                ss << ".";
        }
        ss << "\n";
    }

    return ss.str();
}

bool TetrisBoard::isEvents() { return my_events.size() == 0; }

TetrisBoard::BoardEvent TetrisBoard::getEvent()
{
	if(my_events.size() > 0)
	{
		BoardEvent e = my_events.back();
		my_events.pop_back();
		return e;
	}
	return TetrisBoard::NONE;
}

std::string TetrisBoard::getStats()
{
	std::stringstream ss;
	ss << "CW rotations: " << stat_cw << std::endl;
	ss << "CCW rotations: " << stat_ccw << std::endl;
	ss << "left moves: " << stat_left << std::endl;
	ss << "right moves: " << stat_right << std::endl;
	ss << "down moves: " << stat_down << std::endl;
	ss << "hard drops: " << stat_hd << std::endl;
	ss << "pieces held: " << stat_holds << std::endl;
	ss << "pieces played: " << stat_pieces << std::endl;
	ss << "single line clears: " << stat_singles << std::endl;
	ss << "double line clears: " << stat_doubles << std::endl;
	ss << "triple line clears: " << stat_triples << std::endl;
	ss << "quad line clears: " << stat_quads << std::endl;

	return ss.str();
}

bool TetrisBoard::isOverlap(TetrisPiece* piece)
{
    std::vector<Point> blocks = piece->getBlocks();
    int xp=0, yp=0;

    for(std::vector<Point>::iterator i = blocks.begin(), end = blocks.end();
        i != end ; ++i)
    {
        xp = i->x + piece->getLocation().x;
        yp = i->y + piece->getLocation().y;

        if(yp < board_height && yp >= 0)
        {
            if((my_board.at(yp)->at(xp)) != TetrisPiece::EMPTY_PIECE)
                return true;
        }

    }

    return false;
}

bool TetrisBoard::isWithinBounds(TetrisPiece* piece)
{
    //TODO: erase this stuff!
//    std::cout << "farthest right possible=" << board_width-1 << std::endl;
//    std::cout << "lowest y pos=" << piece->getLocation().y + piece->getBottom() << std::endl;
//    std::cout << "farthest left pos=" << piece->getLocation().x + piece->getLeft() << std::endl;
//    std::cout << "farthest right pos=" << piece->getLocation().x + piece->getRight() << std::endl;

    //if it's below the board
    if(piece->getLocation().y + piece->getBottom() < 0)
        return false;

    //if it's too far to the left
    if(piece->getLocation().x + piece->getLeft() < 0)
        return false;

    //if it's too far to the right
    if(piece->getLocation().x + piece->getRight() >= board_width)
        return false;

    return true;
}

bool TetrisBoard::isOccupied(const int x, const int y)
{
	//check to see if the coords are in bounds first
	if(x >=0 && x < board_width && y >=0 &&
		y < board_height - NUM_HIDDEN_ROWS)
	{
		if((*my_board[y])[x] != TetrisPiece::EMPTY_PIECE)
			return true;
		else
			return false;
	}
	return false;
}

void TetrisBoard::placePiece()
{
    std::vector<Point> blocks = my_piece->getBlocks();
    int x=0,y=0;

    //TODO: put gameover handling logic here! don't do the stuff below if
    //gameover or else you'll get a crash or something.
    if(my_piece->getLocation().y + my_piece->getTop() >
       board_height - NUM_HIDDEN_ROWS)
        game_over = true;

    int edges=0;
    //handle points. the number of points is the number of blocks on the board
    //touching the piece on an edge squared
    x = my_piece->getLocation().x;
    y = my_piece->getLocation().y;
    for(std::vector<Point>::iterator i = blocks.begin(), end = blocks.end();
    	i != end; ++i)
    {
    	if(isOccupied(i->x-1 + x, i->y + y))
    		edges++;
    	if(isOccupied(i->x+1 + x, i->y + y))
    	    edges++;
    	if(isOccupied(i->x + x, i->y-1 + y))
    	    edges++;
    	if(isOccupied(i->x + x, i->y+1 + y))
    	    edges++;
    }
    std::cout << "num touching edges: " << edges << std::endl;
    score += edges * edges;

    if(!game_over)
    {
        for(std::vector<Point>::iterator i = blocks.begin(), end = blocks.end();
            i != end ; ++i)
        {
            x = my_piece->getLocation().x + i->x;
            y = my_piece->getLocation().y + i->y;

            (*my_board[y])[x] = my_piece->getPieceType();

            //TODO: update score
        }

        handleLines();
        newPiece();
        my_events.push_back(TetrisBoard::PIECE_PLACE);
    }

}

void TetrisBoard::newPiece()
{
    held = false;
    newBag();

    my_piece = *my_bag.begin();
    my_bag.erase(my_bag.begin());

    my_piece = my_piece->setLocation(board_width/2, board_height-1);
    stat_pieces++;

    //my_ghost = new TetrisPiece(my_piece->getPieceType());
    handleGhost();
}

void TetrisBoard::handleGhost()
{
//    my_ghost = my_piece->setLocation(my_piece->getLocation().x,
//                                     0-my_piece->getBottom());
//
//    while(isOverlap(my_ghost))
//        my_ghost = my_ghost->moveUp();

//    my_ghost = my_piece->getCopy();
//    while(!isOverlap(my_ghost) || !isWithinBounds(my_ghost))
//        my_ghost = my_ghost->moveDown();
//
//    my_ghost = my_ghost->moveUp();

    my_ghost = my_piece->getCopy();
    while(my_ghost->getLocation().y + my_ghost->getTop() >= board_height)
        my_ghost = my_ghost->moveDown();

    while(!isOverlap(my_ghost) && isWithinBounds(my_ghost))
        my_ghost = my_ghost->moveDown();

    my_ghost = my_ghost->moveUp();
}

void TetrisBoard::handleLines()
{
    bool found = false;
    int numlines = 0; //number of filled lines
    int blockcount = 0;

    do
    {
        found = false;
        //for each row
        for(std::vector<std::vector<TetrisPiece::PieceType>* >::iterator it
            = my_board.begin(), end = my_board.end() ; it != end ; ++it)
        {
            blockcount = 0;
            //for each item in each row
            for(std::vector<TetrisPiece::PieceType>::iterator x = (*it)->begin(),
                endx = (*it)->end() ; x != endx ; ++x)
            {
                if(*x == TetrisPiece::EMPTY_PIECE)
                    break;
                else
                    blockcount++;
            }

            if(blockcount >= board_width)
            {
                found = true;
                numlines++;
                my_board.erase(it);

                //add a blank line
                std::vector<TetrisPiece::PieceType>* row =
                    new std::vector<TetrisPiece::PieceType>();
                for(int xp = 0 ; xp < board_width ; xp++)
                    row->push_back(TetrisPiece::EMPTY_PIECE);
                my_board.push_back(row);

                break;
            }
        }
    } while(found);

    lines += numlines;

    //update level
    if(numlines > 0 && lines / 10 > level)
    {
    	level++;
    	my_events.push_back(TetrisBoard::LEVEL_UP);
    }


    if(numlines == 1)
    {
    	stat_singles++;
    	my_events.push_back(TetrisBoard::SINGLE_LINE);
    }
    else if(numlines == 2)
    {
    	stat_doubles++;
    	my_events.push_back(TetrisBoard::DOUBLE_LINE);
    }
    else if(numlines == 3)
    {
    	stat_triples++;
    	my_events.push_back(TetrisBoard::TRIPLE_LINE);
    }
    else if(numlines == 4)
    {
    	stat_quads++;
    	my_events.push_back(TetrisBoard::QUAD_LINE);
    }

    //UPDATE SCORE
    if(numlines > 0)
    	score += 100 * std::pow((float)2,(float)(numlines-1));
}

void TetrisBoard::newBag()
{
    if((int)my_bag.size() <= TetrisPiece::NUM_PIECES)
    {
        // create a temp bag and load it with pieces
        std::vector<TetrisPiece*> new_bag;
        new_bag.push_back(new TetrisPiece(TetrisPiece::I));
        new_bag.push_back(new TetrisPiece(TetrisPiece::J));
        new_bag.push_back(new TetrisPiece(TetrisPiece::L));
        new_bag.push_back(new TetrisPiece(TetrisPiece::O));
        new_bag.push_back(new TetrisPiece(TetrisPiece::S));
        new_bag.push_back(new TetrisPiece(TetrisPiece::Z));
        new_bag.push_back(new TetrisPiece(TetrisPiece::T));

        // shuffle pieces
        std::random_shuffle(new_bag.begin(), new_bag.end());

        // add pieces to the bag
        for(std::vector<TetrisPiece*>::iterator i = new_bag.begin(),
            end = new_bag.end() ; i != end ; ++i)
        {
            my_bag.push_back(*i);
        }
        // get rid of temp bag. NOTE: this may erase what was just put into
        // my_bag so watch out
        new_bag.clear();
    }
}

void TetrisBoard::emptyBoard()
{
    my_board.clear();
    std::vector<TetrisPiece::PieceType>* row;

    for(int y = 0 ; y < board_height ; y++)
    {
        row = new std::vector<TetrisPiece::PieceType>();
        for(int x = 0 ; x < board_width ; x++)
        {
            row->push_back(TetrisPiece::EMPTY_PIECE);
        }
        my_board.push_back(row);
    }
}

void TetrisBoard::init(const int width, const int height)
{
    //seed the random number generator with current clock time.
    srand(time(0));

	board_width = width;
	board_height = height;

	score = 0;
	level = 0;
	lines = 0;

	game_over = false;

	newBag();
	emptyBoard();
	newPiece();

	my_hold_piece = new TetrisPiece(TetrisPiece::EMPTY_PIECE);

	//init stats
	stat_cw=0; stat_ccw=0; //# of rotations
	stat_left=0; stat_right=0; stat_down=0; stat_hd=0; //number of moves/drops
	stat_holds=0; //# of holds
	stat_singles=0, stat_doubles=0, stat_triples=0, stat_quads=0; //#of clear types
	stat_pieces=0;
}
