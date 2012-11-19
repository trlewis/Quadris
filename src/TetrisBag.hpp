/*
 * TetrisBag.hpp
 *
 *  Created on: Nov 14, 2012
 *      Author: Travis Lewis
 */

#ifndef TETRISBAG_HPP_
#define TETRISBAG_HPP_

#include <vector>
#include <cmath>
#include <algorithm>

#include "TetrisPiece.h"

class TetrisBag
{
public:
	//////////
	// CONSTRUCTORS
	//////////

	/**
	 * \brief Generic constructor, uses standard bag system.
	 */
	TetrisBag()
	{
		srand(time(0));
		my_repeats = false;
	}

	/**
	 * \brief Create a bag that will repeatedly use the given sequence start
	 * to finish.
	 * @param a_sequence The repeating sequence to use.
	 */
	TetrisBag(std::vector<TetrisPiece> a_sequence)
	{
		srand(time(0));
		my_repeats = true;
		my_sequence = std::vector<TetrisPiece>(a_sequence);
		addSequence();
		addSequence();
	}

	//////////
	// METHODS
	//////////

	/**
	 * \brief Removes and returns the first TetrisPiece in the bag's queue.
	 */
	TetrisPiece pop()
	{
		TetrisPiece p = *my_pieces.begin();
		my_pieces.erase(my_pieces.begin());

		if(my_repeats)
		{
			if(my_pieces.size() <= my_sequence.size())
				addSequence();
		}
		else
		{
			if((int)my_pieces.size() <= TetrisPiece::NUM_PIECES)
				addSet();
		}

		return p;
	}

	/**
	 *\brief Returns copies of the first pieces in the bag's queue. Or all of
	 * the pieces in the queue if the_count is larger than the queue's size.
	 */
	std::vector<TetrisPiece> peek(const int the_count)
	{
		std::vector<TetrisPiece> temp;

		for(int i = 0 ; i < the_count ; i++)
			temp.push_back(my_pieces.at(i).getCopy());

		return temp;

	}


private:
	//////////
	// FIELDS
	//////////
	std::vector<TetrisPiece> my_pieces;
	std::vector<TetrisPiece> my_sequence;
	bool my_repeats;

	//////////
	// METHODS
	//////////

	void addSet()
	{
		std::vector<TetrisPiece> temp;
		temp.push_back(TetrisPiece(TetrisPiece::I));
		temp.push_back(TetrisPiece(TetrisPiece::J));
		temp.push_back(TetrisPiece(TetrisPiece::L));
		temp.push_back(TetrisPiece(TetrisPiece::S));
		temp.push_back(TetrisPiece(TetrisPiece::Z));
		temp.push_back(TetrisPiece(TetrisPiece::T));
		temp.push_back(TetrisPiece(TetrisPiece::O));

		std::random_shuffle(temp.begin(), temp.end());

		for(std::vector<TetrisPiece>::iterator i = temp.begin(),
			end = temp.end() ; i != end ; ++i)
		{
			my_pieces.push_back(*i);
		}
	}


	void addSequence()
	{
		for(std::vector<TetrisPiece>::iterator i = my_sequence.begin(),
			end = my_sequence.end() ; i != end ; ++i)
		{
			my_pieces.push_back(*i);
		}
	}
};


#endif /* TETRISBAG_HPP_ */
