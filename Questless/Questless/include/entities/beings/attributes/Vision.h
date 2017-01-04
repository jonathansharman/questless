/**
* @file    Vision.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The Vision class, which encapsulates attributes of a being's vision.
*/

#ifndef VISION_H
#define VISION_H

namespace questless
{
	struct Vision
	{
		// Visibility at a tile = (acuity - distance^2 * vision distance factor) / (1 + (light level at tile - ideal light)^2 / light tolerance * vision affinity factor).
		static constexpr double distance_factor = 1.0;
		static constexpr double light_factor = 1.0;

		double acuity;
		double ideal_light; // Light level at which a being can see best.
		double light_tolerance; // Reduces the rate at which visibility falls off as light diverges from the ideal level.

		/// @return The base vision of the given Being subclass.
		template <typename BeingType>
		static Vision of()
		{
			using T = BeingType;
			return Vision
				{ T::base_visual_acuity
				, T::base_ideal_light
				, T::base_light_tolerance
				};
		}

		Vision() : acuity{0}, ideal_light{0}, light_tolerance{0} {}

		Vision(double acuity, double ideal_light, double light_tolerance)
			: acuity{acuity}, ideal_light{ideal_light}, light_tolerance{light_tolerance}
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

		/// Sets all vision levels that are below their minimum values to their minimum values.
		void clamp()
		{
			if (acuity < 0.0) { acuity = 0.0; }
			if (ideal_light < 0.0) { ideal_light = 0.0; }
			if (light_tolerance < 0.0) { light_tolerance = 0.0; }
		}
	};
}

#endif
