/*
 * TreadmillBoard.h
 *
 * Author: Travis Lewis
 * Created on: 20 Sep 2012
 */

#ifndef TREADMILLBOARD_H_
#define TREADMILLBOARD_H_

#include "TetrisBoard.h"

/** \brief A variant of the classic TetrisBoard style gameplay.
 *
 * This is a simple variant where the only change is that after each piece is
 * placed, the entire board is shifted one space to the left, and the left-most
 * column is wrapped around to become the right-most column.
 */
class TreadmillBoard: public TetrisBoard
{
protected:
	//* <inheritdoc> */
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
