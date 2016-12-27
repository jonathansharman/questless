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
#include "utility/Rate.h"
#include "utility/Vector.h"

using namespace std::literals;

namespace questless
{
	////////////
	// Timing //
	////////////

	using clock = std::chrono::steady_clock;
	using seconds_f = std::chrono::duration<double>;

	constexpr Hertz frame_rate = 60.0_Hz;
	constexpr seconds_f frame_duration = 1.0 / frame_rate;

	constexpr seconds_f accrued_update_time_max{1.0};

	//////////
	// Math //
	//////////

	constexpr double tau = 6.283185307179586476925;
	constexpr double pi = tau / 2.0;

	///////////
	// World //
	///////////

	constexpr int section_radius = 10;
	constexpr int section_diameter = 2 * section_radius + 1;
	
	constexpr GameVector hex_unit_r{0.0, 36.0};
	constexpr GameVector hex_unit_q{44.0, -18.0};

	constexpr int region_background_margin_top = 19;
	constexpr int region_background_margin_left = 22;
}

#endif
