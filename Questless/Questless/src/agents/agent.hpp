//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "action.hpp"
#include "queries/all.hpp"

#include "magic/spell.hpp"
#include "quantities/misc.hpp"
#include "utility/complete.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include <functional>
#include <optional>

namespace ql {
	namespace effects {
		struct effect;
	}
	struct being;
	struct body_part;
	struct item;
	struct gatestone;

	//! Facilitates control of a being. Includes AIs and the player.
	struct agent {
		agent(being& being) : being{being} {}

		template <typename Derived>
		static auto make(being& being) {
			return umake<Derived>(being);
		}

		virtual ~agent() = default;

		agent& operator=(agent const&) = delete;

		being& being;

		//! Chooses and executes an action for the agent's being to perform.
		virtual complete act() = 0;

		//! Causes the agent to perceive the given effect, possibly updating its state accordingly.
		//! @param effect The effect to perceive.
		virtual void perceive(effects::effect const& effect) = 0;

		// Queries and messages

		//! @todo Update documentation here.

		virtual complete send_message(queries::message::query message, std::function<complete()> cont) = 0;

		virtual complete query_count(queries::count::query query,
			int default_value,
			std::optional<int> min,
			std::optional<int> max,
			std::function<complete(std::optional<int>)> cont) = 0;

		virtual complete query_magnitude(queries::magnitude::query,
			double default_value,
			std::optional<double> min,
			std::optional<double> max,
			std::function<complete(std::optional<double>)> cont) = 0;

		virtual complete query_tile(queries::tile::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::point)> predicate,
			std::function<complete(std::optional<region_tile::point>)> cont) = 0;

		virtual complete query_direction(queries::direction::query query,
			std::function<complete(std::optional<region_tile::direction>)> cont) = 0;

		virtual complete query_vector(queries::vector::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::vector)> predicate,
			std::function<complete(std::optional<region_tile::vector>)> cont) = 0;

		virtual complete query_being(queries::being::query query,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<ql::being*>)> cont) = 0;

		virtual complete query_item(queries::item::query query,
			ql::being& source,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<item*>)> cont) = 0;

		// Quick Time Events

		virtual complete aim_missile(region_tile::point source_coords,
			ql::being& target_being,
			std::function<complete(body_part*)> cont);

		virtual complete get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont);

		virtual complete incant(gatestone& gatestone, std::function<complete(std::optional<magic::spell> const&)> cont);

	protected:
		complete idle(action::cont cont);
		complete idle(tick duration);
		complete turn(region_tile::direction direction, action::cont cont);
		complete walk(region_tile::direction direction, action::cont cont);
		complete fly();
	};
}
