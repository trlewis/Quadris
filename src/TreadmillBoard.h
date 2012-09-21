/*
 * TreadmillBoard.h
 *
 * Author: Travis Lewis
 * Created on: 20 Sep 2012
 */

#ifndef TREADMILLBOARD_H_
#define TREADMILLBOARD_H_

#include "TetrisBoard.h"

class TreadmillBoard: public TetrisBoard
{
public:
	void placePiece()
	{
		TetrisBoard::placePiece();

		for(unsigned int i = 0 ; i < my_board.size() ; i++)
		{
			my_board[i]->push_back((*my_board[i])[0]);
			my_board[i]->erase(my_board[i]->begin());
		}
	}
};

#endif /* TREADMILLBOARD_H_ */
