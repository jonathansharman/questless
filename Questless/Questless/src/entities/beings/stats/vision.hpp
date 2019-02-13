//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/perception.hpp"
#include "quantities/distance.hpp"
#include "quantities/quantities.hpp"
#include "utility/io.hpp"
#include "utility/nonnegative.hpp"

namespace ql::stats {
	//! Represents a being's vision-related stats.
	struct vision {
		static constexpr auto acuity_loss_per_span = 10.0_acuity / 1_span;

		//! Increases the range of visibility.
		nonnegative<ql::acuity> acuity = 0.0_acuity;

		//! The minimum illuminance at which a being can see best.
		nonnegative<ql::lum> min_illuminance = 0.0_lum;

		//! The maximum illuminance at which a being can see best.
		nonnegative<ql::lum> max_illuminance = 0.0_lum;

		//! The rate effective acuity is decreased when below min. illuminance.
		nonnegative<acuity_per_lum> darkness_penalty = 0.0_acuity / 1.0_lum;

		//! The rate effective acuity is decreased when above max. illuminance.
		nonnegative<acuity_per_lum> glare_penalty = 0.0_acuity / 1.0_lum;

		constexpr vision() : _previous_acuity{0.0_acuity}, _max_range{0} {}

		constexpr vision(ql::acuity acuity,
			ql::lum min_illuminance,
			ql::lum max_illuminance,
			nonnegative<acuity_per_lum> darkness_penalty,
			nonnegative<acuity_per_lum> glare_penalty)
			: acuity{acuity}
			, min_illuminance{min_illuminance}
			, max_illuminance{max_illuminance}
			, darkness_penalty{darkness_penalty}
			, glare_penalty{glare_penalty}
			, _previous_acuity{acuity}
			, _max_range{get_max_range()} {}

		template <typename Archive>
		void save(Archive& archive) const {
			archive(CEREAL_NVP(acuity),
				CEREAL_NVP(min_illuminance),
				CEREAL_NVP(max_illuminance),
				CEREAL_NVP(darkness_penalty),
				CEREAL_NVP(glare_penalty),
				CEREAL_NVP(acuity));
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive(CEREAL_NVP(acuity),
				CEREAL_NVP(min_illuminance),
				CEREAL_NVP(max_illuminance),
				CEREAL_NVP(darkness_penalty),
				CEREAL_NVP(glare_penalty),
				CEREAL_NVP(acuity));
		}

		vision& operator+=(vision const& that) {
			acuity = std::max(acuity, that.acuity);
			min_illuminance = std::min(min_illuminance, that.min_illuminance);
			max_illuminance = std::max(max_illuminance, that.max_illuminance);
			darkness_penalty = std::min(darkness_penalty, that.darkness_penalty);
			glare_penalty = std::min(glare_penalty, that.glare_penalty);
			return *this;
		}

		vision operator+(vision const& that) {
			vision result = *this;
			result += that;
			return result;
		}

		//! The maximum distance in hex units a being with this vision can see.
		span max_range() const {
			if (_previous_acuity != acuity.value()) {
				_max_range = get_max_range();
				_previous_acuity = acuity;
			}
			return _max_range;
		}

		//! Perception level of a tile with this vision.
		//! @param illuminance The illuminance at the tile.
		//! @param distance Distance from the perceiver to the tile.
		//! @param occlusion The proportion of light blocked between the perceiver and the tile.
		perception::level perception(lum illuminance, span distance, double occlusion) const;

	private:
		// Used for caching result of max_range().
		ql::acuity mutable _previous_acuity;
		span mutable _max_range;

		constexpr span get_max_range() const {
			return cancel::quantity_cast<span>(acuity.value() / acuity_loss_per_span);
		}
	};
}
