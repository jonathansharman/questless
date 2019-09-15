//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "perception.hpp"

#include "entities/beings/body.hpp"
#include "world/region.hpp"

#include <range/v3/algorithm/max.hpp>
#include <range/v3/view/transform.hpp>

namespace ql {
	namespace {
		static constexpr auto perception_loss_per_pace = 10_perception / 1_pace;
	}

	auto max_visual_range(std::vector<stats::vision> const& vision_sources) -> pace {
		if (vision_sources.empty()) { return 0_pace; }
		auto acuities = vision_sources | ranges::views::transform([](stats::vision const& v) { return v.acuity.value(); });
		return ranges::max(acuities) / perception_loss_per_pace;
	}

	//! Determines whether @p target is in the field of vision specified by @p location and @p direction.
	auto inside_field_of_vision(location const& location, tile_hex::direction direction, tile_hex::point target) -> bool {
		auto offset = target - location.coords;
		switch (direction) {
			case tile_hex::direction::dr:
				return offset.q >= 0_pace && offset.q + offset.r >= 0_pace;
			case tile_hex::direction::d:
				return offset.r >= 0_pace && offset.q + offset.r >= 0_pace;
			case tile_hex::direction::dl:
				return offset.q <= 0_pace && offset.r >= 0_pace;
			case tile_hex::direction::ul:
				return offset.q <= 0_pace && offset.q + offset.r <= 0_pace;
			case tile_hex::direction::u:
				return offset.r <= 0_pace && offset.q + offset.r <= 0_pace;
			case tile_hex::direction::ur:
				return offset.q >= 0_pace && offset.r <= 0_pace;
			default:
				UNREACHABLE;
		}
	}

	//! Computes the perception @p vision is capable of, factoring in the @p illuminance.
	auto light_adjusted_perception(stats::vision const& vision, lum illuminance) -> perception {
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

	auto perception_of(id perceptor_id, tile_hex::point target) -> perception {
		auto const& body = reg.get<ql::body>(perceptor_id);

		// Check that the perceptor has at least one source of vision.
		if (body.stats.a.vision_sources.cur.empty()) { return 0_perception; }

		auto const location = reg.get<ql::location>(perceptor_id);

		// Check that the target is within the field of vision.
		if (!inside_field_of_vision(location, body.cond.direction, target)) { return 0_perception; }

		// Check that the target within the maximum possible visual range.
		if ((target - location.coords).length() > max_visual_range(body.stats.a.vision_sources.cur)) {
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
		pace const distance = (target - location.coords).length();
		auto const best_distance_adjusted_perception = best_light_adjusted_perception - distance * perception_loss_per_pace;

		// Account for occlusions between the perceptor and the target.
		double const occlusion = region.occlusion(location.coords, target);
		auto const best_perception = cancel::quantity_cast<perception>((1.0 - occlusion) * best_distance_adjusted_perception);

		return std::max(0_perception, best_perception);
	}
}
