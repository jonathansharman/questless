//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/basic_ai.h"

#include "game.h"

using std::function;

namespace ql
{
	void basic_ai::act()
	{
		_state->act(*this);
	}
	void basic_ai::idle_state::act(basic_ai& ai)
	{
		// Wait for up to 10 time units.
		ai.idle(uniform(0.0, 10.0));
		// Walk next time.
		ai._state = std::make_unique<walk_state>();
	}
	void basic_ai::walk_state::act(basic_ai& ai)
	{
		// Move or turn at random.
		if (random_bool()) {
			ai.walk(ai.being.direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = std::make_unique<idle_state>();
				if (result == action::result::aborted) {
					// Walk failed. Wait for up to 10 time units instead.
					ai.idle(uniform(0.0, 10.0));
				}
				return complete{};
			});
		} else {
			auto direction = static_cast<region_tile::direction>(uniform(1, 6));
			ai.turn(direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = std::make_unique<idle_state>();
				if (result == action::result::aborted) {
					// Turn failed. Wait for up to 10 time units instead.
					ai.idle(uniform(0.0, 10.0));
				}
				return complete{};
			});
		}
	}
	void basic_ai::attack_state::act(basic_ai& ai)
	{
		if (ql::being* target = the_game().beings.ptr(target_id)) {
			if (ai.being.perception_of(target->coords).category() == perception::category::none) {
				// Target not visible. Switch to idle state.
				ai._state = std::make_unique<idle_state>();
				ai.act();

				//! @todo Only go passive while target is out of visual range. Keep a grudge list?
			} else {
				auto target_direction = (target->coords - ai.being.coords).direction();
				if (ai.being.direction != target_direction) {
					// Facing away from target. Turn towards it.
					ai.turn(target_direction, [&ai](action::result result) {
						if (result == action::result::aborted) {
							// Turn failed. Wait for up to 10 time units instead.
							ai.idle(uniform(0.0, 10.0));
						}
						return complete{};
					});
				} else {
					// Facing towards target.
					if ((target->coords - ai.being.coords).length() == 1) {
						// Within striking distance of target.
						//! @todo This is a hack that assumes the first item in the inventory is a melee weapon.
						item& item = *ai.being.inventory.items.begin();
						item.actions().front()->perform(ai.being, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Attack failed. Wait for up to 10 time units instead.
								ai.idle(uniform(0.0, 10.0));
							}
							return complete{};
						});
					} else {
						// Out of range. Move towards target.
						ai.walk(target_direction, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Walk failed. Wait for up to 10 time units instead.
								ai.idle(uniform(0.0, 10.0));
							}
							return complete{};
						});
					}
				}
			}
		} else {
			// Target not found. Switch to idle state.
			ai._state = std::make_unique<idle_state>();
			ai.act();
		}
	}

	complete basic_ai::send_message
		( uptr<message> //message
		, std::function<complete()> cont
		) const
	{
		return cont();
	}

	complete basic_ai::query_count
		( uptr<count_query> query
		, int default
		, std::optional<int> min
		, std::optional<int> max
		, std::function<complete(std::optional<int>)> cont
		) const
	{
		struct count_query_handler : count_query_const_visitor
		{
			int default;
			std::optional<int> min;
			std::optional<int> max;
			std::function<complete(std::optional<int>)> cont;

			count_query_handler
				( int default
				, std::optional<int> min
				, std::optional<int> max
				, std::function<complete(std::optional<int>)> cont
				)
				: default{default}, min{min}, max{max}, cont{std::move(cont)}
			{}
		};

		count_query_handler handler{std::move(default), min, max, std::move(cont)};
		query->accept(handler);
		return complete{};
	}

	complete basic_ai::query_magnitude
		( uptr<magnitude_query> query
		, double default
		, std::optional<double> min
		, std::optional<double> max
		, std::function<complete(std::optional<double>)> cont
		) const
	{
		struct magnitude_query_handler : magnitude_query_const_visitor
		{
			double default;
			std::optional<double> min;
			std::optional<double> max;
			std::function<complete(std::optional<double>)> cont;

			magnitude_query_handler
				( double default
				, std::optional<double> min
				, std::optional<double> max
				, std::function<complete(std::optional<double>)> cont
				)
				: default{default}, min{min}, max{max}, cont{std::move(cont)}
			{}

			void visit(magnitude_query_wait_time const&) final
			{
				cont(default);
			}
			void visit(magnitude_query_shock const&) final
			{
				cont(default);
			}
			void visit(magnitude_query_heal const&) final
			{
				cont(default);
			}
		};

		magnitude_query_handler handler{std::move(default), min, max, std::move(cont)};
		query->accept(handler);
		return complete{};
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
		if (effect.opt_source_id && effect.target_id == being.id) {
			_state = std::make_unique<attack_state>(*effect.opt_source_id);
		}
	}
}
