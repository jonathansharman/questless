//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <optional>

#include "BeingQuery.h"
#include "CountQuery.h"
#include "DirectionQuery.h"
#include "effects/Effect.h"
//#include "entities/beings/Being.h"
#include "entities/beings/WorldView.h"
#include "ItemQuery.h"
#include "items/Item.h"
#include "MagnitudeQuery.h"
#include "Message.h"
#include "TileQuery.h"
#include "VectorQuery.h"

namespace questless
{
	class Being;
	class Gatestone;
	namespace spell
	{
		class Spell;
	}

	//! Facilitates control of a being. Includes AIs and the player.
	class Agent
	{
	public:
		Agent(Being& being) : being{being} {}

		template <typename Derived>
		static auto make(Being& being) { return std::make_unique<Derived>(being); }

		virtual ~Agent() = default;

		Agent& operator =(Agent const&) = delete;

		Being& being;

		//! Chooses and executes an action for the agent's being to perform.
		virtual void act() = 0;

		//! Causes the agent to perceive the given effect, possibly updating its state accordingly.
		//! @param effect The effect to perceive.
		virtual void perceive(sptr<Effect> const& effect) = 0;

		// Queries and messages
		
		//! @todo Update documentation here.

		virtual Complete send_message
			( uptr<Message> message
			, std::function<Complete()> cont
			) const = 0;

		virtual Complete query_count
			( uptr<CountQuery> query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Complete(std::optional<int>)> cont
			) const = 0;

		virtual Complete query_magnitude
			( uptr<MagnitudeQuery>
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Complete(std::optional<double>)> cont
			) const = 0;

		virtual Complete query_tile
			( uptr<TileQuery> query
			, std::optional<RegionTile::Point> origin
			, std::function<bool(RegionTile::Point)> predicate
			, std::function<Complete(std::optional<RegionTile::Point>)> cont
			) const = 0;

		virtual Complete query_direction
			( uptr<DirectionQuery> query
			, std::function<Complete(std::optional<RegionTile::Direction>)> cont
			) const = 0;

		virtual Complete query_vector
			( uptr<VectorQuery> query
			, std::optional<RegionTile::Point> origin
			, std::function<bool(RegionTile::Vector)> predicate
			, std::function<Complete(std::optional<RegionTile::Vector>)> cont
			) const = 0;

		virtual Complete query_being
			( uptr<BeingQuery> query
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Being*>)> cont
			) const = 0;

		virtual Complete query_item
			( uptr<ItemQuery> query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Item*>)> cont
			) const = 0;

		// Quick Time Events

		virtual Complete get_shock_quality(RegionTile::Point target_coords, std::function<Complete(double)> cont) const;

		virtual Complete incant(Gatestone& gatestone, std::function<Complete(uptr<spell::Spell>)> cont) const;
	protected:
		Complete idle(Action::cont_t cont);
		Complete idle(double duration);
		Complete turn(RegionTile::Direction direction, Action::cont_t cont);
		Complete walk(RegionTile::Direction direction, Action::cont_t cont);
		Complete fly();
	};
}
