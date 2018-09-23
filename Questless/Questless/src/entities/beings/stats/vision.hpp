//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/io.hpp"
#include "utility/nonnegative.hpp"
#include "utility/quantities.hpp"

#include "units/constants.hpp"
#include "units/math.hpp"

#include <nlohmann/json.hpp>

namespace ql::stats {
	//! Represents a being's vision-related stats.
	class vision {
	public:
		//! Increases the range of visibility.
		nonnegative<ql::acuity> acuity = 0.0_acuity;

		//! The minimum illuminance at which a being can see best.
		nonnegative<ql::illuminance> min_illuminance = 0.0_illuminance;

		//! The maximum illuminance at which a being can see best.
		nonnegative<ql::illuminance> max_illuminance = 0.0_illuminance;

		//! Reduces the rate at which visibility falls off as illuminance deceeds the ideal level.
		nonnegative<ql::darkness_tolerance> darkness_tolerance = 0.0_darkness_tolerance;

		//! Reduces the rate at which visibility falls off as illuminance exceeds the ideal level.
		nonnegative<ql::glare_tolerance> glare_tolerance = 0.0_glare_tolerance;

		constexpr vision() : _previous_acuity{0.0_acuity}, _max_range{0} {}

		constexpr vision
			( ql::acuity acuity
			, ql::illuminance min_illuminance
			, ql::illuminance max_illuminance
			, ql::darkness_tolerance illuminance_tolerance
			, ql::glare_tolerance glare_tolerance
			)
			: acuity{acuity}
			, min_illuminance{min_illuminance}
			, max_illuminance{max_illuminance}
			, darkness_tolerance{illuminance_tolerance}
			, glare_tolerance{glare_tolerance}
			, _previous_acuity{acuity}
			, _max_range{get_max_range()}
		{}

		vision& operator +=(vision const& that) {
			acuity = std::max(acuity, that.acuity);
			min_illuminance = std::min(min_illuminance, that.min_illuminance);
			max_illuminance = std::max(max_illuminance, that.max_illuminance);
			darkness_tolerance = std::max(darkness_tolerance, that.darkness_tolerance);
			glare_tolerance = std::max(glare_tolerance, that.glare_tolerance);
		}

		vision operator +(vision const& that) {
			vision result = *this;
			result += that;
			return result;
		}

		//! The maximum distance in hex units a being with this vision can see.
		int max_range() const {
			if (_previous_acuity != acuity.value()) {
				_max_range = get_max_range();
				_previous_acuity = acuity;
			}
			return _max_range;
		}

		//! How visible a tile illuminated with @p illuminance is to a being with this vision located @p distance hex units from the tile.
		double visibility(ql::illuminance illuminance, int distance) const;
	private:
		// Used for caching result of max_range().
		ql::acuity mutable _previous_acuity;
		int mutable _max_range;

		constexpr int get_max_range() const {
			// Use a cutoff to avoid including many tiles with very low visibility.
			constexpr double cutoff = 20.0;
			constexpr double distance_factor = 0.05;

			return static_cast<int>(units::math::sqrt((acuity.value().value / cutoff - 1.0) / distance_factor));
		}
	};

	void to_json(nlohmann::json& j, vision const& v) {
		if (v.acuity != 0.0_acuity) to_json(j["acuity"], v.acuity);
		if (v.min_illuminance != 0.0_illuminance) to_json(j["min_illuminance"], v.min_illuminance);
		if (v.max_illuminance != 0.0_illuminance) to_json(j["max_illuminance"], v.max_illuminance);
		if (v.darkness_tolerance != 0.0_darkness_tolerance) to_json(j["darkness_tolerance"], v.darkness_tolerance);
		if (v.glare_tolerance != 0.0_glare_tolerance) to_json(j["glare_tolerance"], v.glare_tolerance);
	}

	void from_json(nlohmann::json const& j, vision& v) {
		auto const acuity = j.find("acuity");
		if (acuity != j.end()) from_json(acuity.value(), v.acuity);

		auto const min_illuminance = j.find("min_illuminance");
		if (min_illuminance != j.end()) from_json(min_illuminance.value(), v.min_illuminance);

		auto const max_illuminance = j.find("max_illuminance");
		if (max_illuminance != j.end()) from_json(max_illuminance.value(), v.max_illuminance);

		auto const darkness_tolerance = j.find("darkness_tolerance");
		if (darkness_tolerance != j.end()) from_json(darkness_tolerance.value(), v.darkness_tolerance);

		auto const glare_tolerance = j.find("glare_tolerance");
		if (glare_tolerance != j.end()) from_json(glare_tolerance.value(), v.glare_tolerance);
	}
}
