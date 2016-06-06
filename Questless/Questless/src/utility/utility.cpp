/**
* @file    utility.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Implementation for utility functions.
*/

#include "utility/utility.h"

using std::function;

namespace questless
{
	///////////////
	// Debugging //
	///////////////

	double time(function<void()> code)
	{
		auto start_time = clock::now();
		code();
		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(clock::now() - start_time).count();
	}

	//////////////////////////////
	// Random number generation //
	//////////////////////////////

	std::mt19937 rng(static_cast<unsigned long>(clock::now().time_since_epoch().count()));

	VectorF random_displacement(double max_length)
	{
		VectorF ret{uniform(0.0, max_length), 0.0};
		ret.rotate(uniform(0.0, 360.0));
		return ret;
	}

	VectorF random_displacement(double min_length, double max_length)
	{
		VectorF ret{uniform(min_length, max_length), 0.0};
		ret.rotate(uniform(0.0, 360.0));
		return ret;
	}

	//////////
	// Math //
	//////////

	uint8_t percentage_to_byte(double percent)
	{
		if (percent <= 0) {
			return 0;
		} else if (percent >= 1) {
			return 255;
		} else {
			return static_cast<uint8_t>(255 * percent);
		}
	}

	void extend_bounds(sdl::Rect& bounds, sdl::Point point)
	{
		if (point.x < bounds.x) {
			bounds.w += bounds.x - point.x;
			bounds.x = point.x;
		} else if (point.x >= bounds.x + bounds.w) {
			bounds.w = point.x - bounds.x + 1;
		}
		if (point.y < bounds.y) {
			bounds.h += bounds.y - point.y;
			bounds.y = point.y;
		} else if (point.y >= bounds.y + bounds.h) {
			bounds.h = point.y - bounds.y + 1;
		}
	}
}