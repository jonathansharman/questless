//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/being.hpp"
#include "entities/beings/body.hpp"
#include "entities/objects/corpse.hpp"
#include "game.hpp"
#include "agents/agent.hpp"
#include "utility/random.hpp"
#include "world/section.hpp"
#include "world/region.hpp"

using std::function;

namespace ql
{
	being::being(const function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::body body, const function<ql::stats()>& make_base_stats)
		: entity{}
		, id{id}
		, body{std::move(body)}
		, base_stats{make_base_stats()}
		, stats{get_base_stats_plus_body_stats()}
		, mana{stats.spirit, [] { return 0.0; }, [this] { return stats.spirit.value(); }}
		, energy{stats.stamina, [] { return 0.0; }, [this] { return stats.stamina.value(); }}
		, satiety{max_satiety}
		, alertness{max_alertness}
		, mood{0.0}
		, busy_time{0.0}
		, dead{false}
		, direction{static_cast<region_tile::direction>(uniform(1, 6))}
		, _agent{make_agent(*this)}
		
	{
		refresh_stats();

		// Only set busy time mutator after initialization so that it won't try to access the being's region before it's placed in one.
		busy_time.set_mutator(busy_time_mutator(), false);
	}

	being::being(std::istream& in, ql::body body) //! @todo Load agent.
		: entity{in}
		, id{in}
		, body{std::move(body)}
		, mana{[] { return 0.0; }, [this] { return stats.spirit.value(); }}
		, energy{[] { return 0.0; }, [this] { return stats.stamina.value(); }}
		, busy_time{busy_time_mutator()}
	{
		//! @todo Read body.

		in >> base_stats;
		in >> stats;

		in >> mana >> energy >> satiety >> alertness >> mood >> busy_time >> dead;

		//! @todo Is there a better way to extract into an enum class?
		int direction_int;
		in >> direction_int;
		direction = static_cast<region_tile::direction>(direction_int);
	}

	being::~being() = default;

	void being::serialize(std::ostream& out) const
	{
		entity::serialize(out);

		out << id << ' ';

		//! @todo Write body.

		out << mana << ' ' << energy << ' ' << satiety << ' ' << alertness << ' ' << mood << ' '
			<< busy_time << ' ' << dead << ' ' << static_cast<int>(direction);

		out << base_stats << ' ';
		out << stats << ' ';
	}

	perception being::perception_of(region_tile::point region_tile_coords) const
	{
		bool in_front;
		auto offset = region_tile_coords - coords;
		switch (direction) {
			case region_tile::direction::one:
				in_front = offset.q >= 0 && offset.q + offset.r >= 0;
				break;
			case region_tile::direction::two:
				in_front = offset.r >= 0 && offset.q + offset.r >= 0;
				break;
			case region_tile::direction::three:
				in_front = offset.q <= 0 && offset.r >= 0;
				break;
			case region_tile::direction::four:
				in_front = offset.q <= 0 && offset.q + offset.r <= 0;
				break;
			case region_tile::direction::five:
				in_front = offset.r <= 0 && offset.q + offset.r <= 0;
				break;
			case region_tile::direction::six:
				in_front = offset.q >= 0 && offset.r <= 0;
				break;
			default:
				throw std::logic_error{"Invalid direction."};
		}
		if (in_front && (region_tile_coords - coords).length() <= stats.vision.max_range()) {
			double const illuminance = region->illuminance(region_tile_coords);
			int const distance = (region_tile_coords - coords).length();
			double const occlusion = region->occlusion(coords, region_tile_coords);
			return stats.vision.visibility(illuminance, distance) * occlusion;
		} else {
			return 0.0;
		}
	}

	void being::act()
	{
		if (_delayed_actions.empty()) {
			// No delayed actions; have agent choose a new action to perform.
			agent().act();
		} else {
			// Pop and execute the oldest delayed action, with its continuation.
			auto [action, cont] = std::move(_delayed_actions.front());
			_delayed_actions.pop_front();
			action->perform(*this, std::move(cont));
			// If there are additional delayed actions, pop and execute the next delay.
			if (!_action_delays.empty()) {
				busy_time += _action_delays.front();
				_action_delays.pop_front();
			}
		}
	}

	complete being::add_delayed_action(double delay, action::cont cont, uptr<action> action)
	{
		// If there are no enqueued delayed actions, just incur the delay immediately instead of enqueueing it.
		if (_delayed_actions.empty()) {
			busy_time += delay;
		} else {
			_action_delays.push_back(delay);
		}
		_delayed_actions.push_back(std::make_tuple(std::move(action), std::move(cont)));
		return complete{};
	}

	void being::clear_delayed_actions()
	{
		_action_delays.clear();
		_delayed_actions.clear();
	}

	void being::update()
	{
		refresh_stats();
		
		// Update status modifiers.
		for (auto& status : _statuses) {
			status->update(*this);
		}
		// Remove expired status modifiers.
		erase_if(_statuses, [](auto const& status) { return status->duration() == 0; });

		// Update conditions.
		mana += stats.mana_regen;
		satiety += satiety_rate;
		energy += energy_rate;
		alertness += alertness_rate;
		busy_time -= 1.0;

		// Update body.
		body.update();

		{ // Handle temperature damage.
			double const temp = region->temperature(coords);
			if (temp > stats.max_temp) {
				dmg::group burn = dmg::burn{(temp - stats.max_temp) / (stats.max_temp - stats.min_temp) * temperature_damage_factor};
				for (body_part& part : body) {
					take_damage(burn, part, std::nullopt);
				}
			} else if (temp < stats.min_temp) {
				dmg::group freeze = dmg::freeze{(stats.min_temp - temp) / (stats.max_temp - stats.min_temp) * temperature_damage_factor};
				for (body_part& part : body) {
					take_damage(freeze, part, std::nullopt);
				}
			}
		}

		{ // Handle blood loss.
			constexpr double stage_1_max = 0.15;
			constexpr double stage_2_max = 0.3;
			constexpr double stage_3_max = 0.4;

			double const pct_blood_lost = 1.0 - body.blood / body.total_vitality();

			// No effects for stage 1 blood loss.
			if (pct_blood_lost > stage_1_max) {
				// Stage 2: anxiety

				// Mood loss per unit time as a factor of stage-2 blood loss.
				constexpr double mood_loss_factor = 1.0;
				mood -= (pct_blood_lost - stage_1_max) / (1.0 - stage_1_max) * mood_loss_factor;
			}
			if (pct_blood_lost > stage_2_max) {
				// Stage 3: confusion
				
				// Intellect loss as a factor of stage-3 blood loss.
				constexpr double intellect_loss_factor = 50.0;
				stats.intellect -= (pct_blood_lost - stage_2_max) / (1.0 - stage_2_max) * intellect_loss_factor;
			}
			if (pct_blood_lost > stage_3_max) {
				// Stage 4: lethargy, loss of consciousness, damage

				double const pct_stage_4_blood_lost = (pct_blood_lost - stage_3_max) / (1.0 - stage_3_max);

				// Energy loss per unit time as a factor of stage-4 blood loss.
				constexpr double energy_loss_factor = 1.0;
				energy -= pct_stage_4_blood_lost * energy_loss_factor;

				// Alertness loss per unit time as a factor of stage-4 blood loss.
				constexpr double alertness_loss_factor = 1.0;
				alertness -= pct_stage_4_blood_lost * alertness_loss_factor;

				// Percent of each part's vitality dealt to it as blight damage as a factor of stage-4 blood loss.
				constexpr double damage_factor = 0.1;
				for (body_part& part : body) {
					dmg::group blight = dmg::blight{part.vitality * pct_stage_4_blood_lost * damage_factor};
					part.take_damage(blight, std::nullopt);
				}
			}
		}

		// Update abilities.
		abilities.update(*this);

		// Update items.
		for (item& item : inventory.items) {
			item.update();
		}
	}

	void being::take_damage(dmg::group& damage, body_part& target_part, std::optional<ql::id<being>> opt_source_id)
	{
		// Store whether the being was already dead upon taking this damage.
		bool const was_already_dead = dead;

		// Get source.
		being* const source = opt_source_id ? the_game().beings.ptr(*opt_source_id) : nullptr;

		// Target will take damage.
		if (before_take_damage(damage, target_part, opt_source_id)) {
			// Source, if present, will deal damage.
			if (!source || source->before_deal_damage(damage, target_part, id)) {
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
					dmg::protect const total_protection = target_part.protection() + stats.protection;
					dmg::resist const total_resistance = stats.resistance + target_part.resistance();
					dmg::vuln const total_vulnerability = stats.vulnerability + target_part.vulnerability();
					damage = damage.with(total_protection, total_resistance, total_vulnerability);
				}

				{ // Apply secondary damage effects.
					//! @todo Add effects for other damage types. Balance numbers.
					struct damage_effect_applier
					{
						body_part& body_part;
						void operator ()(dmg::slash const& slash)
						{
							constexpr double bleeding_per_slash = 0.1;
							body_part.bleeding += slash * bleeding_per_slash;
						}
						void operator ()(dmg::pierce const& pierce)
						{
							constexpr double bleeding_per_pierce = 0.1;
							body_part.bleeding += pierce * bleeding_per_pierce;
						}
						void operator ()(dmg::cleave const& cleave)
						{
							constexpr double bleeding_per_cleave = 0.1;
							body_part.bleeding += cleave * bleeding_per_cleave;
						}
						void operator ()(dmg::bludgeon const& bludgeon)
						{
							constexpr double bleeding_per_bludgeon = 0.05;
							body_part.bleeding += bludgeon * bleeding_per_bludgeon;
						}
						void operator ()(dmg::burn const& burn)
						{
							constexpr double cauterization_per_burn = 1.0;
							body_part.bleeding -= burn * cauterization_per_burn;
						}
						void operator ()(dmg::freeze const& /*freeze*/) {}
						void operator ()(dmg::blight const& /*blight*/) {}
						void operator ()(dmg::poison const& /*poison*/) {}
						void operator ()(dmg::shock const& /*shock*/) {}
					};
					for (dmg::damage const& damage_part : damage.parts()) {
						std::visit(damage_effect_applier{target_part}, damage_part);
					}
				}

				// Part loses health.
				target_part.health -= damage.total() / (1.0 + endurance_factor * stats.endurance);

				// Check for part disability.
				if (target_part.health <= 0) {
					//! @todo Disable target_part.
					if (target_part.vital()) {
						// A vital part has been disabled. Kill target.
						dead = true;
					}
				}

				// Add injury effect.
				the_game().add_effect(smake<injury_effect>(coords, damage, id, target_part.id, opt_source_id));

				// Target has taken damage.
				if (after_take_damage(damage, target_part, opt_source_id)) {
					// Source, if present, has dealt damage.
					if (!source || source->after_deal_damage(damage, target_part, id)) {
						// Handle death, if this damage killed the target.
						if (!was_already_dead && dead) {
							// Target will die.
							if (before_die(opt_source_id)) {
								// Source, if present, will have killed target.
								if (!source || source->before_kill(id)) {
									// Target's death succeeded.

									if (corporeal()) {
										// Spawn corpse.
										ql::region& corpse_region = *region; // Save region since the being's region pointer will be nulled when it's removed.
										region->remove(*this);
										auto corpse = umake<ql::corpse>(id);
										corpse_region.add(*corpse, coords);
										the_game().objects.add(std::move(corpse));
									} else {
										region->remove(*this);
									}

									// Source, if present, has killed target.
									if (!source || source->after_kill(id)) {
										// Target has died.
										if (after_die(opt_source_id)) {
											return;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void being::heal(double amount, body_part& part, std::optional<ql::id<being>> opt_source_id)
	{
		//! @todo heal the part, if present.

		// Get source.
		being* source = opt_source_id ? the_game().beings.ptr(*opt_source_id) : nullptr;

		// Source will give healing.
		if (!source || source->before_give_heal(amount, part, id)) {
			// Target will receive healing.
			if (before_receive_heal(amount, part, opt_source_id)) {
				// Target gains health.
				part.health += amount;
				// Target has received healing.
				if (after_receive_heal(amount, part, opt_source_id)) {
					// Source has given healing.
					if (!source || source->after_give_heal(amount, part, id)) {
						return;
					}
				}
			}
		}
	}

	void being::add_status(uptr<status> status)
	{
		ql::status& ref = *status;
		_statuses.push_back(std::move(status));
		ref.apply(*this);
	}

	////////////////////////////////
	// Stats and Status Modifiers //
	////////////////////////////////

	stats being::get_base_stats_plus_body_stats()
	{
		ql::stats result = base_stats;

		// Apply body part stat modifiers, and sum weight.
		for (body_part const& part : body) {
			modifier::apply_all(part.modifiers(), result);

			result.weight += part.weight();
		}

		return result;
	}

	stats being::get_stats()
	{
		ql::stats result = get_base_stats_plus_body_stats();

		// Apply status stat modifiers after body part modifiers.
		for (auto const& status : _statuses) {
			modifier::apply_all(status->modifiers(), result);
		}

		// Apply condition effects.

		double pct_weary = 1.0 - energy / result.stamina;
		result.strength -= pct_weary * energy_strength_penalty * base_stats.strength;
		result.endurance -= pct_weary * energy_endurance_penalty * base_stats.endurance;

		double pct_sleepy = 1.0 - alertness / max_alertness;
		result.agility -= pct_sleepy * alertness_agility_penalty * base_stats.agility;
		//! @todo How to apply sleepy penalty to body part dexterities?
		//result.dexterity -= pct_sleepy * alertness_dexterity_penalty * base_stats.dexterity;
		result.intellect -= pct_sleepy * alertness_intellect_penalty * base_stats.intellect;

		double pct_hungry = 1.0 - satiety / max_satiety;
		result.health_regen -= pct_hungry * satiety_health_regen_penalty * base_stats.health_regen;
		result.mana_regen -= pct_hungry * satiety_mana_regen_penalty * base_stats.mana_regen;

		return result;
	}

	std::function<void(double&, double const&)> being::busy_time_mutator()
	{
		return [this](double& busy_time, double const& new_busy_time) {
			this->region->remove_from_turn_queue(*this);
			busy_time = new_busy_time;
			this->region->add_to_turn_queue(*this);
		};
	}
}
