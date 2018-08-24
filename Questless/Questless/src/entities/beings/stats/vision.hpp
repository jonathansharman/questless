//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <iostream>

#include "utility/static_bounded.hpp"
#include "utility/tagged_type.hpp"
#include "units/constants.hpp"
#include "units/math.hpp"

namespace ql {
	struct acuity : tagged_type<double> { using tagged_type::tagged_type; };
	struct ideal_illuminance : tagged_type<double> { using tagged_type::tagged_type; };
	struct darkness_tolerance : tagged_type<double> { using tagged_type::tagged_type; };
	struct glare_tolerance : tagged_type<double> { using tagged_type::tagged_type; };

	//! Represents a being's vision-related stats.
	class vision {
	public:
		static constexpr double minimum_value = 0.0;

		//! Increases the range of visibility.
		static_bounded<double, minimum_value> acuity;

		//! The illuminance at which a being can see best.
		static_bounded<double, minimum_value> ideal_illuminance;

		//! Reduces the rate at which visibility falls off as illuminance deceeds the ideal level.
		static_bounded<double, minimum_value> darkness_tolerance;

		//! Reduces the rate at which visibility falls off as illuminance exceeds the ideal level.
		static_bounded<double, minimum_value> glare_tolerance;

		constexpr vision()
			: acuity{0.0}
			, ideal_illuminance{0.0}
			, darkness_tolerance{0.0}
			, glare_tolerance{0.0}
			, _previous_acuity{0.0}
			, _max_range{0}
		{}

		constexpr vision
			( ql::acuity acuity
			, ql::ideal_illuminance ideal_illuminance
			, ql::darkness_tolerance illuminance_tolerance
			, ql::glare_tolerance glare_tolerance
			)
			: acuity{acuity.value}
			, ideal_illuminance{ideal_illuminance.value}
			, darkness_tolerance{illuminance_tolerance.value}
			, glare_tolerance{glare_tolerance.value}
			, _previous_acuity{acuity.value}
			, _max_range{get_max_range()}
		{}

		friend std::ostream& operator <<(std::ostream& out, vision v) {
			out << v.acuity << ' ' << v.ideal_illuminance << ' ' << v.darkness_tolerance << ' ' << v.glare_tolerance << ' ';
			return out;
		}

		friend std::istream& operator >>(std::istream& in, vision v) {
			in >> v.acuity >> v.ideal_illuminance >> v.darkness_tolerance >> v.glare_tolerance;
			return in;
		}

		//! The maximum distance in hex units a being with this vision can see.
		int max_range() const {
			if (_previous_acuity != acuity) {
				_max_range = get_max_range();
				_previous_acuity = acuity;
			}
			return _max_range;
		}

		//! How visible a tile illuminated with @p illuminance is to a being with this vision located @p distance hex units from the tile.
		double visibility(double illuminance, int distance) const;
	private:
		static constexpr double _distance_factor = 0.05;

		// Used for caching result of max_range().
		double mutable _previous_acuity;
		int mutable _max_range;

		constexpr int get_max_range() const {
			// Use a cutoff to avoid including many tiles with very low visibility.
			constexpr double cutoff = 20.0;

			return static_cast<int>(units::math::sqrt((acuity / cutoff - 1.0) / _distance_factor));
		}
	};
}
