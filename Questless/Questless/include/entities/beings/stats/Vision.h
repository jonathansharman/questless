//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <iostream>

#include "utility/Bounded.h"
#include "utility/TaggedType.h"
#include "units/constants.h"

namespace questless
{
	struct Acuity : TaggedType<double> { using TaggedType::TaggedType; };
	struct IdealIlluminance : TaggedType<double> { using TaggedType::TaggedType; };
	struct DarknessTolerance : TaggedType<double> { using TaggedType::TaggedType; };
	struct GlareTolerance : TaggedType<double> { using TaggedType::TaggedType; };

	//! Represents a being's vision-related stats.
	class Vision
	{
	public:
		static constexpr double minimum_value = 0.0;

		//! Increases the range of visibility.
		Bounded<double, minimum_value> acuity;

		//! The illuminance at which a being can see best.
		Bounded<double, minimum_value> ideal_illuminance;

		//! Reduces the rate at which visibility falls off as illuminance deceeds the ideal level.
		Bounded<double, minimum_value> darkness_tolerance;

		//! Reduces the rate at which visibility falls off as illuminance exceeds the ideal level.
		Bounded<double, minimum_value> glare_tolerance;

		constexpr Vision() : acuity{0.0}, ideal_illuminance{0.0}, darkness_tolerance{0.0}, glare_tolerance{0.0} {}

		constexpr Vision
			( Acuity acuity
			, IdealIlluminance ideal_illuminance
			, DarknessTolerance illuminance_tolerance
			, GlareTolerance glare_tolerance
			)
			: acuity{acuity.value}
			, ideal_illuminance{ideal_illuminance.value}
			, darkness_tolerance{illuminance_tolerance.value}
			, glare_tolerance{glare_tolerance.value}
		{}

		friend std::ostream& operator <<(std::ostream& out, Vision v)
		{
			out << v.acuity << ' ' << v.ideal_illuminance << ' ' << v.darkness_tolerance << ' ' << v.glare_tolerance << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Vision v)
		{
			in >> v.acuity >> v.ideal_illuminance >> v.darkness_tolerance >> v.glare_tolerance;
			return in;
		}

		//! The maximum distance in hex units a being with this vision can see.
		int max_range() const;

		//! How visible a tile illuminated with @p illuminance is to a being with this vision located @p distance hex units from the tile.
		double visibility(double illuminance, int distance) const;
	};
}
