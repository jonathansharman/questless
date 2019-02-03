//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"
#include "utility/id.hpp"

namespace ql {
	//! A basic AI, suitable for most NPCs.
	struct basic_ai : agent {
		basic_ai(ql::being& being) : agent{being} {}

		complete act() final;

		void perceive(effects::effect const& effect) final;

		// Queries and messages

		complete send_message(queries::message::query message, std::function<complete()> cont) final;

		complete query_count(queries::count::query query,
			int default_value,
			std::optional<int> min,
			std::optional<int> max,
			std::function<complete(std::optional<int>)> cont) final;

		complete query_magnitude(queries::magnitude::query query,
			double default_value,
			std::optional<double> min,
			std::optional<double> max,
			std::function<complete(std::optional<double>)> cont) final;

		complete query_tile(queries::tile::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::point)> predicate,
			std::function<complete(std::optional<region_tile::point>)> cont) final;

		complete query_direction(queries::direction::query query,
			std::function<complete(std::optional<region_tile::direction>)> cont) final;

		complete query_vector(queries::vector::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::vector)> predicate,
			std::function<complete(std::optional<region_tile::vector>)> cont) final;

		complete query_being(queries::being::query query,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<ql::being*>)> cont) final;

		complete query_item(queries::item::query query,
			ql::being& source,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<item*>)> cont) final;

	private:
		friend class state;
		struct state {
			virtual ~state() = default;
			virtual complete act(basic_ai& ai) = 0;
		};
		struct idle_state : state {
			complete act(basic_ai& ai) final;
		};
		struct walk_state : state {
			complete act(basic_ai& ai) final;
		};
		struct attack_state : state {
			id<ql::being> target_id;
			attack_state(id<ql::being> target_id) : target_id{target_id} {}
			complete act(basic_ai& ai) final;
		};

		uptr<state> _state = umake<idle_state>();
	};
}
