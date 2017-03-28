/**
* @file    Vision.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <algorithm>

#include "utility/Bounded.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Acuity : TaggedType<double> { using TaggedType::TaggedType; };
	struct IdealLight : TaggedType<double> { using TaggedType::TaggedType; };
	struct LightTolerance : TaggedType<double> { using TaggedType::TaggedType; };

	/// Represents a being's vision-related stats.
	///
	/// Let v  = visibility at a tile
	///     a  = visual acuity
	///     d  = distance to tile
	///     df = distance factor (constant)
	///     l  = light level at tile
	///     i  = ideal light
	///     t  = light tolerance
	///     af = affinity factor (constant)
	///
	/// Then v = (a - d^2 * df) / (1 + (l - i)^2 / t * af)
	////
	struct Vision
	{
		static constexpr double minimum_value = 0.0;

		/// Increases the effect of distance on vision.
		////
		static constexpr double distance_factor = 1.0;

		/// Increases the effect of lighting on vision.
		////
		static constexpr double light_factor = 1.0;

		/// Increases the range of visibility.
		////
		Bounded<double, minimum_value> acuity;

		/// The light level at which a being can see best.
		////
		Bounded<double, minimum_value> ideal_light;

		/// Reduces the rate at which visibility falls off as light diverges from the ideal level.
		////
		Bounded<double, minimum_value> light_tolerance;

		constexpr Vision() : acuity{0.0}, ideal_light{0.0}, light_tolerance{0.0} {}

		constexpr Vision(Acuity acuity, IdealLight ideal_light, LightTolerance light_tolerance)
			: acuity{acuity.value}
			, ideal_light{ideal_light.value}
			, light_tolerance{light_tolerance.value}
		{}

		friend std::ostream& operator <<(std::ostream& out, Vision v)
		{
			out << v.acuity << ' ' << v.ideal_light << ' ' << v.light_tolerance;
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Vision v)
		{
			in >> v.acuity >> v.ideal_light >> v.light_tolerance;
			return in;
		}
	};
}
