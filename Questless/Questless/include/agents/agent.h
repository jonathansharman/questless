//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <optional>

#include "being_query.h"
#include "count_query.h"
#include "direction_query.h"
#include "effects/effect.h"
#include "entities/beings/world_view.h"
#include "item_query.h"
#include "items/item.h"
#include "magnitude_query.h"
#include "message.h"
#include "tile_query.h"
#include "vector_query.h"

namespace ql
{
	class being;
	class gatestone;
	namespace magic
	{
		class spell;
	}

	//! Facilitates control of a being. Includes AIs and the player.
	class agent
	{
	public:
		agent(being& being) : being{being} {}

		template <typename Derived>
		static auto make(being& being) { return std::make_unique<Derived>(being); }

		virtual ~agent() = default;

		agent& operator =(agent const&) = delete;

		being& being;

		//! Chooses and executes an action for the agent's being to perform.
		virtual void act() = 0;

		//! Causes the agent to perceive the given effect, possibly updating its state accordingly.
		//! @param effect The effect to perceive.
		virtual void perceive(sptr<effect> const& effect) = 0;

		// Queries and messages
		
		//! @todo Update documentation here.

		virtual complete send_message
			( uptr<message> message
			, std::function<complete()> cont
			) const = 0;

		virtual complete query_count
			( uptr<count_query> query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<complete(std::optional<int>)> cont
			) const = 0;

		virtual complete query_magnitude
			( uptr<magnitude_query>
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<complete(std::optional<double>)> cont
			) const = 0;

		virtual complete query_tile
			( uptr<tile_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const = 0;

		virtual complete query_direction
			( uptr<direction_query> query
			, std::function<complete(std::optional<region_tile::direction>)> cont
			) const = 0;

		virtual complete query_vector
			( uptr<vector_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const = 0;

		virtual complete query_being
			( uptr<being_query> query
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<ql::being*>)> cont
			) const = 0;

		virtual complete query_item
			( uptr<item_query> query
			, ql::being& source
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<item*>)> cont
			) const = 0;

		// Quick Time Events

		virtual complete get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) const;

		virtual complete incant(gatestone& gatestone, std::function<complete(uptr<magic::spell>)> cont) const;
	protected:
		complete idle(action::cont cont);
		complete idle(double duration);
		complete turn(region_tile::direction direction, action::cont cont);
		complete walk(region_tile::direction direction, action::cont cont);
		complete fly();
	};
}
