/**
* @file    BasicAI.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BasicAI class.
*/

#include "agents/BasicAI.h"
#include "Game.h"

using std::function;

namespace questless
{
	void BasicAI::act()
	{
		_state->act(*this);
	}
	void BasicAI::IdleState::act(BasicAI& ai)
	{
		// Wait for up to 10 time units.
		ai.idle(uniform(0.0, 10.0));
		// Walk next time.
		ai._state = std::make_unique<WalkState>();
	}
	void BasicAI::WalkState::act(BasicAI& ai)
	{
		// Move to or turn towards a random neighbor hex, favoring movement.
		auto direction = uniform(0, 1) ? ai.being.direction : static_cast<RegionTileCoords::Direction>(uniform(1, 6));
		ai.walk(direction, [&ai](Action::Result result) {
			// Idle next time.
			ai._state = std::make_unique<IdleState>();
			if (result == Action::Result::aborted) {
				// Walk failed. Wait for up to 10 time units instead.
				ai.idle(uniform(0.0, 10.0));
			}
			return Action::Complete{};
		});
	}
	void BasicAI::AttackState::act(BasicAI& ai)
	{
		if (Being* target = game().being(target_id)) {
			if (ai.being.coords.distance_to(target->coords) == 1) {
				// Within striking distance of target.
				/// @todo This is a hack that assumes the first item in the inventory is a melee weapon.
				Item* item = ai.being.inventory().items().head();
				item->actions().front()->perform(ai.being, [&ai](Action::Result result) {
					if (result == Action::Result::aborted) {
						// Attack failed. Wait for up to 10 time units instead.
						ai.idle(uniform(0.0, 10.0));
					}
					return Action::Complete{};
				});
			} else {
				// Out of range. Move towards target.
				auto direction = ai.being.coords.direction_towards(target->coords);
				ai.walk(direction, [&ai](Action::Result result) {
					if (result == Action::Result::aborted) {
						// Walk failed. Wait for up to 10 time units instead.
						ai.idle(uniform(0.0, 10.0));
					}
					return Action::Complete{};
				});
			}
		} else {
			// Target not found. Switch to idle state.
			ai._state = std::make_unique<IdleState>();
			ai.act();
		}
	}

	Action::Complete BasicAI::send_message
		( Message::ptr //message
		, std::function<Action::Complete()> cont
		) const
	{
		return cont();
	}

	Action::Complete BasicAI::query_count
		( CountQuery::ptr query
		, int default
		, boost::optional<int> min
		, boost::optional<int> max
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		struct CountQueryHandler : CountQueryVisitor
		{
			int default;
			boost::optional<int> min;
			boost::optional<int> max;
			std::function<Action::Complete(boost::optional<int>)> cont;

			CountQueryHandler
				( int default
				, boost::optional<int> min
				, boost::optional<int> max
				, std::function<Action::Complete(boost::optional<int>)> cont
				)
				: default{default}, min{min}, max{max}, cont{std::move(cont)}
			{}
		};

		CountQueryHandler handler{std::move(default), min, max, std::move(cont)};
		query->accept(handler);
		return Action::Complete{};
	}

	Action::Complete BasicAI::query_magnitude
		( MagnitudeQuery::ptr query
		, double default
		, boost::optional<double> min
		, boost::optional<double> max
		, std::function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		struct MagnitudeQueryHandler : MagnitudeQueryVisitor
		{
			double default;
			boost::optional<double> min;
			boost::optional<double> max;
			std::function<Action::Complete(boost::optional<double>)> cont;

			MagnitudeQueryHandler
				( double default
				, boost::optional<double> min
				, boost::optional<double> max
				, std::function<Action::Complete(boost::optional<double>)> cont
				)
				: default{default}, min{min}, max{max}, cont{std::move(cont)}
			{}

			void visit(MagnitudeQueryWaitTime const&) override
			{
				cont(default);
			}
			void visit(MagnitudeQueryLightningBolt const&) override
			{
				cont(default);
			}
			void visit(MagnitudeQueryHeal const&) override
			{
				cont(default);
			}
		};

		MagnitudeQueryHandler handler{std::move(default), min, max, std::move(cont)};
		query->accept(handler);
		return Action::Complete{};
	}

	Action::Complete BasicAI::query_tile
		( TileQuery::ptr query
		, boost::optional<RegionTileCoords> origin
		, std::function<bool(RegionTileCoords)> predicate
		, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		struct TileQueryHandler : TileQueryVisitor
		{
			BasicAI const& ai;
			boost::optional<RegionTileCoords> origin;
			std::function<bool(RegionTileCoords)> predicate;
			std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont;

			TileQueryHandler
				( BasicAI const& ai
				, boost::optional<RegionTileCoords> origin
				, std::function<bool(RegionTileCoords)> predicate
				, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
				)
				: ai{ai}
				, origin{std::move(origin)}
				, predicate{std::move(predicate)}
				, cont{std::move(cont)}
			{}

			void visit(TileQueryMeleeTarget const&) override
			{
				auto target_id = dynamic_cast<AttackState*>(ai._state.get())->target_id;
				if (Being* target = game().being(target_id)) {
					// Attack towards the target.
					auto direction = ai.being.coords.direction_towards(target->coords);
					cont(origin->neighbor(direction));
					return;
				}
				cont(boost::none);
			}
			void visit(TileQueryRangedTarget const& query) override
			{
				auto target_id = dynamic_cast<AttackState*>(ai._state.get())->target_id;
				if (Being* target = game().being(target_id)) {
					if (ai.being.coords.distance_to(target->coords) <= query.range) {
						// If in range, shoot the target.
						cont(target->coords);
						return;
					}
				}
				cont(boost::none);
			}
			void visit(TileQueryLightningBoltTarget const&) override
			{
				cont(boost::none);
			}
			void visit(TileQueryTeleportTarget const&) override
			{
				cont(boost::none);
			}
		};

		TileQueryHandler handler{*this, std::move(origin), std::move(predicate), std::move(cont)};
		query->accept(handler);
		return Action::Complete{};
	}

	Action::Complete BasicAI::query_being
		( BeingQuery::ptr //query
		, std::function<bool(Being&)> //predicate
		, std::function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_item
		( ItemQuery::ptr //query
		, Being& //source
		, std::function<bool(Being&)> //predicate
		, std::function<Action::Complete(boost::optional<Item*>)> cont
		) const
	{
		return cont(boost::none);
	}
}
