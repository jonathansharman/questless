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
		static constexpr double distance_factor = 1;
		static constexpr double light_factor = 1.0;

		double acuity;
		double ideal_light;
		double light_tolerance;

		/// @return The base vision of the given Being subclass.
		template <typename BeingType>
		static Vision of()
		{
			using t = BeingType;
			return Vision
				{ t::base_visual_acuity
				, t::base_ideal_light
				, t::base_light_tolerance
				};
		}

		Vision() : acuity{0}, ideal_light{0}, light_tolerance{0} {}

		Vision(double acuity, double ideal_light, double light_tolerance)
			: acuity{acuity}, ideal_light{ideal_light}, light_tolerance{light_tolerance}
		{}

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
