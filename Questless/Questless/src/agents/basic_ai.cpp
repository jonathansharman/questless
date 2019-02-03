//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "basic_ai.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"
#include "utility/random.hpp"
#include "utility/visitation.hpp"

namespace ql {
	complete basic_ai::act() {
		return _state->act(*this);
	}

	complete basic_ai::idle_state::act(basic_ai& ai) {
		// Walk next time.
		ai._state = umake<walk_state>();
		// Wait for up to 10 time units.
		return ai.idle(tick{uniform(0, 10)});
	}

	complete basic_ai::walk_state::act(basic_ai& ai) {
		// Move or turn at random.
		if (random_bool()) {
			return ai.walk(ai.being.direction, [&ai](action::result result) {
				// Idle next time.
				ai._state = umake<idle_state>();
				if (result == action::result::aborted) {
					// Walk failed. Wait for up to 10 ticks instead.
					return ai.idle(tick{uniform(0, 10)});
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
					return ai.idle(tick{uniform(0, 10)});
				}
				return complete{};
			});
		}
	}

	complete basic_ai::attack_state::act(basic_ai& ai) {
		if (ql::being* target = the_game().beings.ptr(target_id)) {
			if (perception::get_category(ai.being.perception_of(target->coords)) == perception::category::none) {
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
							return ai.idle(tick{uniform(0, 10)});
						}
						return complete{};
					});
				} else {
					// Facing towards target.
					if ((target->coords - ai.being.coords).length() == 1_span) {
						// Within striking distance of target.
						//! @todo This is a hack that assumes the first item in the inventory is a melee weapon.
						item& item = *ai.being.inventory.items.begin();
						return item.actions().front()->perform(ai.being, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Attack failed. Wait for up to 10 time units instead.
								return ai.idle(tick{uniform(0, 10)});
							}
							return complete{};
						});
					} else {
						// Out of range. Move towards target.
						return ai.walk(target_direction, [&ai](action::result result) {
							if (result == action::result::aborted) {
								// Walk failed. Wait for up to 10 time units instead.
								return ai.idle(tick{uniform(0, 10)});
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

	void basic_ai::perceive(effects::effect const& effect) {
		if (std::holds_alternative<effects::injury>(effect)) {
			// Retaliate against injuries. Ignore all other effects.
			effects::injury const& injury = std::get<effects::injury>(effect);
			if (injury.opt_source_id && injury.target_being_id == being.id) {
				_state = umake<attack_state>(*injury.opt_source_id);
			}
		}
	}

	complete basic_ai::send_message(queries::message::query /*message*/, std::function<complete()> cont) {
		return cont();
	}

	complete basic_ai::query_count(queries::count::query query,
		int default_value,
		std::optional<int> /*min*/,
		std::optional<int> /*max*/,
		std::function<complete(std::optional<int>)> cont) //
	{
		return std::visit([&](queries::count::wait_time const&) { return cont(default_value); }, query);
	}

	complete basic_ai::query_magnitude(queries::magnitude::query query,
		double default_value,
		std::optional<double> /*min*/,
		std::optional<double> /*max*/,
		std::function<complete(std::optional<double>)> cont) //
	{
		return match(query, //
			[&](queries::magnitude::heal const&) { return cont(default_value); },
			[&](queries::magnitude::shock const&) { return cont(default_value); });
	}

	complete basic_ai::query_tile(queries::tile::query query,
		std::optional<region_tile::point> /*origin*/,
		std::function<bool(region_tile::point)> predicate,
		std::function<complete(std::optional<region_tile::point>)> cont) {
		return match(query, //
			[&](queries::tile::ranged_attack_target const& query) {
				auto target_id = dynamic_cast<attack_state*>(_state.get())->target_id;
				if (ql::being* target = the_game().beings.ptr(target_id)) {
					if ((target->coords - being.coords).length() <= query.range) {
						// If in range, shoot the target.
						return cont(target->coords);
					}
				}
				return cont(std::nullopt);
			},
			[&](queries::tile::shock_target const&) { return cont(std::nullopt); },
			[&](queries::tile::teleport_target const&) { return cont(std::nullopt); });
	}

	complete basic_ai::query_direction(queries::direction::query // query
		,
		std::function<complete(std::optional<region_tile::direction>)> cont) {
		return cont(std::nullopt);
	}

	complete basic_ai::query_vector(queries::vector::query query,
		std::optional<region_tile::point>,
		std::function<bool(region_tile::vector)> predicate,
		std::function<complete(std::optional<region_tile::vector>)> cont) {
		return std::visit(
			[&](queries::vector::melee_attack const&) {
				auto target_id = dynamic_cast<attack_state*>(_state.get())->target_id;
				if (ql::being* target = the_game().beings.ptr(target_id)) {
					// Attack towards the target.
					return cont((target->coords - being.coords).unit());
				}
				return cont(std::nullopt);
			},
			query);
	}

	complete basic_ai::query_being(queries::being::query /*query*/,
		std::function<bool(ql::being&)> /*predicate*/,
		std::function<complete(std::optional<ql::being*>)> cont) {
		return cont(std::nullopt);
	}

	complete basic_ai::query_item(queries::item::query /*query*/,
		ql::being& /*source*/,
		std::function<bool(ql::being&)> /*predicate*/,
		std::function<complete(std::optional<item*>)> cont) {
		return cont(std::nullopt);
	}
}
