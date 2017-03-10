/**
* @file    Being.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Being class.
*/

#include "entities/beings/Being.h"
#include "entities/beings/Body.h"
#include "entities/objects/Corpse.h"
#include "Game.h"
#include "agents/Agent.h"
#include "world/Section.h"
#include "world/Region.h"

using std::unique_ptr;
using std::function;

namespace questless
{
	Being::Being(Game& game, const function<unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const function<Stats()>& make_base_stats)
		: Entity(game)
		, id{id}
		, body{std::move(body)}
		, base_stats{make_base_stats()}
		, stats{base_stats_plus_body_stats()}
		, health{stats.vitality, health_mutator()}
		, mana{stats.spirit, mana_mutator()}
		, energy{stats.stamina, energy_mutator()}
		, satiety{max_satiety}
		, alertness{max_alertness}
		, busy_time{0.0}
		, dead{false}
		, direction{static_cast<RegionTileCoords::Direction>(uniform(1, 6))}
		, _agent{make_agent(*this)}
		, _need_to_calculate_stats{false}
		
	{
		stats = effective_stats();

		// Only set busy time mutator after initialization so that it won't try to access the being's region before it's placed in one.
		busy_time.set_mutator(busy_time_mutator(), false);
	}

	Being::Being(Game& game, std::istream& in, Body body)
		: Entity(game, in)
		, id{in}
		, body{std::move(body)}
		, health{health_mutator()}
		, mana{mana_mutator()}
		, energy{energy_mutator()}
		, busy_time{busy_time_mutator()}
		, _need_to_calculate_stats{true}
	{
		/// @todo Read body.

		in >> base_stats;
		in >> stats;

		in >> health >> mana >> energy >> satiety >> alertness >> busy_time >> dead;

		/// @todo Is there a better way to extract into an enum class?
		int direction_int;
		in >> direction_int;
		direction = static_cast<RegionTileCoords::Direction>(direction_int);
	}

	Being::~Being() = default;

	void Being::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		out << id << ' ';

		/// @todo Write body.

		out << health << ' ' << mana << ' ' << energy << ' ' << satiety << ' ' << alertness << ' '
			<< busy_time << ' ' << dead << ' ' << static_cast<int>(direction);

		out << base_stats << ' ';
		out << stats << ' ';
	}

	void Being::act()
	{
		if (_delayed_actions.empty()) {
			// No delayed actions; have agent choose a new action to perform.
			agent().act();
		} else {
			// Pop and execute the oldest delayed action, with its continuation.
			Action::ptr action = std::move(_delayed_actions.front());
			_delayed_actions.pop_front();
			Action::cont_t cont = std::move(_delayed_action_conts.front());
			_delayed_action_conts.pop_front();
			action->perform(*this, std::move(cont));
			// If there are additional delayed actions, pop and execute the next delay.
			if (!_action_delays.empty()) {
				busy_time += _action_delays.front();
				_action_delays.pop_front();
			}
		}
	}

	void Being::add_delayed_action(double delay, Action::cont_t cont, Action::ptr action)
	{
		// If there are no enqueued delayed actions, just incur the delay immediately instead of enqueueing it.
		if (_delayed_actions.empty()) {
			busy_time += delay;
		} else {
			_action_delays.push_back(delay);
		}
		_delayed_actions.push_back(std::move(action));
		_delayed_action_conts.push_back(std::move(cont));
	}

	void Being::clear_delayed_actions()
	{
		_action_delays.clear();
		_delayed_actions.clear();
		_delayed_action_conts.clear();
	}

	void Being::update()
	{
		if (_need_to_calculate_stats) {
			stats = effective_stats();
			_need_to_calculate_stats = false;
		}

		// Update status modifiers.
		{
			auto i = _statuses.begin();
			while (i != _statuses.end()) {
				(*i)->update(*this);
				if ((*i)->duration() == 0) {
					(*i)->expire(*this);
					_statuses.erase(i);
					_need_to_calculate_stats = true;
				} else {
					++i;
				}
			}
		}

		// Update conditions.
		health += stats.health_regen;
		mana += stats.mana_regen;
		satiety += satiety_rate;
		energy += energy_rate;
		alertness += alertness_rate;
		busy_time -= 1.0;

		// Update body parts.
		for (BodyPart& part : body) {
			part.update();
		}

		// Update abilities.
		abilities.update(*this);

		// Handle temperature damage.
		double temp = region->temperature(coords);
		if (temp > stats.max_temp) {
			Damage burn{Burn{(temp - stats.max_temp) / (stats.max_temp - stats.min_temp) * temperature_damage_factor}};
			take_damage(burn, nullptr, boost::none);
		} else if (temp < stats.min_temp) {
			Damage freeze{Freeze{(stats.min_temp - temp) / (stats.max_temp - stats.min_temp) * temperature_damage_factor}};
			take_damage(freeze, nullptr, boost::none);
		}

		// Update items.
		for (Item* item : inventory().items()) {
			item->update();
		}
	}

	void Being::take_damage(Damage& damage, BodyPart* part, boost::optional<Id<Being>> opt_source_id)
	{
		// Store whether the being was already dead upon taking this damage.
		bool was_already_dead = dead;

		// Get source.
		Being* source = opt_source_id ? game.being(*opt_source_id) : nullptr;

		// Target will take damage.
		if (before_take_damage(damage, part, opt_source_id)) {
			// Source, if present, will deal damage.
			if (!source || source->before_deal_damage(damage, part, id)) {
				// Initialize total resistance and vulnerability to the being's resistance/vulnerability stats.
				auto total_resistance = stats.resistance;
				auto total_vulnerability = stats.vulnerability;

				// First, apply shields' protection, in reverse order so that more recently equipped shields wear sooner.
				for (auto shields_it = _shields.rbegin(); shields_it != _shields.rend(); ++shields_it) {
					Armor& shield = **shields_it;
					shield.apply_protection(damage);
					total_resistance += shield.resistance();
				}

				// Shields take resistance wear based on damage after shield protection is applied.
				for (auto const& shield : shields()) {
					shield->take_resistance_wear(damage);
				}

				if (part) {
					// Damage is part-targeted.

					// Apply part's armor's protection, also in reverse order.
					for (auto armor_it = part->armor().rbegin(); armor_it != part->armor().rend(); ++armor_it) {
						Armor& armor = *armor_it;
						armor.apply_protection(damage);
						total_resistance += armor.resistance();
					}

					// Apply part's and being's protection stats.
					damage -= part->protection().reduction() + stats.protection.reduction();

					// Part's armor takes resistance wear based on the final damage to the part before multipliers.
					for (Armor& armor : part->armor()) {
						armor.take_resistance_wear(damage);
					}

					// Add part's resistance and vulnerability stats to totals.
					total_resistance += part->resistance();
					total_vulnerability += part->vulnerability();

					// Part and being lose health.
					double health_lost = damage.with(total_resistance, total_vulnerability).total() / (1.0 + endurance_factor * stats.endurance);
					part->health -= health_lost;
					health -= health_lost;

					// Check for part disability.
					if (part->health <= 0) {
						/// @todo Disable part.
						if (part->vital()) {
							dead = true;
						}
					}
				} else {
					// Damage is non-part-targeted.

					// Apply being's protection stat.
					damage -= stats.protection.reduction();

					// Being loses health.
					double health_lost = damage.with(total_resistance, total_vulnerability).total() / (1.0 + endurance_factor * stats.endurance);
					health -= health_lost;
				}

				// Add injury effect.
				game.add_effect(std::make_shared<InjuryEffect>(coords, damage, id, opt_source_id));

				// Target has taken damage.
				if (after_take_damage(damage, part, opt_source_id)) {
					// Source, if present, has dealt damage.
					if (!source || source->after_deal_damage(damage, part, id)) {
						// If target has taken fatal damage, mark it dead.
						if (health <= 0) {
							dead = true;
						}
						// Handle death, if this damage killed the target.
						if (!was_already_dead && dead) {
							// Target will die.
							if (before_die(opt_source_id)) {
								// Source, if present, will have killed target.
								if (!source || source->before_kill(id)) {
									// Target's death succeeded.

									if (corporeal()) {
										// Spawn corpse.
										Region& corpse_region = *region; // Save region since the being's region pointer will be nulled when it's removed.
										auto corpse = std::make_unique<Corpse>(game, Id<Object>::make(), corpse_region.remove(*this));
										corpse_region.add<Object>(std::move(corpse), coords);
									} else {
										/// @todo Eliminate graveyard. It is a crutch to deal with references to annihilated beings when dealing fatal damage to a body part.
										// Move target to the graveyard.
										game.add_to_graveyard(region->remove(*this));
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

	void Being::heal(double amount, BodyPart* part, boost::optional<Id<Being>> opt_source_id)
	{
		/// @todo Heal the part, if present.

		// Get source.
		Being* source = opt_source_id ? game.being(*opt_source_id) : nullptr;

		// Source will give healing.
		if (source != nullptr) {
			source->before_give_heal(amount, part, id);
		}
		// Target will receive healing.
		before_receive_heal(amount, part, opt_source_id);
		// Target gains health.
		health += amount;
		// Target has received healing.
		after_receive_heal(amount, part, opt_source_id);
		// Source has given healing.
		if (source != nullptr) {
			source->after_give_heal(amount, part, id);
		}
	}

	void Being::add_status(std::unique_ptr<Status> status)
	{
		status->apply(*this);
		_statuses.push_back(std::move(status));
	}

	/////////////////////////////////////
	//    Stats and Status Modifiers   //
	/////////////////////////////////////

	Stats Being::base_stats_plus_body_stats()
	{
		Stats result = base_stats;

		// Apply body part stat modifiers, and sum weight.
		for (BodyPart const& part : body) {
			Modifier::apply_all(part.modifiers(), result);

			result.weight += part.weight();
		}

		return result;
	}

	Stats Being::effective_stats()
	{
		Stats result = base_stats_plus_body_stats();

		// Apply status stat modifiers (may override body part modifiers).
		for (auto const& status : _statuses) {
			Modifier::apply_all(status->modifiers(), result);
		}

		// Apply condition effects.

		double percent_weary = 1.0 - energy / result.stamina;
		result.strength -= percent_weary * energy_strength_penalty * base_stats.strength;
		result.endurance -= percent_weary * energy_endurance_penalty * base_stats.endurance;

		double percent_sleepy = 1.0 - alertness / max_alertness;
		result.agility -= percent_sleepy * alertness_agility_penalty * base_stats.agility;
		result.dexterity -= percent_sleepy * alertness_dexterity_penalty * base_stats.dexterity;
		result.intellect -= percent_sleepy * alertness_intellect_penalty * base_stats.intellect;

		double percent_hungry = 1.0 - satiety / max_satiety;
		result.health_regen -= percent_hungry * satiety_health_regen_penalty * base_stats.health_regen;
		result.mana_regen -= percent_hungry * satiety_mana_regen_penalty * base_stats.mana_regen;

		return result;
	}

	std::function<void(double&, double const&)> Being::health_mutator()
	{
		return [this](double& health, double const& new_health)
		{
			health = std::clamp(new_health, 0.0, stats.vitality.get());
		};
	}
	std::function<void(double&, double const&)> Being::mana_mutator()
	{
		return [this](double& mana, double const& new_mana)
		{
			mana = std::clamp(new_mana, 0.0, stats.spirit.get());
		};
	}
	std::function<void(double&, double const&)> Being::energy_mutator()
	{
		return [this](double& energy, double const& new_energy)
		{
			energy = std::clamp(new_energy, 0.0, stats.stamina.get());
		};
	}
	std::function<void(double&, double const&)> Being::busy_time_mutator()
	{
		return [this](double& busy_time, double const& new_busy_time) {
			region->remove_from_turn_queue(*this);
			busy_time = new_busy_time;
			region->add_to_turn_queue(*this);
		};
	}
}
