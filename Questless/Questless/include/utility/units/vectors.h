/**
* @file    vectors.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Contains definitions for various vector units.
*/

#ifndef VECTORS_H
#define VECTORS_H

#include "geometry.h"
#include "../Rate.h"
#include "../VectorF.h"

namespace questless
{
	using Velocity = Rate<VectorF, double>;
	using Acceleration = Rate<Velocity, double>;

	using AngularVelocity = Rate<double, double>;

	using ScaleVelocity = Rate<double, double>;
}

#endif
