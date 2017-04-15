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

#include <optional>

#include "Message.h"
#include "CountQuery.h"
#include "MagnitudeQuery.h"
#include "TileQuery.h"
#include "DirectionQuery.h"
#include "BeingQuery.h"
#include "ItemQuery.h"
#include "entities/beings/Being.h"
#include "entities/beings/WorldView.h"
#include "items/Item.h"
#include "effects/Effect.h"

namespace questless
{
	class Agent
	{
	public:
		using uptr = std::unique_ptr<Agent>;

		Agent(Being& being) : being{being} {}

		template <typename Derived>
		static uptr make(Being& being) { return std::make_unique<Derived>(being); }

		virtual ~Agent() = default;

		Agent& operator =(Agent const&) = delete;

		Being& being;

		/// Chooses and executes an action for the agent's being to perform.
		virtual void act() = 0;

		/// Causes the agent to perceive the given effect, possibly updating its state accordingly.
		/// @param effect The effect to perceive.
		virtual void perceive(Effect::uptr const& effect) = 0;

		// Queries and messages
		
		/// @todo Update documentation here.

		virtual Complete send_message
			( Message::uptr message
			, std::function<Complete()> cont
			) const = 0;

		virtual Complete query_count
			( CountQuery::uptr query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Complete(std::optional<int>)> cont
			) const = 0;

		virtual Complete query_magnitude
			( MagnitudeQuery::uptr
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Complete(std::optional<double>)> cont
			) const = 0;

		virtual Complete query_tile
			( TileQuery::uptr query
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Complete(std::optional<RegionTileCoords>)> cont
			) const = 0;

		virtual Complete query_direction
			( DirectionQuery::uptr query
			, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
			) const = 0;

		virtual Complete query_being
			( BeingQuery::uptr query
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Being*>)> cont
			) const = 0;

		virtual Complete query_item
			( ItemQuery::uptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Item*>)> cont
			) const = 0;

		// Quick Time Events

		virtual Complete get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Complete(double)> cont) const;
	protected:
		Complete idle(Action::cont_t cont);
		Complete idle(double duration);
		Complete turn(RegionTileCoords::Direction direction, Action::cont_t cont);
		Complete walk(RegionTileCoords::Direction direction, Action::cont_t cont);
		Complete fly();
	};
}
