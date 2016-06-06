/**
* @file    constants.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Constants, type definitions, and enums used in Questless.
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "utility/Frequency.h"
#include "utility/VectorF.h"

namespace questless
{
	////////////
	// Timing //
	////////////

	using clock = std::chrono::steady_clock;
	using double_seconds = std::chrono::duration<double>;

	const Hertz frame_rate{60.0};
	const double_seconds frame_duration = 1 / frame_rate;

	const double_seconds accrued_update_time_max{1.0};

	//////////
	// Math //
	//////////

	const double pi = 3.14159265358979323846;
	const double tau = 6.283185307179586476925;

	///////////
	// World //
	///////////

	const int section_radius = 10;
	const int section_diameter = 2 * section_radius + 1;
	
	const sdl::Vector hex_unit_r{0, 36};
	const sdl::Vector hex_unit_q{44, -18};

	const int region_background_margin_top = 19;
	const int region_background_margin_left = 22;
}

#endif