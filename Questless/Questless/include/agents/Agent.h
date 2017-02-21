/**
* @file    Agent.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Agent abstract base class, representing an agent acting on behalf of some number of beings. E.g., the player agent.
*/

#pragma once

#include <functional>
#include <stdexcept>

#include <boost/optional.hpp>

#include "Message.h"
#include "CountQuery.h"
#include "MagnitudeQuery.h"
#include "TileQuery.h"
#include "BeingQuery.h"
#include "ItemQuery.h"
#include "entities/beings/Being.h"
#include "entities/beings/WorldView.h"
#include "items/Item.h"
#include "effects/Effect.h"
#include "units/Point.h" /// @todo This shouldn't be needed in the base class.

namespace questless
{
	class Agent
	{
	public:
		using ptr = std::unique_ptr<Agent>;

		Agent(Being& being) : being{being} {}

		template <typename Derived>
		static ptr make(Being& being) { return std::make_unique<Derived>(being); }

		virtual ~Agent() = default;

		Agent& operator =(Agent const& other) = delete;

		Being& being;

		/// Chooses and executes an action for the agent's being to perform.
		virtual void act() = 0;

		/// Causes the agent to perceive the given effect, possibly updating its state accordingly.
		/// @param effect The effect to perceive.
		virtual void perceive(Effect::ptr const& effect) = 0;

		// Queries and messages
		
		/// @todo Update documentation here.

		virtual Action::Complete send_message
			( Message::ptr message
			, std::function<Action::Complete()> cont
			) const = 0;

		virtual Action::Complete query_count
			( CountQuery::ptr query
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;

		virtual Action::Complete query_magnitude
			( MagnitudeQuery::ptr
			, double default
			, boost::optional<double> min
			, boost::optional<double> max
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const = 0;

		virtual Action::Complete query_tile
			( TileQuery::ptr query
			, boost::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const = 0;

		virtual Action::Complete query_being
			( BeingQuery::ptr query
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const = 0;

		virtual Action::Complete query_item
			( ItemQuery::ptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const = 0;

		// Quick Time Events

		virtual Action::Complete get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Action::Complete(double)> cont) const;
	protected:
		Action::Complete idle(Action::cont_t cont);
		Action::Complete idle(double duration);
		Action::Complete walk(RegionTileCoords::Direction direction, Action::cont_t cont);
		Action::Complete fly();
	};
}
