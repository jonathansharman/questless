//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "utility/utility.h"
#include "Game.h"

using std::function;

namespace questless
{
	///////////////
	// Debugging //
	///////////////

	std::chrono::nanoseconds time(function<void()> f)
	{
		auto start_time = Game::clock::now();
		f();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(Game::clock::now() - start_time);
	}

	//////////////
	// File I/O //
	//////////////

	std::string contents_of_file(char const* filename)
	{
		std::ifstream fin(filename);
		std::stringstream ss;
		ss << fin.rdbuf();
		return ss.str();
	}

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

	//////////////////////////////
	// Random Number Generation //
	//////////////////////////////

	std::mt19937_64 rng(Game::clock::now().time_since_epoch().count());
}
