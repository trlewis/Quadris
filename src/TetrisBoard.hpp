/*
 * TetrisBoard.hpp
 *
 * Author: Travis Lewis
 * Created on: 12 Sep 2012
 */

#ifndef TETRISBOARD_HPP_
#define TETRISBOARD_HPP_

#include <vector>
#include <string>

#include "TetrisPiece.hpp"
#include "Timer.h"

class TetrisBoard
{
public:
	enum BoardEvent {
		SINGLE_LINE, DOUBLE_LINE, TRIPLE_LINE, QUAD_LINE, HOLD,
		ROTATE, PIECE_PLACE, LEVEL_UP, GAME_WIN, NONE
	};

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

	int getScore();
	int getLevel();
	int getLines();

	int getWidth();
	int getHeight();

	//TODO: create gameover handling!
	bool isGameOver();

	std::vector<std::vector<TetrisPiece::PieceType> > getBoard();
	TetrisPiece getHold();
	TetrisPiece getGhost();
	TetrisPiece getActivePiece();
	std::vector<TetrisPiece> getBag();

	std::string toString();
	bool hasEvents();//whether or not there are events not returned
	BoardEvent getEvent();//get the top event on the stack
	std::string getStats();

	//this gets rid of "TetrisBoard has virtual method placePiece but
	//non-virtual destructor.
	virtual ~TetrisBoard() {}
protected:
	std::vector<std::vector<TetrisPiece::PieceType> > my_board; // my_board[y][x]
	virtual void placePiece();
private:
	static const int DEFAULT_BOARD_WIDTH  = 10;
	static const int DEFAULT_BOARD_HEIGHT = 22;
	//hidden rows are always the top two of the board height

	int board_width, board_height;
	TetrisPiece my_piece;
	TetrisPiece my_ghost;
	TetrisPiece my_hold_piece;
	std::vector<TetrisPiece> my_bag;

	std::vector<BoardEvent> my_events; //gives user info on what's happening (should be a queue...)

	bool held; //to prevent holding before placing a piece
	bool game_over;
	int score, level, lines;

	//stats
	int stat_cw, stat_ccw; //# of rotations
	int stat_left, stat_right, stat_down, stat_hd; //number of moves/drops
	int stat_holds; //# of holds
	int stat_singles, stat_doubles, stat_triples, stat_quads; //#of clear types
	int stat_pieces; //# of pieces played

	bool isOverlap(TetrisPiece* piece);//tests to see if given piece overlaps anything on board
	bool isWithinBounds(TetrisPiece* piece); //tests to see if the piece is inside the board boundaries
	bool isOccupied(const int x, const int y);//if the given space on the board is occupied.
	void newPiece(); // all the stuff that goes with getting a new piece
	void handleGhost(); // puts ghost in correct position (doesn't rotate)
	void handleLines();
	void newBag(); //puts the 7 pieces in a random order into the bag
	void emptyBoard(); //create an empty board

	void init(const int width, const int height);
};


#endif /* TETRISBOARD_H_ */
