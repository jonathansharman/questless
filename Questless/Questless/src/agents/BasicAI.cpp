//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
		// Move or turn at random.
		if (random_bool()) {
			ai.walk(ai.being.direction, [&ai](Action::Result result) {
				// Idle next time.
				ai._state = std::make_unique<IdleState>();
				if (result == Action::Result::aborted) {
					// Walk failed. Wait for up to 10 time units instead.
					ai.idle(uniform(0.0, 10.0));
				}
				return Complete{};
			});
		} else {
			auto direction = static_cast<RegionTileCoords::Direction>(uniform(1, 6));
			ai.turn(direction, [&ai](Action::Result result) {
				// Idle next time.
				ai._state = std::make_unique<IdleState>();
				if (result == Action::Result::aborted) {
					// Turn failed. Wait for up to 10 time units instead.
					ai.idle(uniform(0.0, 10.0));
				}
				return Complete{};
			});
		}
	}
	void BasicAI::AttackState::act(BasicAI& ai)
	{
		if (Being* target = game().beings.get(target_id)) {
			auto target_direction = ai.being.coords.direction_towards(target->coords);
			if (ai.being.direction != target_direction) {
				// Facing away from target. Turn towards it.
				ai.turn(target_direction, [&ai](Action::Result result) {
					if (result == Action::Result::aborted) {
						// Turn failed. Wait for up to 10 time units instead.
						ai.idle(uniform(0.0, 10.0));
					}
					return Complete{};
				});
			} else {
				// Facing towards target.
				if (ai.being.coords.distance_to(target->coords) == 1) {
					// Within striking distance of target.
					//! @todo This is a hack that assumes the first item in the inventory is a melee weapon.
					Item& item = *ai.being.inventory.items().begin();
					item.actions().front()->perform(ai.being, [&ai](Action::Result result) {
						if (result == Action::Result::aborted) {
							// Attack failed. Wait for up to 10 time units instead.
							ai.idle(uniform(0.0, 10.0));
						}
						return Complete{};
					});
				} else {
					// Out of range. Move towards target.
					ai.walk(target_direction, [&ai](Action::Result result) {
						if (result == Action::Result::aborted) {
							// Walk failed. Wait for up to 10 time units instead.
							ai.idle(uniform(0.0, 10.0));
						}
						return Complete{};
					});
				}
			}
		} else {
			// Target not found. Switch to idle state.
			ai._state = std::make_unique<IdleState>();
			ai.act();
		}
	}

	Complete BasicAI::send_message
		( uptr<Message> //message
		, std::function<Complete()> cont
		) const
	{
		return cont();
	}

	Complete BasicAI::query_count
		( uptr<CountQuery> query
		, int default
		, std::optional<int> min
		, std::optional<int> max
		, std::function<Complete(std::optional<int>)> cont
		) const
	{
		struct CountQueryHandler : CountQueryConstVisitor
		{
			int default;
			std::optional<int> min;
			std::optional<int> max;
			std::function<Complete(std::optional<int>)> cont;

			CountQueryHandler
				( int default
				, std::optional<int> min
				, std::optional<int> max
				, std::function<Complete(std::optional<int>)> cont
				)
				: default{default}, min{min}, max{max}, cont{std::move(cont)}
			{}
		};

		CountQueryHandler handler{std::move(default), min, max, std::move(cont)};
		query->accept(handler);
		return Complete{};
	}

	Complete BasicAI::query_magnitude
		( uptr<MagnitudeQuery> query
		, double default
		, std::optional<double> min
		, std::optional<double> max
		, std::function<Complete(std::optional<double>)> cont
		) const
	{
		struct MagnitudeQueryHandler : MagnitudeQueryConstVisitor
		{
			double default;
			std::optional<double> min;
			std::optional<double> max;
			std::function<Complete(std::optional<double>)> cont;

			MagnitudeQueryHandler
				( double default
				, std::optional<double> min
				, std::optional<double> max
				, std::function<Complete(std::optional<double>)> cont
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
		return Complete{};
	}

	Complete BasicAI::query_tile
		( uptr<TileQuery> query
		, std::optional<RegionTileCoords> origin
		, std::function<bool(RegionTileCoords)> predicate
		, std::function<Complete(std::optional<RegionTileCoords>)> cont
		) const
	{
		struct TileQueryHandler : TileQueryConstVisitor
		{
			BasicAI const& ai;
			std::optional<RegionTileCoords> origin;
			std::function<bool(RegionTileCoords)> predicate;
			std::function<Complete(std::optional<RegionTileCoords>)> cont;

			TileQueryHandler
				( BasicAI const& ai
				, std::optional<RegionTileCoords> origin
				, std::function<bool(RegionTileCoords)> predicate
				, std::function<Complete(std::optional<RegionTileCoords>)> cont
				)
				: ai{ai}
				, origin{std::move(origin)}
				, predicate{std::move(predicate)}
				, cont{std::move(cont)}
			{}

			void visit(TileQueryRangedAttackTarget const& query) override
			{
				auto target_id = dynamic_cast<AttackState*>(ai._state.get())->target_id;
				if (Being* target = game().beings.get(target_id)) {
					if (ai.being.coords.distance_to(target->coords) <= query.range) {
						// If in range, shoot the target.
						cont(target->coords);
						return;
					}
				}
				cont(std::nullopt);
			}
			void visit(TileQueryLightningBoltTarget const&) override
			{
				cont(std::nullopt);
			}
			void visit(TileQueryTeleportTarget const&) override
			{
				cont(std::nullopt);
			}
		};

		TileQueryHandler handler{*this, std::move(origin), std::move(predicate), std::move(cont)};
		query->accept(handler);
		return Complete{};
	}

	Complete BasicAI::query_direction
		( uptr<DirectionQuery> query
		, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
		) const
	{
		struct DirectionQueryHandler : DirectionQueryConstVisitor
		{
			BasicAI const& ai;
			std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont;

			DirectionQueryHandler
				( BasicAI const& ai
				, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
				)
				: ai{ai}
				, cont{std::move(cont)}
			{}

			void visit(DirectionQueryMeleeAttack const&) override
			{
				auto target_id = dynamic_cast<AttackState*>(ai._state.get())->target_id;
				if (Being* target = game().beings.get(target_id)) {
					// Attack towards the target direction.
					auto direction = ai.being.coords.direction_towards(target->coords);
					cont(direction);
					return;
				}
				cont(std::nullopt);
			}
		};

		DirectionQueryHandler handler{*this, std::move(cont)};
		query->accept(handler);
		return Complete{};
	}

	Complete BasicAI::query_being
		( uptr<BeingQuery> //query
		, std::function<bool(Being&)> //predicate
		, std::function<Complete(std::optional<Being*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	Complete BasicAI::query_item
		( uptr<ItemQuery> //query
		, Being& //source
		, std::function<bool(Being&)> //predicate
		, std::function<Complete(std::optional<Item*>)> cont
		) const
	{
		return cont(std::nullopt);
	}
}
