/**
* @file    Being.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Being class.
*/

#include "entities/beings/Being.h"
#include "entities/beings/Agent.h"
#include "entities/beings/BodyPart.h"
#include "world/Section.h"
#include "world/Region.h"

using std::unique_ptr;
using std::function;

namespace questless
{
	Being::Being(function<unique_ptr<Agent>(Being&)> agent_factory, id_t id, BodyPart::ptr body, Attributes base_attributes)
		: Entity{id}
		, _agent{agent_factory(*this)}
		, _body{std::move(body)}
		, _need_to_calculate_attributes{false}
		, _base_attributes{base_attributes}
		, _attributes{_base_attributes}
		, _conditions{_base_attributes.vitality, _base_attributes.spirit, 0.0, 0.0, 0.0, 0}
	{}

	Being::Being(std::istream& in, BodyPart::ptr body)
		: Entity(in)
		, _body{std::move(body)}
		, _need_to_calculate_attributes{true}
	{
		double vitality, spirit, health_regen, mana_regen, strength, endurance, stamina, agility, dexterity, stealth, vision, light_affinity, hearing, intellect, lift, min_temp, max_temp;
		unsigned hands;
		bool mute;
		double white_power, black_power, green_power, red_power, blue_power, yellow_power;
		double white_resistance, black_resistance, green_resistance, red_resistance, blue_resistance, yellow_resistance;

		in >> vitality >> spirit >> health_regen >> mana_regen >> strength >> endurance >> stamina >> agility >> dexterity >> stealth >> vision >> light_affinity >> hearing
			>> intellect >> lift >> min_temp >> max_temp >> hands >> mute >> white_power >> black_power >> green_power >> red_power >> blue_power >> yellow_power
			>> white_resistance >> black_resistance >> green_resistance >> red_resistance >> blue_resistance >> yellow_resistance;

		_attributes = Attributes{vitality, spirit, health_regen, mana_regen, strength, endurance, stamina, agility, dexterity, stealth, vision, light_affinity,
			hearing, intellect, lift, min_temp, max_temp, hands, mute, white_power, black_power, green_power, red_power, blue_power, yellow_power,
			white_resistance, black_resistance, green_resistance, red_resistance, blue_resistance, yellow_resistance};

		double health, mana, energy, satiety, alertness, busy_time;

		in >> health >> mana >> energy >> satiety >> alertness >> busy_time;

		_conditions = Conditions{health, mana, energy, satiety, alertness, busy_time};
	}

	void Being::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		// Write attributes.
		out << vitality() << ' ' << strength() << ' ' << endurance() << ' ' << stamina() << ' ' << agility() << ' ' << dexterity() << ' ' << stealth() << ' '
			<< vision() << ' ' << hearing() << ' ' << intellect() << ' ' << hands() << ' ' << mute() << ' ' << white_power() << ' ' << black_power() << ' '
			<< green_power() << ' ' << red_power() << ' ' << blue_power() << ' ' << yellow_power() << ' ' << white_resistance() << ' ' << black_resistance() << ' '
			<< green_resistance() << ' ' << red_resistance() << ' ' << blue_resistance() << ' ' << yellow_resistance() << ' ';

		// Write conditions.
		out << health() << ' ' << mana() << ' ' << energy() << ' ' << satiety() << ' ' << alertness() << ' ' << busy_time() << ' ';
	}

	double Being::power(Spell::Color color) const
	{
		switch (color) {
			case Spell::Color::white:  return _attributes.white_power;
			case Spell::Color::black:  return _attributes.black_power;
			case Spell::Color::green:  return _attributes.green_power;
			case Spell::Color::red:    return _attributes.red_power;
			case Spell::Color::blue:   return _attributes.blue_power;
			case Spell::Color::yellow: return _attributes.yellow_power;
		}
	}

	double Being::resistance(Spell::Color color) const
	{
		switch (color) {
			case Spell::Color::white:  return _attributes.white_resistance;
			case Spell::Color::black:  return _attributes.black_resistance;
			case Spell::Color::green:  return _attributes.green_resistance;
			case Spell::Color::red:    return _attributes.red_resistance;
			case Spell::Color::blue:   return _attributes.blue_resistance;
			case Spell::Color::yellow: return _attributes.yellow_resistance;
		}
	}

	double Being::power(Spell::Color color, double value)
	{
		switch (color) {
			case Spell::Color::white:  return _attributes.white_power = value;
			case Spell::Color::black:  return _attributes.black_power = value;
			case Spell::Color::green:  return _attributes.green_power = value;
			case Spell::Color::red:    return _attributes.red_power = value;
			case Spell::Color::blue:   return _attributes.blue_power = value;
			case Spell::Color::yellow: return _attributes.yellow_power = value;
		}
	}

	void Being::resistance(Spell::Color color, double value)
	{
		switch (color) {
			case Spell::Color::white:  _attributes.white_resistance = value;
			case Spell::Color::black:  _attributes.black_resistance = value;
			case Spell::Color::green:  _attributes.green_resistance = value;
			case Spell::Color::red:    _attributes.red_resistance = value;
			case Spell::Color::blue:   _attributes.blue_resistance = value;
			case Spell::Color::yellow: _attributes.yellow_resistance = value;
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
		_conditions.satiety += BeingK::satiety_rate;
		_conditions.energy += BeingK::energy_rate;
		_conditions.alertness += BeingK::alertness_rate;
		lose_busy_time(1.0);

		// Clamp conditions.

		if (_conditions.health > _attributes.vitality) { _conditions.health = _attributes.vitality; }
		if (_conditions.mana > _attributes.spirit) { _conditions.mana = _attributes.spirit; }

		// Handle temperature damage.

		double temp = 0.0; /// @todo Get ambient temperature here!
		if (temp > max_temp()) {
			double burn = (temp - max_temp()) / (max_temp() - min_temp()) * BeingK::temperature_damage_factor;
			take_damage(Damage::from_burn(burn));
		} else if (temp < min_temp()) {
			double freeze = (min_temp() - temp) / (max_temp() - min_temp()) * BeingK::temperature_damage_factor;
			take_damage(Damage::from_freeze(freeze));
		}

		// Update items.

		for (Item* item : inventory().items()) {
			item->update();
		}
	}

	void Being::take_damage(Damage& damage, Being* source)
	{
		// Target will take damage.
		before_take_damage(damage, source);
		// Source will deal damage.
		if (source != nullptr) {
			source->before_deal_damage(damage, this);
		}

		// Target loses health.
		Damage damage_reduction;
		for (const auto& armor : _equipped_amor) {
			damage_reduction += armor->apply(damage);
		}
		double applied_damage = (damage - damage_reduction).total() / (1.0 + BeingK::endurance_factor * endurance());
		lose_health(applied_damage);

		// Target has taken damage.
		after_take_damage(damage, source);
		// Source has dealt damage.
		if (source != nullptr) {
			source->after_deal_damage(damage, this);
		}

		// Check for death.
		if (dead()) {
			// Target will die.
			before_die(source);
			// Source will have killed target.
			if (source != nullptr) {
				source->before_kill(this);
			}
			
			// Target's death occurs here.
			/// @todo Make corpse, or do whatever is appropriate for the type of being. (Add abstract base classes for Corporeal, Incorporeal, etc.?)
			_region->remove_from_turn_queue(*this); /// @todo There needs to be a standard way to remove things from a region/section so that calls to remove_from_turn_queue() don't get left out.
			_section->remove<Being>(*this);

			// Source has killed target.
			if (source != nullptr) {
				source->after_kill(this);
			}
			// Target has died.
			after_die(source);
		}
	}

	void Being::heal(double amount, Being* source)
	{
		// Source will give healing.
		if (source != nullptr) {
			source->before_give_heal(amount, this);
		}
		// Target will receive healing.
		before_receive_heal(amount, source);
		// Target gains health.
		gain_health(amount);
		// Target has received healing.
		after_receive_heal(amount, source);
		// Source has given healing.
		if (source != nullptr) {
			source->after_give_heal(amount, this);
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
		if (satiety() > BeingK::max_satiety) {
			satiety(BeingK::max_satiety);
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
		if (alertness() > BeingK::max_alertness) {
			alertness(BeingK::max_alertness);
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
		_region->remove_from_turn_queue(*this);
		_conditions.busy_time = value;
		_region->add_to_turn_queue(*this);
	}
	void Being::gain_busy_time(double amount)
	{
		_region->remove_from_turn_queue(*this);
		_conditions.busy_time += amount;
		_region->add_to_turn_queue(*this);
	}
	void Being::lose_busy_time(double amount)
	{
		_region->remove_from_turn_queue(*this);
		_conditions.busy_time -= amount;
		_region->add_to_turn_queue(*this);
	}

	/////////////////////////////////////
	// Attributes and Status Modifiers //
	/////////////////////////////////////

	void Being::calculate_attributes()
	{
		_need_to_calculate_attributes = false;

		_attributes = _base_attributes;

		// Apply status modifiers.
		for (const auto& status : _statuses) {
			_attributes.modify(status->modifiers());
		}

		// Apply condition effects.

		double percent_weary = 1 - energy() / stamina();
		_attributes.strength -= percent_weary * BeingK::energy_strength_penalty * _base_attributes.strength;
		_attributes.endurance -= percent_weary * BeingK::energy_endurance_penalty * _base_attributes.endurance;

		double percent_sleepy = 1 - alertness() / BeingK::max_alertness;
		_attributes.agility -= percent_sleepy * BeingK::alertness_agility_penalty * _base_attributes.agility;
		_attributes.dexterity -= percent_sleepy * BeingK::alertness_dexterity_penalty * _base_attributes.dexterity;
		_attributes.intellect -= percent_sleepy * BeingK::alertness_intellect_penalty * _base_attributes.intellect;

		double percent_hungry = 1 - satiety() / BeingK::max_satiety;
		_attributes.health_regen -= percent_hungry * BeingK::satiety_health_regen_penalty * _base_attributes.health_regen;
		_attributes.mana_regen -= percent_hungry * BeingK::satiety_mana_regen_penalty * _base_attributes.mana_regen;

		// Clamp negative values to zero.
		_attributes.clamp();
	}
}