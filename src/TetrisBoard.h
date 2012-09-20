/*
 * TetrisBoard.h
 *
 *  Created on: Sep 12, 2012
 *      Author: econobeing
 */

#ifndef TETRISBOARD_H_
#define TETRISBOARD_H_

#include <vector>
#include <string>

#include "TetrisPiece.h"
#include "Timer.h"

class TetrisBoard
{
public:
    static const int NUM_HIDDEN_ROWS = 2;

	TetrisBoard();
	TetrisBoard(const int width, const int height);

	void moveLeft();
	void moveRight();
	void moveDown();
	void hardDrop();

	void rotateCCW();
	void rotateCW();

	void holdPiece();

//	void update(); //this is called so the timer can move the piece down
//	void pause();
//	void unpause(); //probably don't actually need these, we'll let the screen handle timing

	int getScore();
	int getLevel();
	int getLines();

	int getWidth();
	int getHeight();

	//TODO: create gameover handling!
	bool isGameOver();

	std::vector<std::vector<TetrisPiece::PieceType> > getBoard();
	TetrisPiece* getHold();
	TetrisPiece* getGhost();
	TetrisPiece* getActivePiece();
	std::vector<TetrisPiece*> getBag();

	std::string toString();
private:
	static const int DEFAULT_BOARD_WIDTH  = 10;
	static const int DEFAULT_BOARD_HEIGHT = 22;
	//hidden rows are always the top two of the board height

	int board_width, board_height;
	TetrisPiece* my_piece;
	TetrisPiece* my_ghost;
	TetrisPiece* my_hold_piece;
	std::vector<TetrisPiece*> my_bag;
	std::vector<std::vector<TetrisPiece::PieceType>* > my_board; // my_board[y][x]
	bool held; //to prevent holding before placing a piece
	bool game_over;
	int score, level, lines;

	bool isOverlap(TetrisPiece* piece);//tests to see if given piece overlaps anything on board
	bool isWithinBounds(TetrisPiece* piece); //tests to see if the piece is inside the board boundaries
	void placePiece();
	void newPiece(); // all the stuff that goes with getting a new piece
	void handleGhost(); // puts ghost in correct position (doesn't rotate)
	void handleLines();
	void newBag(); //puts the 7 pieces in a random order into the bag
	void emptyBoard(); //create an empty board

	void init(const int width, const int height);
};


#endif /* TETRISBOARD_H_ */
