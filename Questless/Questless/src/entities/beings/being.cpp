//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/being.hpp"

#include "agents/agent.hpp"
#include "effects/effect.hpp"
#include "effects/injury.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/statuses/status.hpp"
#include "entities/objects/corpse.hpp"
#include "game.hpp"
#include "utility/io.hpp"
#include "utility/random.hpp"
#include "world/region.hpp"
#include "world/section.hpp"

#include <numeric>

using std::function;

namespace ql {
	being::being(const function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::species species)
		: id{id}
		, species{std::move(species)}
		, body{this->species.make_body(id)}
		, base_stats{this->species.make_base_stats()}
		, stats{base_stats}
		, cond{stats}
		, _agent{make_agent(*this)} //
	{
		refresh_stats();

		cond.energy = stats.a.stamina;
		cond.energy.lower_bound_getter = [] { return 0.0_ep; };
		cond.energy.upper_bound_getter = [this] { return stats.a.stamina; };

		cond.direction = static_cast<region_tile::direction>(uniform(1, 6));

		// Only set busy time mutator after initialization so that it won't try to access the being's region before it's
		// placed in one.
		cond.busy_time.set_mutator(busy_time_mutator(), false);
	}

	stats::being being::load_stats(char const* filepath) {
		stats::being result;
		load_from_json(filepath, result);
		return result;
	}

	perception::level being::perception_of(region_tile::point region_tile_coords) const {
		bool in_front = false;
		auto offset = region_tile_coords - location.coords;
		switch (cond.direction) {
			case region_tile::direction::one:
				in_front = offset.q >= 0_span && offset.q + offset.r >= 0_span;
				break;
			case region_tile::direction::two:
				in_front = offset.r >= 0_span && offset.q + offset.r >= 0_span;
				break;
			case region_tile::direction::three:
				in_front = offset.q <= 0_span && offset.r >= 0_span;
				break;
			case region_tile::direction::four:
				in_front = offset.q <= 0_span && offset.q + offset.r <= 0_span;
				break;
			case region_tile::direction::five:
				in_front = offset.r <= 0_span && offset.q + offset.r <= 0_span;
				break;
			case region_tile::direction::six:
				in_front = offset.q >= 0_span && offset.r <= 0_span;
				break;
			default:
				assert(false && "Invalid direction.");
		}
		if (in_front && (region_tile_coords - location.coords).length() <= stats.a.vision.max_range()) {
			auto const illuminance = location.region->illuminance(region_tile_coords);
			span const distance{(region_tile_coords - location.coords).length()};
			double const occlusion = location.region->occlusion(location.coords, region_tile_coords);
			return stats.a.vision.perception(illuminance, distance, occlusion);
		} else {
			return 0.0_perception;
		}
	}

	complete being::act() {
		if (_delayed_actions.empty()) {
			// No delayed actions; have agent choose a new action to perform.
			return agent().act();
		} else {
			// Pop and execute the oldest delayed action, with its continuation.
			auto [action, cont] = std::move(_delayed_actions.front());
			_delayed_actions.pop_front();
			return action->perform(*this, [&, this, cont = std::move(cont)](action::result result) {
				// If there are additional delayed actions, pop and execute the next delay.
				if (!_action_delays.empty()) {
					this->cond.busy_time += _action_delays.front();
					_action_delays.pop_front();
				}
				return cont(result);
			});
		}
	}

	complete being::add_delayed_action(tick delay, action::cont cont, uptr<action> action) {
		// If there are no enqueued delayed actions, just incur the delay immediately instead of enqueueing it.
		if (_delayed_actions.empty()) {
			cond.busy_time += delay;
		} else {
			_action_delays.push_back(delay);
		}
		_delayed_actions.push_back(std::make_tuple(std::move(action), std::move(cont)));
		return complete{};
	}

	void being::clear_delayed_actions() {
		_action_delays.clear();
		_delayed_actions.clear();
	}

	void being::update(tick elapsed) {
		refresh_stats();

		// Update status modifiers.
		for (auto& status : _statuses) {
			status->update(*this, elapsed);
		}
		// Remove expired status modifiers.
		erase_if(_statuses, [](auto const& status) { return status->duration() == 0_tick; });

		// Update conditions.
		cond.satiety -= (cond.awake() ? 0.05_sat : 0.025_sat) / 1_tick * elapsed;
		cond.energy += (cond.awake() ? 1.0_ep : 3.0_ep) / 1_tick * elapsed;
		cond.alertness += (cond.awake() ? -0.1_alert : 0.2_alert) / 1_tick * elapsed;
		cond.busy_time -= elapsed;

		// Update body.
		body.update(elapsed);

		{ // Handle temperature damage.
			constexpr auto temperature_burn_rate = 1.0_burn / 1.0_temp / 1_tick;
			constexpr auto temperature_freeze_rate = 1.0_freeze / 1.0_temp / 1_tick;
			auto const temp = location.region->temperature(location.coords);
			if (temp > stats.max_temp) {
				dmg::group burn = (temp - stats.max_temp) * temperature_burn_rate * elapsed;
				for (body_part& part : body.parts()) {
					take_damage(burn, part, std::nullopt);
				}
			} else if (temp < stats.min_temp) {
				dmg::group freeze = (stats.min_temp - temp) * temperature_freeze_rate * elapsed;
				for (body_part& part : body.parts()) {
					take_damage(freeze, part, std::nullopt);
				}
			}
		}

		{ // Handle blood loss.
			constexpr double stage_1_max = 0.15;
			constexpr double stage_2_max = 0.3;
			constexpr double stage_3_max = 0.4;

			double const pct_blood_lost = 1.0 -
										  (body.blood.value() / body.total_vitality() / body_part::blood_per_vitality).value;

			// No effects for stage 1 blood loss.
			if (pct_blood_lost > stage_1_max) {
				// Stage 2: anxiety

				// Joy loss as a factor of stage-2 blood loss.
				constexpr auto joy_loss_rate = 1.0_joy / 1_tick;
				cond.joy -= (pct_blood_lost - stage_1_max) / (1.0 - stage_1_max) * joy_loss_rate * elapsed;

				// Courage loss as a factor of stage-2 blood loss.
				constexpr auto courage_loss_rate = 1.0_courage / 1_tick;
				cond.courage -= (pct_blood_lost - stage_1_max) / (1.0 - stage_1_max) * courage_loss_rate * elapsed;
			}
			if (pct_blood_lost > stage_2_max) {
				// Stage 3: confusion

				// Intellect loss as a factor of stage-3 blood loss.
				constexpr auto intellect_loss_rate = 50.0_int / 1_tick;
				stats.a.intellect -= (pct_blood_lost - stage_2_max) / (1.0 - stage_2_max) * intellect_loss_rate * elapsed;
			}
			if (pct_blood_lost > stage_3_max) {
				// Stage 4: lethargy, loss of consciousness, damage

				double const pct_stage_4_blood_lost = (pct_blood_lost - stage_3_max) / (1.0 - stage_3_max);

				// Energy loss as a factor of stage-4 blood loss.
				constexpr auto energy_loss_rate = 1.0_ep / 1_tick;
				cond.energy -= pct_stage_4_blood_lost * energy_loss_rate * elapsed;

				// Alertness loss as a factor of stage-4 blood loss.
				constexpr auto alertness_loss_rate = 1.0_alert / 1_tick;
				cond.alertness -= pct_stage_4_blood_lost * alertness_loss_rate * elapsed;

				// Percent of each part's vitality dealt to it as blight damage as a factor of stage-4 blood loss.
				constexpr auto damage_rate = 0.1_blight / 1.0_hp / 1_tick;
				for (body_part& part : body.parts()) {
					dmg::group blight = part.stats.a.vitality.value() * pct_stage_4_blood_lost * damage_rate * elapsed;
					part.take_damage(blight, std::nullopt);
				}
			}
		}

		// Update items.
		for (item& item : inventory.items) {
			item.update(elapsed);
		}
	}

	void being::take_damage(dmg::group& damage, body_part& target_part, std::optional<ql::id<being>> opt_source_id) {
		// Store whether the being was already dead upon taking this damage.
		bool const was_already_dead = cond.mortality == mortality::dead;

		// Get source.
		being* const source = opt_source_id ? the_game().beings.ptr(*opt_source_id) : nullptr;

		// Target will take damage.
		if (before_take_damage(damage, target_part, opt_source_id) == handled::yes) return;

		// Source, if present, will deal damage.
		if (source && source->before_deal_damage(damage, target_part, id) == handled::yes) return;

		// First, apply shields' protection, in reverse order so that more recently equipped shields are the first defense.
		for (auto shields_it = _shields.rbegin(); shields_it != _shields.rend(); ++shields_it) {
			armor& shield = **shields_it;
			shield.apply(damage);
		}

		// Apply part's armor's protection and resistance.
		if (target_part.equipped_item_id) {
			if (armor* armor = the_game().items.ptr_as<ql::armor>(*target_part.equipped_item_id)) {
				armor->apply(damage);
			}
		}

		{ // Apply part's and being's protection, resistance, and vulnerability.
			dmg::protect const total_protection = target_part.stats.protect + stats.protect;
			dmg::resist const total_resistance = stats.resist + target_part.stats.resist;
			dmg::vuln const total_vulnerability = stats.vuln + target_part.stats.vuln;
			damage = damage.with(total_protection, total_resistance, total_vulnerability);
		}

		{ // Apply secondary damage effects.
			//! @todo Add effects for other damage types. Balance numbers.
			struct damage_effect_applier {
				body_part& body_part;
				void operator()(dmg::slash const& slash) {
					constexpr auto bleeding_per_slash = 0.1_blood_per_tick / 1.0_slash;
					body_part.bleeding += slash * bleeding_per_slash;
				}
				void operator()(dmg::pierce const& pierce) {
					constexpr auto bleeding_per_pierce = 0.2_blood_per_tick / 1.0_pierce;
					body_part.bleeding += pierce * bleeding_per_pierce;
				}
				void operator()(dmg::cleave const& cleave) {
					constexpr auto bleeding_per_cleave = 0.1_blood_per_tick / 1.0_cleave;
					body_part.bleeding += cleave * bleeding_per_cleave;
				}
				void operator()(dmg::bludgeon const& bludgeon) {
					constexpr auto bleeding_per_bludgeon = 0.05_blood_per_tick / 1.0_bludgeon;
					body_part.bleeding += bludgeon * bleeding_per_bludgeon;
				}
				void operator()(dmg::burn const& burn) {
					constexpr auto cauterization_per_burn = 1.0_blood_per_tick / 1.0_burn;
					body_part.bleeding -= burn * cauterization_per_burn;
				}
				void operator()(dmg::freeze const&) {}
				void operator()(dmg::blight const&) {}
				void operator()(dmg::poison const&) {}
				void operator()(dmg::shock const&) {}
			};
			for (dmg::damage const& damage_part : damage.parts()) {
				match(damage_part,
					[&](dmg::slash const& slash) {
						constexpr auto bleeding_per_slash = 0.1_blood_per_tick / 1.0_slash;
						target_part.bleeding += slash * bleeding_per_slash;
					},
					[&](dmg::pierce const& pierce) {
						constexpr auto bleeding_per_pierce = 0.2_blood_per_tick / 1.0_pierce;
						target_part.bleeding += pierce * bleeding_per_pierce;
					},
					[&](dmg::cleave const& cleave) {
						constexpr auto bleeding_per_cleave = 0.1_blood_per_tick / 1.0_cleave;
						target_part.bleeding += cleave * bleeding_per_cleave;
					},
					[&](dmg::bludgeon const& bludgeon) {
						constexpr auto bleeding_per_bludgeon = 0.05_blood_per_tick / 1.0_bludgeon;
						target_part.bleeding += bludgeon * bleeding_per_bludgeon;
					},
					[&](dmg::burn const& burn) {
						constexpr auto cauterization_per_burn = 1.0_blood_per_tick / 1.0_burn;
						target_part.bleeding -= burn * cauterization_per_burn;
					},
					[&](dmg::freeze const&) {},
					[&](dmg::blight const&) {},
					[&](dmg::poison const&) {},
					[&](dmg::shock const&) {});
			};
		}

		// Part loses health.
		target_part.health -= damage.health_loss();

		// Check for part disability.
		if (target_part.health.value() <= 0.0_hp) {
			//! @todo Disable target_part.
			if (target_part.vital()) {
				// A vital part has been disabled. Kill target.
				cond.mortality = ql::mortality::dead;
			}
		}

		// Add injury effect.
		location.region->add_effect(effects::injury{location.coords, damage, id, target_part.id, opt_source_id});

		// Target has taken damage.
		if (after_take_damage(damage, target_part, opt_source_id) == handled::yes) return;

		// Source, if present, has dealt damage.
		if (source && source->after_deal_damage(damage, target_part, id) == handled::yes) return;

		// Handle death, if this damage killed the target.
		if (!was_already_dead && cond.mortality == ql::mortality::dead) {
			// Target will die.
			if (before_die(opt_source_id) == handled::yes) return;

			// Source, if present, will have killed target.
			if (source && source->before_kill(id) == handled::yes) return;

			// Target's death succeeded.
			if (cond.corporeal()) {
				// Spawn corpse.
				ql::region& corpse_region = *location.region; // Save region since the being's region pointer will be
															  // nulled when it's removed.
				location.region->remove(*this);
				auto corpse = umake<ql::corpse>(id);
				if (!corpse_region.try_add(*corpse, location.coords)) {
					//! @todo What to do in the unlikely event that the corpse can't be spawned?
				}
				the_game().objects.add(std::move(corpse));
			} else {
				location.region->remove(*this);
			}

			// Source, if present, has killed target.
			if (source && source->after_kill(id) == handled::yes) return;

			// Target has died.
			if (after_die(opt_source_id) == handled::yes) return;
		}
	}

	void being::heal(ql::health amount, body_part& part, std::optional<ql::id<being>> opt_source_id) {
		// Get source.
		being* source = opt_source_id ? the_game().beings.ptr(*opt_source_id) : nullptr;

		// Source will give healing.
		if (source && source->before_give_heal(amount, part, id) == handled::yes) return;

		// Target will receive healing.
		if (before_receive_heal(amount, part, opt_source_id) == handled::yes) return;

		// Target gains health.
		part.health += amount;

		// Target has received healing.
		if (after_receive_heal(amount, part, opt_source_id) == handled::yes) return;

		// Source has given healing.
		if (source && source->after_give_heal(amount, part, id) == handled::yes) return;
	}

	void being::add_status(uptr<status> status) {
		ql::status& ref = *status;
		_statuses.push_back(std::move(status));
		ref.apply(*this);
	}

	stats::being being::get_stats() {
		stats::being result = base_stats;

		// Add body part stats to being stats.
		result.a = std::accumulate(
			body.parts().begin(), body.parts().end(), result.a, [](stats::aggregate acc, body_part const& p) {
				return acc + p.stats.a;
			});

		// Apply status stat modifiers after body part modifiers.
		for (auto const& status : _statuses) {
			stats::apply_all(status->modifiers(), result);
		}

		// Apply condition effects.

		if (cond.weary()) { result.a.strength *= 0.5; }
		if (cond.sleepy()) {
			result.a.agility *= 0.5;
			//! @todo How to apply sleepy penalty to body part dexterities?
			// result.dexterity *= 0.5;
			result.a.intellect *= 0.75;
		}
		if (cond.starving()) {
			result.regen = 0_per_tick;
		} else if (cond.hungry()) {
			result.regen *= 0.5;
		}

		return result;
	}

	std::function<void(tick&, tick const&)> being::busy_time_mutator() {
		return [this](tick& busy_time, tick const& new_busy_time) {
			location.region->remove_from_turn_queue(*this);
			busy_time = new_busy_time;
			location.region->add_to_turn_queue(*this);
		};
	}
}
