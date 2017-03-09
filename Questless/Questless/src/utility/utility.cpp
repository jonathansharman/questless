/**
* @file    utility.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implementation for utility functions.
*/

#include "utility/utility.h"
#include "Game.h"

using std::function;

namespace questless
{
	///////////////
	// Debugging //
	///////////////

	double time(function<void()> code)
	{
		auto start_time = Game::clock::now();
		code();
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(Game::clock::now() - start_time).count();
	}

	//////////////////////////////
	// Random number generation //
	//////////////////////////////

	std::mt19937_64 rng(Game::clock::now().time_since_epoch().count());

	//////////
	// Math //
	//////////

	uint8_t percentage_to_byte(double percentage)
	{
		if (percentage <= 0) {
			return 0;
		} else if (percentage >= 1) {
			return 255;
		} else {
			return static_cast<uint8_t>(255 * percentage);
		}
	}
}
