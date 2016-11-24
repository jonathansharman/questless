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
#include "Game.h"
#include "agents/Agent.h"
#include "world/Section.h"
#include "world/Region.h"

using std::unique_ptr;
using std::function;

namespace questless
{
	Being::Being(Game& game, function<unique_ptr<Agent>(Being&)> agent_factory, BeingId id, Body body, Attributes base_attributes)
		: Entity(game)
		, _id{id}
		, _agent{agent_factory(*this)}
		, _body{std::move(body)}
		, _need_to_calculate_attributes{false}
		, _base_attributes{base_attributes}
		, _attributes{_base_attributes}
		, _conditions{_base_attributes.vitality, _base_attributes.spirit, 0.0, 0.0, 0.0, 0, false}
	{}

	Being::Being(Game& game, std::istream& in, Body body)
		: Entity(game, in)
		, _body{std::move(body)}
		, _need_to_calculate_attributes{true}
	{
		in >> _id.key;
		in >> _attributes;
		in >> _conditions;
	}

	void Being::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		out << id().key << ' ';
		out << _attributes << ' ';
		out << _conditions << ' ';
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
				gain_busy_time(_action_delays.front());
				_action_delays.pop_front();
			}
		}
	}

	void Being::add_delayed_action(double delay, Action::cont_t cont, Action::ptr action)
	{
		// If there are no enqueued delayed actions, just incur the delay immediately instead of enqueueing it.
		if (_delayed_actions.empty()) {
			gain_busy_time(delay);
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

	double Being::magic_power(spell::Color color) const
	{
		switch (color) {
			case spell::Color::white:  return _attributes.magic_power.white;
			case spell::Color::black:  return _attributes.magic_power.black;
			case spell::Color::green:  return _attributes.magic_power.green;
			case spell::Color::red:    return _attributes.magic_power.red;
			case spell::Color::blue:   return _attributes.magic_power.blue;
			case spell::Color::yellow: return _attributes.magic_power.yellow;
			default: throw std::logic_error{"Unrecognized spell color."};
		}
	}

	double Being::magic_resistance(spell::Color color) const
	{
		switch (color) {
			case spell::Color::white:  return _attributes.magic_resistance.white;
			case spell::Color::black:  return _attributes.magic_resistance.black;
			case spell::Color::green:  return _attributes.magic_resistance.green;
			case spell::Color::red:    return _attributes.magic_resistance.red;
			case spell::Color::blue:   return _attributes.magic_resistance.blue;
			case spell::Color::yellow: return _attributes.magic_resistance.yellow;
			default: throw std::logic_error{"Unrecognized spell color."};
		}
	}

	void Being::magic_power(spell::Color color, double value)
	{
		switch (color) {
			case spell::Color::white:  _attributes.magic_power.white = value;
			case spell::Color::black:  _attributes.magic_power.black = value;
			case spell::Color::green:  _attributes.magic_power.green = value;
			case spell::Color::red:    _attributes.magic_power.red = value;
			case spell::Color::blue:   _attributes.magic_power.blue = value;
			case spell::Color::yellow: _attributes.magic_power.yellow = value;
			default: throw std::logic_error{"Unrecognized spell color."};
		}
	}

	void Being::magic_resistance(spell::Color color, double value)
	{
		switch (color) {
			case spell::Color::white:  _attributes.magic_resistance.white = value;
			case spell::Color::black:  _attributes.magic_resistance.black = value;
			case spell::Color::green:  _attributes.magic_resistance.green = value;
			case spell::Color::red:    _attributes.magic_resistance.red = value;
			case spell::Color::blue:   _attributes.magic_resistance.blue = value;
			case spell::Color::yellow: _attributes.magic_resistance.yellow = value;
			default: throw std::logic_error{"Unrecognized spell color."};
		}
	}

	void Being::update()
	{
		if (_need_to_calculate_attributes) {
			calculate_attributes();
		}

		// Update status modifiers.
		{
			auto i = _statuses.begin();
			while (i != _statuses.end()) {
				(*i)->update(*this);
				if ((*i)->duration() == 0) {
					(*i)->expire(*this);
					_statuses.erase(i);
					_need_to_calculate_attributes = true;
				} else {
					++i;
				}
			}
		}

		// Update conditions.

		_conditions.health += _attributes.health_regen;
		_conditions.mana += _attributes.mana_regen;
		_conditions.satiety += satiety_rate;
		_conditions.energy += energy_rate;
		_conditions.alertness += alertness_rate;
		lose_busy_time(1.0);

		// Update body parts.

		for (BodyPart& part : _body) {
			part.update();
		}

		// Clamp conditions.

		if (_conditions.health > _attributes.vitality) { _conditions.health = _attributes.vitality; }
		if (_conditions.mana > _attributes.spirit) { _conditions.mana = _attributes.spirit; }

		// Handle temperature damage.

		double temp = region().temperature(coords());
		if (temp > max_temp()) {
			auto burn = Damage::from_burn((temp - max_temp()) / (max_temp() - min_temp()) * temperature_damage_factor);
			take_damage(burn, boost::none, boost::none);
		} else if (temp < min_temp()) {
			auto freeze = Damage::from_freeze((min_temp() - temp) / (max_temp() - min_temp()) * temperature_damage_factor);
			take_damage(freeze, boost::none, boost::none);
		}

		// Update items.

		for (Item* item : inventory().items()) {
			item->update();
		}
	}

	void Being::take_damage(Damage& damage, boost::optional<BodyPart::ref> opt_part, boost::optional<BeingId> opt_source_id)
	{
		// Store whether the being was already dead upon taking this damage.
		bool was_already_dead = dead();

		// Get source.
		Being* source = opt_source_id ? game().being(*opt_source_id) : nullptr;

		// Target will take damage.
		if (before_take_damage(damage, opt_part, opt_source_id)) {
			// Source, if present, will deal damage.
			if (!source || source->before_deal_damage(damage, opt_part, id())) {
				// Initialize total resistance and vulnerability to the being's resistance/vulnerability attributes.
				auto total_resistance = resistance();
				auto total_vulnerability = vulnerability();

				// First, apply shields' protection, in reverse order so that more recently equipped shields wear sooner.
				for (auto shields_it = _shields.rbegin(); shields_it != _shields.rend(); ++shields_it) {
					Armor& shield = **shields_it;
					shield.apply_protection(damage);
					total_resistance += shield.resistance();
				}

				// Shields take resistance wear based on damage after shield protection is applied.
				for (const auto& shield : shields()) {
					shield->take_resistance_wear(damage);
				}

				if (opt_part) {
					// Damage is part-targeted.
					BodyPart& part = *opt_part;

					// Apply part's armor's protection, also in reverse order.
					for (auto armor_it = part.armor().rbegin(); armor_it != part.armor().rend(); ++armor_it) {
						Armor& armor = *armor_it;
						armor.apply_protection(damage);
						total_resistance += armor.resistance();
					}

					// Apply part's and being's protection attributes.
					damage -= part.protection().reduction() + protection().reduction();

					// Part's armor takes resistance wear based on the final damage to the part before multipliers.
					for (Armor& armor : part.armor()) {
						armor.take_resistance_wear(damage);
					}

					// Add part's resistance and vulnerability attributes to totals.
					total_resistance += part.resistance();
					total_vulnerability += part.vulnerability();

					// Part and being lose health.
					double health_lost = damage.with(total_resistance, total_vulnerability).total() / (1.0 + endurance_factor * endurance());
					part.lose_health(health_lost);
					lose_health(health_lost);

					// Check for part disability.
					if (part.health() <= 0) {
						/// @todo Disable part.
						if (part.vital()) {
							die();
						}
					}
				} else {
					// Damage is non-part-targeted.

					// Apply being's protection attribute.
					damage -= protection().reduction();

					// Being loses health.
					double health_lost = damage.with(total_resistance, total_vulnerability).total() / (1.0 + endurance_factor * endurance());
					lose_health(health_lost);
				}

				// Target has taken damage.
				if (after_take_damage(damage, opt_part, opt_source_id)) {
					// Source, if present, has dealt damage.
					if (!source || source->after_deal_damage(damage, opt_part, id())) {
						// If target has taken fatal damage, mark it dead.
						if (health() <= 0) {
							die();
						}
						// Handle death, if this damage killed the target.
						if (!was_already_dead && dead()) {
							// Target will die.
							if (before_die(opt_source_id)) {
								// Source, if present, will have killed target.
								if (!source || source->before_kill(id())) {
									// Target's death succeeded.
									// Move it to the graveyard.
									game().add_to_graveyard(std::move(region().remove(*this)));
									/// @todo Make corpse, or do whatever is appropriate for the type of being. (Add abstract base classes for Corporeal, Incorporeal, etc.?)

									// Source, if present, has killed target.
									if (!source || source->after_kill(id())) {
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

	void Being::heal(double amount, boost::optional<BodyPart::ref> opt_part, boost::optional<BeingId> opt_source_id)
	{
		/// @todo Heal the part, if present.

		// Get source.
		Being* source = opt_source_id ? game().being(*opt_source_id) : nullptr;

		// Source will give healing.
		if (source != nullptr) {
			source->before_give_heal(amount, opt_part, id());
		}
		// Target will receive healing.
		before_receive_heal(amount, opt_part, opt_source_id);
		// Target gains health.
		gain_health(amount);
		// Target has received healing.
		after_receive_heal(amount, opt_part, opt_source_id);
		// Source has given healing.
		if (source != nullptr) {
			source->after_give_heal(amount, opt_part, id());
		}
	}

	void Being::add_status(std::unique_ptr<Status> status)
	{
		status->apply(*this);
		_statuses.push_back(std::move(status));
	}

	void Being::gain_health(double amount)
	{
		_conditions.health += amount;
		if (health() > vitality()) {
			health(vitality());
		}
	}
	void Being::lose_health(double amount)
	{
		_conditions.health -= amount;
		if (health() < 0) {
			health(0);
		}
	}

	void Being::gain_mana(double amount)
	{
		_conditions.mana += amount;
		if (mana() > spirit()) {
			mana(spirit());
		}
	}
	void Being::lose_mana(double amount)
	{
		_conditions.mana -= amount;
		if (mana() < 0) {
			mana(0);
		}
	}

	void Being::gain_energy(double amount)
	{
		_conditions.energy += amount;
		if (energy() > stamina()) {
			energy(stamina());
		}
	}
	void Being::lose_energy(double amount)
	{
		_conditions.energy -= amount;
		if (energy() < 0) {
			energy(0);
		}
	}

	void Being::gain_satiety(double amount)
	{
		_conditions.satiety += amount;
		if (satiety() > max_satiety) {
			satiety(max_satiety);
		}
	}
	void Being::lose_satiety(double amount)
	{
		_conditions.satiety -= amount;
		if (satiety() < 0) {
			satiety(0);
		}
	}

	void Being::gain_alertness(double amount)
	{
		_conditions.alertness += amount;
		if (alertness() > max_alertness) {
			alertness(max_alertness);
		}
	}
	void Being::lose_alertness(double amount)
	{
		_conditions.alertness -= amount;
		if (alertness() < 0) {
			alertness(0);
		}
	}

	void Being::busy_time(double value)
	{
		region().remove_from_turn_queue(*this);
		_conditions.busy_time = value;
		region().add_to_turn_queue(*this);
	}
	void Being::gain_busy_time(double amount)
	{
		region().remove_from_turn_queue(*this);
		_conditions.busy_time += amount;
		region().add_to_turn_queue(*this);
	}
	void Being::lose_busy_time(double amount)
	{
		region().remove_from_turn_queue(*this);
		_conditions.busy_time -= amount;
		region().add_to_turn_queue(*this);
	}

	/////////////////////////////////////
	// Attributes and Status Modifiers //
	/////////////////////////////////////

	void Being::calculate_attributes()
	{
		_need_to_calculate_attributes = false;

		_attributes = _base_attributes;

		// Add body part attributes.
		for (const BodyPart& part : _body) {
			part.attributes();
			/// @todo Add attributes.
		}

		// Apply status modifiers.
		for (const auto& status : _statuses) {
			_attributes.modify(status->modifiers());
		}

		// Apply condition effects.

		double percent_weary = 1 - energy() / stamina();
		_attributes.strength -= percent_weary * energy_strength_penalty * _base_attributes.strength;
		_attributes.endurance -= percent_weary * energy_endurance_penalty * _base_attributes.endurance;

		double percent_sleepy = 1 - alertness() / max_alertness;
		_attributes.agility -= percent_sleepy * alertness_agility_penalty * _base_attributes.agility;
		_attributes.dexterity -= percent_sleepy * alertness_dexterity_penalty * _base_attributes.dexterity;
		_attributes.intellect -= percent_sleepy * alertness_intellect_penalty * _base_attributes.intellect;

		double percent_hungry = 1 - satiety() / max_satiety;
		_attributes.health_regen -= percent_hungry * satiety_health_regen_penalty * _base_attributes.health_regen;
		_attributes.mana_regen -= percent_hungry * satiety_mana_regen_penalty * _base_attributes.mana_regen;

		// Clamp negative values to zero.
		_attributes.clamp();
	}
}
