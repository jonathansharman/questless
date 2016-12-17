/**
* @file    geometry.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Contains definitions for units of motion.
*/

#ifndef UNITS_H
#define UNITS_H

#include "../TaggedType.h"

namespace questless
{
	using Length = TaggedType<double, struct LengthTag>;
	using LengthSquared = TaggedType<double, struct LengthSquaredTag>;

	using AngleRadians = TaggedType<double, struct AngleRadiansTag>;

	using Scale = TaggedType<double, struct ScaleTag>;
}

#endif
