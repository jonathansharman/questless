//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "basic_ai.hpp"

#include "effects/effect.hpp"
#include "game.hpp"
#include "utility/random.hpp"
#include "utility/type_switch.hpp"

namespace ql {
	complete basic_ai::act() {
		return _state->act(*this);
	}
	complete basic_ai::idle_state::act(basic_ai& ai) {
		// Walk next time.
		ai._state = umake<walk_state>();
		// Wait for up to 10 time units.
		return ai.idle(ticks{uniform(0.0, 10.0)});
	}
	complete basic_ai::walk_state::act(basic_ai& ai) {
		// Move or turn at random.
		if (random_bool()) {
			return ai.walk(ai.being.direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = umake<idle_state>();
				if (result == action::result::aborted) {
					// Walk failed. Wait for up to 10 time units instead.
					return ai.idle(ticks{uniform(0.0, 10.0)});
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
					return ai.idle(ticks{uniform(0.0, 10.0)});
				}
				return complete{};
			});
		}
	}
	complete basic_ai::attack_state::act(basic_ai& ai) {
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
							return ai.idle(ticks{uniform(0.0, 10.0)});
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
								return ai.idle(ticks{uniform(0.0, 10.0)});
							}
							return complete{};
						});
					} else {
						// Out of range. Move towards target.
						return ai.walk(target_direction, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Walk failed. Wait for up to 10 time units instead.
								return ai.idle(ticks{uniform(0.0, 10.0)});
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

	complete basic_ai::send_message(queries::message::any /*message*/, std::function<complete()> cont) const {
		return cont();
	}

	//complete basic_ai::query_count
	//	( queries::count::any query
	//	, int default_value
	//	, std::optional<int> min
	//	, std::optional<int> max
	//	, std::function<complete(std::optional<int>)> cont
	//	) const
	//{
	//	struct count_query_handler : count_query_const_visitor {
	//		int default_value;
	//		std::optional<int> min;
	//		std::optional<int> max;
	//		std::function<complete(std::optional<int>)> cont;

	//		count_query_handler
	//			( int default_value
	//			, std::optional<int> min
	//			, std::optional<int> max
	//			, std::function<complete(std::optional<int>)> cont
	//			)
	//			: default_value{default_value}, min{min}, max{max}, cont{std::move(cont)}
	//		{}
	//	};

	//	count_query_handler handler{std::move(default_value), min, max, std::move(cont)};
	//	query->accept(handler);
	//	return complete{};
	//}

	complete basic_ai::query_magnitude
		( queries::magnitude::any query
		, double default_value
		, std::optional<double> /*min*/
		, std::optional<double> /*max*/
		, std::function<complete(std::optional<double>)> cont
		) const
	{
		return std::visit([&](auto&& query) {
			SWITCH_TYPE(query) {
				MATCH_TYPE(queries::magnitude::heal) return cont(default_value);
				MATCH_TYPE(queries::magnitude::shock) return cont(default_value);
				MATCH_TYPE(queries::magnitude::wait_time) return cont(default_value);
			}
		}, query);
	}

	complete basic_ai::query_tile
		( queries::tile::any query
		, std::optional<region_tile::point> /*origin*/
		, std::function<bool(region_tile::point)> predicate
		, std::function<complete(std::optional<region_tile::point>)> cont
		) const
	{
		return std::visit([&](auto&& query) {
			SWITCH_TYPE(query) {
				MATCH_TYPE(queries::tile::ranged_attack_target) {
					auto target_id = dynamic_cast<attack_state*>(_state.get())->target_id;
					if (ql::being* target = the_game().beings.ptr(target_id)) {
						if ((target->coords - being.coords).length() <= query.range) {
							// If in range, shoot the target.
							return cont(target->coords);
						}
					}
					return cont(std::nullopt);
				}
				MATCH_TYPE(queries::tile::shock_target) return cont(std::nullopt);
				MATCH_TYPE(queries::tile::teleport_target) return cont(std::nullopt);
			}
		}, query);
	}

	//complete basic_ai::query_direction
	//	( queries::direction::any query
	//	, std::function<complete(std::optional<region_tile::direction>)> cont
	//	) const
	//{
	//	struct direction_query_handler : direction_query_const_visitor {
	//		basic_ai const& ai;
	//		std::function<complete(std::optional<region_tile::direction>)> cont;

	//		direction_query_handler
	//			( basic_ai const& ai
	//			, std::function<complete(std::optional<region_tile::direction>)> cont
	//			)
	//			: ai{ai}
	//			, cont{std::move(cont)}
	//		{}
	//	};

	//	direction_query_handler handler{*this, std::move(cont)};
	//	query->accept(handler);
	//	return complete{};
	//}

	complete basic_ai::query_vector
		( queries::vector::any query
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::vector)> predicate
		, std::function<complete(std::optional<region_tile::vector>)> cont
		) const
	{
		return std::visit([&](auto&& query) {
			SWITCH_TYPE(query) {
				MATCH_TYPE(queries::vector::melee_attack) {
					auto target_id = dynamic_cast<attack_state*>(_state.get())->target_id;
					if (ql::being* target = the_game().beings.ptr(target_id)) {
						// Attack towards the target.
						return cont((target->coords - being.coords).unit());
					}
					return cont(std::nullopt);
				}
			}
		}, query);
	}

	complete basic_ai::query_being
		( queries::being::any //query
		, std::function<bool(ql::being&)> //predicate
		, std::function<complete(std::optional<ql::being*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	//complete basic_ai::query_item
	//	( queries::item::any //query
	//	, ql::being& //source
	//	, std::function<bool(ql::being&)> //predicate
	//	, std::function<complete(std::optional<item*>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}

	////////////////////////////
	// Effect Visitor Methods //
	////////////////////////////

	void basic_ai::visit(injury_effect const& effect) {
		// Retaliate against injuries.
		if (effect.opt_source_id && effect.target_being_id == being.id) {
			_state = umake<attack_state>(*effect.opt_source_id);
		}
	}
}
