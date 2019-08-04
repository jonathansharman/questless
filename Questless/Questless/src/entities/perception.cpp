//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "perception.hpp"

#include "entities/beings/body.hpp"
#include "world/region.hpp"

namespace ql {
	namespace {
		static constexpr auto perception_loss_per_span = 10_perception / 1_span;
	}

	span max_visual_range(std::vector<stats::vision> const& vision_sources) {
		auto max_element = std::max_element( //
			vision_sources.begin(),
			vision_sources.end(),
			[](stats::vision const& v1, stats::vision const& v2) { return v1.acuity < v2.acuity; });
		return max_element->acuity.value() / perception_loss_per_span;
	}

	//! Determines whether @p target is in the field of vision specified by @p location and @p direction.
	bool inside_field_of_vision(location const& location, region_tile::direction direction, region_tile::point target) {
		auto offset = target - location.coords;
		switch (direction) {
			case region_tile::direction::one:
				return offset.q >= 0_span && offset.q + offset.r >= 0_span;
			case region_tile::direction::two:
				return offset.r >= 0_span && offset.q + offset.r >= 0_span;
			case region_tile::direction::three:
				return offset.q <= 0_span && offset.r >= 0_span;
			case region_tile::direction::four:
				return offset.q <= 0_span && offset.q + offset.r <= 0_span;
			case region_tile::direction::five:
				return offset.r <= 0_span && offset.q + offset.r <= 0_span;
			case region_tile::direction::six:
				return offset.q >= 0_span && offset.r <= 0_span;
			default:
				UNREACHABLE;
		}
	}

	//! Computes the perception @p vision is capable of, factoring in the @p illuminance.
	perception light_adjusted_perception(stats::vision const& vision, lum illuminance) {
		if (illuminance < vision.min_illuminance.value()) {
			// Too dark.
			return vision.acuity - (vision.min_illuminance.value() - illuminance) * vision.darkness_penalty.value();
		} else if (illuminance > vision.max_illuminance.value()) {
			// Too bright.
			return vision.acuity - (illuminance - vision.max_illuminance.value()) * vision.glare_penalty.value();
		} else {
			// Within ideal illuminance range.
			return vision.acuity;
		}
	}

	perception perception_of(id perceptor_id, region_tile::point target) {
		auto const& body = reg.get<ql::body>(perceptor_id);

		// Check that the perceptor has at least one source of vision.
		if (body.stats.a.vision_sources.cur.empty()) { return 0_perception; }

		auto const location = reg.get<ql::location>(perceptor_id);

		// Check that the target is within the field of vision.
		if (!inside_field_of_vision(location, body.cond.direction, target)) { return 0_perception; }

		// Check that the target within the maximum possible visual range.
		if ((target - location.coords).length() < max_visual_range(body.stats.a.vision_sources.cur)) {
			return 0_perception;
		}

		auto const& region = reg.get<ql::region>(location.region_id);

		// Find the best possible visual perception, factoring in the light level at the target.
		auto const illuminance = region.illuminance(target);
		auto const best_light_adjusted_perception = std::reduce( //
			body.stats.a.vision_sources.cur.begin(),
			body.stats.a.vision_sources.cur.end(),
			0_perception,
			[&](perception acc, stats::vision v) { return std::max(acc, light_adjusted_perception(v, illuminance)); });

		// Account for distance.
		span const distance{(target - location.coords).length()};
		auto const best_distance_adjusted_perception = best_light_adjusted_perception - distance * perception_loss_per_span;

		// Account for occlusions between the perceptor and the target.
		double const occlusion = region.occlusion(location.coords, target);
		auto const best_perception = cancel::quantity_cast<perception>((1.0 - occlusion) * best_distance_adjusted_perception);

		return best_perception;
	}
}
