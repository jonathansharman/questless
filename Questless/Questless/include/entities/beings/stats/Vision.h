//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <iostream>

#include "utility/Bounded.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Acuity : TaggedType<double> { using TaggedType::TaggedType; };
	struct IdealIlluminance : TaggedType<double> { using TaggedType::TaggedType; };
	struct IlluminanceTolerance : TaggedType<double> { using TaggedType::TaggedType; };

	//! Represents a being's vision-related stats.
	class Vision
	{
	public:
		static constexpr double minimum_value = 0.0;

		//! Increases the range of visibility.
		Bounded<double, minimum_value> acuity;

		//! The illuminance at which a being can see best.
		Bounded<double, minimum_value> ideal_illuminance;

		//! Reduces the rate at which visibility falls off as light diverges from the ideal level.
		Bounded<double, minimum_value> illuminance_tolerance;

		constexpr Vision() : acuity{0.0}, ideal_illuminance{0.0}, illuminance_tolerance{0.0} {}

		constexpr Vision(Acuity acuity, IdealIlluminance ideal_illuminance, IlluminanceTolerance illuminance_tolerance)
			: acuity{acuity.value}
			, ideal_illuminance{ideal_illuminance.value}
			, illuminance_tolerance{illuminance_tolerance.value}
		{}

		friend std::ostream& operator <<(std::ostream& out, Vision v)
		{
			out << v.acuity << ' ' << v.ideal_illuminance << ' ' << v.illuminance_tolerance;
			return out;
		}

		friend std::istream& operator >>(std::istream& in, Vision v)
		{
			in >> v.acuity >> v.ideal_illuminance >> v.illuminance_tolerance;
			return in;
		}

		//! The maximum distance in hex units a being with this vision can see.
		int max_range() const;

		//! How visible a tile illuminated with @p illuminance is to a being with this vision located @p distance hex units from the tile.
		double visibility(double illuminance, int distance) const;
	};
}
