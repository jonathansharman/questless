//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/basic_ai.hpp"

#include "agents/queries/all_queries.hpp"
#include "effects/effect.hpp"
#include "game.hpp"
#include "utility/random.hpp"

namespace ql
{
	complete basic_ai::act()
	{
		return _state->act(*this);
	}
	complete basic_ai::idle_state::act(basic_ai& ai)
	{
		// Walk next time.
		ai._state = umake<walk_state>();
		// Wait for up to 10 time units.
		return ai.idle(uniform(0.0, 10.0));
	}
	complete basic_ai::walk_state::act(basic_ai& ai)
	{
		// Move or turn at random.
		if (random_bool()) {
			return ai.walk(ai.being.direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = umake<idle_state>();
				if (result == action::result::aborted) {
					// Walk failed. Wait for up to 10 time units instead.
					return ai.idle(uniform(0.0, 10.0));
				}
				return complete{};
			});
		} else {
			auto direction = static_cast<region_tile::direction>(uniform(1, 6));
			return ai.turn(direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = umake<idle_state>();
				if (result == action::result::aborted) {
					// Turn failed. Wait for up to 10 time units instead.
					return ai.idle(uniform(0.0, 10.0));
				}
				return complete{};
			});
		}
	}
	complete basic_ai::attack_state::act(basic_ai& ai)
	{
		if (ql::being* target = the_game().beings.ptr(target_id)) {
			if (ai.being.perception_of(target->coords).category() == perception::category::none) {
				// Target not visible. Switch to idle state.
				ai._state = umake<idle_state>();
				return ai.act();

				//! @todo Only go passive while target is out of visual range. Keep a grudge list?
			} else {
				auto target_direction = (target->coords - ai.being.coords).direction();
				if (ai.being.direction != target_direction) {
					// Facing away from target. Turn towards it.
					return ai.turn(target_direction, [&ai](action::result result) {
						if (result == action::result::aborted) {
							// Turn failed. Wait for up to 10 time units instead.
							return ai.idle(uniform(0.0, 10.0));
						}
						return complete{};
					});
				} else {
					// Facing towards target.
					if ((target->coords - ai.being.coords).length() == 1) {
						// Within striking distance of target.
						//! @todo This is a hack that assumes the first item in the inventory is a melee weapon.
						item& item = *ai.being.inventory.items.begin();
						return item.actions().front()->perform(ai.being, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Attack failed. Wait for up to 10 time units instead.
								return ai.idle(uniform(0.0, 10.0));
							}
							return complete{};
						});
					} else {
						// Out of range. Move towards target.
						return ai.walk(target_direction, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Walk failed. Wait for up to 10 time units instead.
								return ai.idle(uniform(0.0, 10.0));
							}
							return complete{};
						});
					}
				}
			}
		} else {
			// Target not found. Switch to idle state.
			ai._state = umake<idle_state>();
			return ai.act();
		}
	}

	void basic_ai::perceive(sptr<effect> const& effect) { effect->accept(*this); }

	complete basic_ai::send_message
		( uptr<message> //message
		, std::function<complete()> cont
		) const
	{
		return cont();
	}

	complete basic_ai::query_count
		( uptr<count_query> query
		, int default_value
		, std::optional<int> min
		, std::optional<int> max
		, std::function<complete(std::optional<int>)> cont
		) const
	{
		struct count_query_handler : count_query_const_visitor
		{
			int default_value;
			std::optional<int> min;
			std::optional<int> max;
			std::function<complete(std::optional<int>)> cont;

			count_query_handler
				( int default_value
				, std::optional<int> min
				, std::optional<int> max
				, std::function<complete(std::optional<int>)> cont
				)
				: default_value{default_value}, min{min}, max{max}, cont{std::move(cont)}
			{}
		};

		count_query_handler handler{std::move(default_value), min, max, std::move(cont)};
		query->accept(handler);
		return complete{};
	}

	complete basic_ai::query_magnitude
		( uptr<magnitude_query> query
		, double default_value
		, std::optional<double> min
		, std::optional<double> max
		, std::function<complete(std::optional<double>)> cont
		) const
	{
		struct magnitude_query_handler : magnitude_query_const_visitor
		{
			double default_value;
			std::optional<double> min;
			std::optional<double> max;
			std::function<complete(std::optional<double>)> cont;
			complete result;

			magnitude_query_handler
				( double default_value
				, std::optional<double> min
				, std::optional<double> max
				, std::function<complete(std::optional<double>)> cont
				)
				: default_value{default_value}, min{min}, max{max}, cont{std::move(cont)}
			{}

			void visit(magnitude_query_wait_time const&) final
			{
				result = cont(default_value);
			}
			void visit(magnitude_query_shock const&) final
			{
				result = cont(default_value);
			}
			void visit(magnitude_query_heal const&) final
			{
				result = cont(default_value);
			}
		};

		magnitude_query_handler handler{std::move(default_value), min, max, std::move(cont)};
		query->accept(handler);
		return handler.result;
	}

	complete basic_ai::query_tile
		( uptr<tile_query> query
		, std::optional<region_tile::point> origin
		, std::function<bool(region_tile::point)> predicate
		, std::function<complete(std::optional<region_tile::point>)> cont
		) const
	{
		struct tile_query_handler : tile_query_const_visitor
		{
			basic_ai const& ai;
			std::optional<region_tile::point> origin;
			std::function<bool(region_tile::point)> predicate;
			std::function<complete(std::optional<region_tile::point>)> cont;

			tile_query_handler
				( basic_ai const& ai
				, std::optional<region_tile::point> origin
				, std::function<bool(region_tile::point)> predicate
				, std::function<complete(std::optional<region_tile::point>)> cont
				)
				: ai{ai}
				, origin{std::move(origin)}
				, predicate{std::move(predicate)}
				, cont{std::move(cont)}
			{}

			void visit(tile_query_ranged_attack_target const& query) final
			{
				auto target_id = dynamic_cast<attack_state*>(ai._state.get())->target_id;
				if (ql::being* target = the_game().beings.ptr(target_id)) {
					if ((target->coords - ai.being.coords).length() <= query.range) {
						// If in range, shoot the target.
						cont(target->coords);
						return;
					}
				}
				cont(std::nullopt);
			}
			void visit(tile_query_shock_target const&) final
			{
				cont(std::nullopt);
			}
			void visit(tile_query_teleport_target const&) final
			{
				cont(std::nullopt);
			}
		};

		tile_query_handler handler{*this, std::move(origin), std::move(predicate), std::move(cont)};
		query->accept(handler);
		return complete{};
	}

	complete basic_ai::query_direction
		( uptr<direction_query> query
		, std::function<complete(std::optional<region_tile::direction>)> cont
		) const
	{
		struct direction_query_handler : direction_query_const_visitor
		{
			basic_ai const& ai;
			std::function<complete(std::optional<region_tile::direction>)> cont;

			direction_query_handler
				( basic_ai const& ai
				, std::function<complete(std::optional<region_tile::direction>)> cont
				)
				: ai{ai}
				, cont{std::move(cont)}
			{}
		};

		direction_query_handler handler{*this, std::move(cont)};
		query->accept(handler);
		return complete{};
	}

	complete basic_ai::query_vector
		( uptr<vector_query> query
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::vector)> predicate
		, std::function<complete(std::optional<region_tile::vector>)> cont
		) const
	{
		struct vector_query_handler : vector_query_const_visitor
		{
			basic_ai const& ai;
			std::function<complete(std::optional<region_tile::vector>)> cont;

			vector_query_handler
				( basic_ai const& ai
				, std::function<complete(std::optional<region_tile::vector>)> cont
				)
				: ai{ai}
				, cont{std::move(cont)}
			{}

			void visit(vector_query_melee_attack const&) final
			{
				auto target_id = dynamic_cast<attack_state*>(ai._state.get())->target_id;
				if (ql::being* target = the_game().beings.ptr(target_id)) {
					// Attack towards the target.
					cont((target->coords - ai.being.coords).unit());
					return;
				}
				cont(std::nullopt);
			}
		};

		vector_query_handler handler{*this, std::move(cont)};
		query->accept(handler);
		return complete{};
	}

	complete basic_ai::query_being
		( uptr<being_query> //query
		, std::function<bool(ql::being&)> //predicate
		, std::function<complete(std::optional<ql::being*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete basic_ai::query_item
		( uptr<item_query> //query
		, ql::being& //source
		, std::function<bool(ql::being&)> //predicate
		, std::function<complete(std::optional<item*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	////////////////////////////
	// Effect Visitor Methods //
	////////////////////////////

	void basic_ai::visit(injury_effect const& effect)
	{
		// Retaliate against injuries.
		if (effect.opt_source_id && effect.target_being_id == being.id) {
			_state = umake<attack_state>(*effect.opt_source_id);
		}
	}
}
