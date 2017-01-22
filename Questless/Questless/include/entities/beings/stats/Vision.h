/**
* @file    Vision.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Vision class, which encapsulates stats of a being's vision.
*/

#pragma once

#include <algorithm>

#include "utility/TaggedType.h"

namespace questless
{
	struct Acuity : TaggedType<double> { using TaggedType::TaggedType; };
	struct IdealLight : TaggedType<double> { using TaggedType::TaggedType; };
	struct LightTolerance : TaggedType<double> { using TaggedType::TaggedType; };

	class Vision
	{
	public:
		// Visibility at a tile = (acuity - distance^2 * vision distance factor) / (1 + (light level at tile - ideal light)^2 / light tolerance * vision affinity factor).
		static constexpr double distance_factor = 1.0;
		static constexpr double light_factor = 1.0;

		constexpr Vision() : _acuity{0.0}, _ideal_light{0.0}, _light_tolerance{0.0} {}

		constexpr Vision(Acuity acuity, IdealLight ideal_light, LightTolerance light_tolerance)
			: _acuity{std::max(0.0, acuity.value)}
			, _ideal_light{std::max(0.0, ideal_light.value)}
			, _light_tolerance{std::max(0.0, light_tolerance.value)}
		{}

		friend std::ostream& operator <<(std::ostream& out, Vision v)
		{
			out << v._acuity << ' ' << v._ideal_light << ' ' << v._light_tolerance;
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Vision v)
		{
			in >> v._acuity >> v._ideal_light >> v._light_tolerance;
			return in;
		}

		constexpr double acuity() const { return _acuity; }
		void acuity(double value) { _acuity = std::max(0.0, value); }

		/// @return The light level at which a being can see best.
		constexpr double ideal_light() const { return _ideal_light; }
		void ideal_light(double value) { _ideal_light = std::max(0.0, value); }

		/// @return Light tolerance, which reduces the rate at which visibility falls off as light diverges from the ideal level.
		constexpr double light_tolerance() const { return _light_tolerance; }
		void light_tolerance(double value) { _light_tolerance = std::max(0.0, value); }
	private:
		double _acuity;
		double _ideal_light;
		double _light_tolerance;
	};
}
