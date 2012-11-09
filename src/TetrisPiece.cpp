/*
 * TetrisPiece.cpp
 *
 * Author: Travis Lewis
 * Created on: 12 Sep 2012
 */

//TODO: delete this when done testing!
#include <iostream>
#include <sstream>

#include <vector>

#include "TetrisPiece.h"

TetrisPiece::TetrisPiece(PieceType type)
{
	my_location.x=0;
	my_location.y=0;
	my_type = type;
	my_blocks.clear();

	//TODO: replace all Point{a,b} calls with newPoint(a,b)
	switch(type)
	{
		case I:
			my_blocks.push_back(newPoint(1,0));
			my_blocks.push_back(newPoint(1,1));
			my_blocks.push_back(newPoint(1,2));
			my_blocks.push_back(newPoint(1,3));
			break;
		case J:
			my_blocks.push_back(newPoint(0,0));
			my_blocks.push_back(newPoint(1,0));
			my_blocks.push_back(newPoint(1,1));
			my_blocks.push_back(newPoint(1,2));
			break;
		case L:
			my_blocks.push_back(newPoint(0,0));
			my_blocks.push_back(newPoint(0,1));
			my_blocks.push_back(newPoint(0,2));
			my_blocks.push_back(newPoint(1,0));
			break;
		case Z:
			my_blocks.push_back(newPoint(0,1));
			my_blocks.push_back(newPoint(1,1));
			my_blocks.push_back(newPoint(1,0));
			my_blocks.push_back(newPoint(2,0));
			break;
		case S:
			my_blocks.push_back(newPoint(0,0));
			my_blocks.push_back(newPoint(1,0));
			my_blocks.push_back(newPoint(1,1));
			my_blocks.push_back(newPoint(2,1));
			break;
		case O:
			my_blocks.push_back(newPoint(1,2));
			my_blocks.push_back(newPoint(1,3));
			my_blocks.push_back(newPoint(2,2));
			my_blocks.push_back(newPoint(2,3));
			break;
		case T:
			my_blocks.push_back(newPoint(0,0));
			my_blocks.push_back(newPoint(1,0));
			my_blocks.push_back(newPoint(2,0));
			my_blocks.push_back(newPoint(1,1));
			break;
		default:
			my_blocks.push_back(newPoint(0,0)); // hopefully this
			my_blocks.push_back(newPoint(1,1)); // should never
			my_blocks.push_back(newPoint(2,2)); // happen...
			my_blocks.push_back(newPoint(3,3));
			break;
	}
}

TetrisPiece TetrisPiece::rotateCCW()
{
	// O pieces don't get rotated, so just return the current piece
	if(my_type == TetrisPiece::O)
		return getCopy();//return this;

	int oldX = 0, oldY = 0, lowX=3;
	std::vector<Point> new_blocks;

	//if it's an I block you need to do special rotation
	if(my_type == TetrisPiece::I)
	{
		if(isHorizI())
		{
			new_blocks.push_back(newPoint(1,0));
			new_blocks.push_back(newPoint(1,1));
			new_blocks.push_back(newPoint(1,2));
			new_blocks.push_back(newPoint(1,3));
		}
		else
		{
			new_blocks.push_back(newPoint(0,1));
			new_blocks.push_back(newPoint(1,1));
			new_blocks.push_back(newPoint(2,1));
			new_blocks.push_back(newPoint(3,1));
		}

		return TetrisPiece(new_blocks, my_type, my_location.x,
				my_location.y);
	}

	// if it's not an I or an O, rotate using this algorithm
	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
	{
		oldX = it->x;
		oldY = it->y;

		if(3 - oldY < lowX)
			lowX = 3 - oldY;

		new_blocks.push_back(newPoint(3-oldY,oldX));
	}

	// move it to the left
	for(std::vector<Point>::iterator it = new_blocks.begin(),
			end = new_blocks.end() ; it != end ; ++it)
		it->x -= lowX;

	return TetrisPiece(new_blocks, my_type, my_location.x, my_location.y);
}

TetrisPiece TetrisPiece::rotateCW()
{
	// O pieces don't get rotated, so just return the current piece
	if(my_type == TetrisPiece::O)
		return getCopy();//return this;

	int oldX = 0, oldY = 0, lowY=3;
	std::vector<Point> new_blocks;

	// if it's an I block you need to do special rotation
	if(my_type == TetrisPiece::I)
	{
		if(isHorizI())
		{
			new_blocks.push_back(newPoint(1,0));
			new_blocks.push_back(newPoint(1,1));
			new_blocks.push_back(newPoint(1,2));
			new_blocks.push_back(newPoint(1,3));
		}
		else
		{
			new_blocks.push_back(newPoint(0,1));
			new_blocks.push_back(newPoint(1,1));
			new_blocks.push_back(newPoint(2,1));
			new_blocks.push_back(newPoint(3,1));
		}

		return TetrisPiece(new_blocks, my_type, my_location.x,
				my_location.y);
	}

	// if it's not an I or O, do rotation as normal
	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end; ++it)
	{
		oldX = it->x;
		oldY = it->y;

		if(3 - oldX < lowY)
			lowY = 3 - oldX;

		new_blocks.push_back(newPoint(oldY,3-oldX));
	}

	// move to lower left corner
	for(std::vector<Point>::iterator it = new_blocks.begin(),
			end = new_blocks.end() ; it != end ; ++it)
		it->y -= lowY;

	return TetrisPiece(new_blocks, my_type, my_location.x, my_location.y);
}

TetrisPiece TetrisPiece::moveLeft()
{
	return TetrisPiece(my_blocks, my_type, my_location.x-1, my_location.y);
}

TetrisPiece TetrisPiece::moveRight()
{
	return TetrisPiece(my_blocks, my_type, my_location.x+1, my_location.y);
}

TetrisPiece TetrisPiece::moveDown()
{
	return TetrisPiece(my_blocks, my_type, my_location.x, my_location.y-1);
}

TetrisPiece TetrisPiece::moveUp()
{
	return TetrisPiece(my_blocks, my_type, my_location.x, my_location.y+1);
}

TetrisPiece TetrisPiece::setLocation(const int x, const int y)
{
	return TetrisPiece(my_blocks, my_type, x, y);
}

TetrisPiece TetrisPiece::getCopy()
{
	return TetrisPiece(my_blocks, my_type, my_location.x, my_location.y);
}

Point TetrisPiece::getLocation()
{
	return newPoint(my_location.x, my_location.y);
}

std::vector<Point> TetrisPiece::getBlocks()
{
	std::vector<Point> temp;
	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
		temp.push_back(newPoint(it->x,it->y));
	return temp;
}

TetrisPiece::PieceType TetrisPiece::getPieceType()
{
	return my_type;
}

int TetrisPiece::getLeft()
{
	int far = 4;

	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
		if(it->x < far)
			far = it->x;

	return far;
}

int TetrisPiece::getRight()
{
	int far = 0;

	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
		if(it->x > far)
			far = it->x;

	return far;
}

int TetrisPiece::getBottom()
{
	int low = 4;

	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
		if(it->y < low)
			low = it->y;

	return low;
}

int TetrisPiece::getTop()
{
    int high = 0;
    for(std::vector<Point>::iterator it = my_blocks.begin(),
        end = my_blocks.end() ; it != end ; ++it)
        if(it->y > high)
            high = it->y;
    return high;
}

std::string TetrisPiece::toString()
{
	std::stringstream ss;
	bool found = false;

	for(int y = 3 ; y >= 0 ; y--)
	{
		for(int x = 0 ; x <= 3 ; x++)
		{
			found = false;
			ss << "[";

			for(std::vector<Point>::iterator it = my_blocks.begin(),
					end = my_blocks.end() ; it != end ; ++it)
			{
				if(it->x == x && it->y == y)
				{
					found = true;
					break;
				}
			}
			ss << ((found==true) ? "X]" : " ]");
		}
		ss << "\n";
	}
	return ss.str();
}

TetrisPiece::TetrisPiece(std::vector<Point> blocks, PieceType type, const int x,
		const int y)
{
	my_blocks = blocks;
	my_type = type;
	my_location.x = x;
	my_location.y = y;
}

bool TetrisPiece::isHorizI()
{
	if(my_type != I)
		return false;

	// the horizontal I piece should be the only one to have
	// a block at (3,1)
	for(std::vector<Point>::iterator it = my_blocks.begin(),
			end = my_blocks.end() ; it != end ; ++it)
		if(it->x == 3 && it->y == 1)
			return true;

	return false;
}

Point TetrisPiece::newPoint(const int x, const int y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}
