/*
 * TetrisPiece.h
 *
 * Author: Travis Lewis
 * Created on: 11 Sep 2012
 */

#ifndef TETRISPIECE_H_
#define TETRISPIECE_H_

#include <vector>

/**
 * \brief A simple point.
 *
 * Holds X and Y values
 */
struct Point
{
	int x; //!< X coordinate
	int y; //!< Y coordinate
};

/**
 * \brief Holds information about a Tetris Piece.
 *
 * Does not hold or perform operations having to do with a game of Tetris.
 * This object only holds a generic information, the board or game that
 * uses this object needs to handle all of the other information/operations.
 * TetrisPiece objects are immutable.
 */
class TetrisPiece
{
public:
	/**
	 * The type of the piece. Can (and should) be used by the board / game to
	 * determine which image to draw for the blocks that make up the piece.
	 * EMPTY_PIECE should never be used as a usable piece.
	 */
	enum PieceType { EMPTY_PIECE, I, J, L, Z, S, O, T };

	/** The number of difference pieces in Tetris. */
	static const int NUM_PIECES = 7;

	/** The number of blocks in each piece. */
	static const int BLOCKS_PER_PIECE = 4;

	/**
	 * \brief generic constructor, give it a piece type and get out a piece.
	 * @param type The type of piece to create.
	 * @return The completed piece.
	 */
	TetrisPiece(PieceType type);

	//TODO: redo rotation functions so they follow tetris guidelines

	/**
	 * \brief Creates a new piece representing the current piece rotated CCW
	 * by 90 degrees.
	 * @return A pointer to the rotated piece.
	 */
	TetrisPiece* rotateCCW();

	/**
	 * \brief Creates a new piece representing the current piece rotated CW
	 * by 90 degrees.
	 * @return A pointer to the rotated piece.
	 */
	TetrisPiece* rotateCW();

	/**
	 * \brief Creates a new piece representing the current piece moved left
	 * one unit.
	 * @return A pointer to the newly translated piece.
	 */
	TetrisPiece* moveLeft();

	/**
	 * \brief Creates a new piece representing the current piece moved right
	 * one unit.
	 * @return A pointer to the newly translated piece.
	 */
	TetrisPiece* moveRight();

	/**
	 * \brief Creates a new piece representing the current piece moved down
	 * one unit.
	 * @return A pointer to the newly translated piece.
	 */
	TetrisPiece* moveDown();

	/**
	 * \brief Creates a new piece representing the current piece moved up
	 * one unit.
	 * @return A pointer to the newly translated piece.
	 */
	TetrisPiece* moveUp();

	/**
	 * \brief Creates a new piece representing the current piece moved
	 * to the given location.
	 * @param x The X position to place the translated piece.
	 * @param y The Y position to place the translated piece.
	 * @return A pointer to newly translated piece.
	 */
	TetrisPiece* setLocation(const int x, const int y);

	/** \brief Returns a copy of the piece. Useful for read-only operations. */
	TetrisPiece* getCopy();

	/**
	 * \brief Gets the current location of the piece. (read only)
	 * @return A copy of the piece's location.
	 */
	Point getLocation();

	/**
	 * \brief Gets a copy of the positions of the blocks.
	 * @return A vector which contains copies of the current positions of
	 * the blocks.
	 */
	std::vector<Point> getBlocks();

	/** \brief Gets the type of the current piece. */
	PieceType getPieceType();

	/** \brief Gets the farthest left block's X position. */
	int getLeft();

	/** \brief Gets the farthest right block's X position. */
	int getRight();

	/** \brief Gets the lowest block's Y position. */
	int getBottom();

	/** \brief Gets the highest block's Y position. */
	int getTop();

	// this will probably mainly be used for testing
	/**
	 * \brief Gets a string representation of the current piece.
	 *
	 * The returned string is constructed to be viewed visually, such as
	 * on the console. It isn't really useful for coding purposes.
	 * @return The string representation of the piece.
	 */
	std::string toString();
private:
	PieceType my_type; //!< The current piece type

	/**
	 * Contains information about where the blocks that make up the piece are
	 * located relative to the 4x4 grid, with it's origin at the bottom left.
	 */
	std::vector<Point> my_blocks;

	/** Where the lower left of the 4x4 grid of the piece is. */
	Point my_location;

	/**
	 * Used by internal functions to create a new piece after rotation or
	 * translation.
	 * @param blocks The blocks (positions of) that will make up the new piece.
	 * @param type The piece type.
	 * @param x The X position of the new piece.
	 * @param y The Y position of the new piece.
	 */
	TetrisPiece(std::vector<Point> blocks, PieceType type, const int x,
			const int y);

	/**
	 * Helper method to determine if the current piece is a horizontally
	 * oriented I piece.
	 * @return True if the current piece is a horizontally oriented I piece.
	 */
	bool isHorizI();

	/**
	 * \brief Creates a new point at the location given.
	 *
	 * A helper method to create a new Point object in one line. Useful to
	 * get rid of warnings from the compiler.
	 * @param x The X position of the point.
	 * @param y The Y position of the point.
	 * @return The newly created point.
	 */
	Point newPoint(const int x, const int y);
};


#endif /* TETRISBLOCK_H_ */
