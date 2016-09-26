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

	double Being::power(Spell::Color color) const
	{
		switch (color) {
			case Spell::Color::white:  return _attributes.magic_power.white;
			case Spell::Color::black:  return _attributes.magic_power.black;
			case Spell::Color::green:  return _attributes.magic_power.green;
			case Spell::Color::red:    return _attributes.magic_power.red;
			case Spell::Color::blue:   return _attributes.magic_power.blue;
			case Spell::Color::yellow: return _attributes.magic_power.yellow;
			default: throw logic_error{"Unrecognized spell color."};
		}
	}

	double Being::resistance(Spell::Color color) const
	{
		switch (color) {
			case Spell::Color::white:  return _attributes.magic_resistance.white;
			case Spell::Color::black:  return _attributes.magic_resistance.black;
			case Spell::Color::green:  return _attributes.magic_resistance.green;
			case Spell::Color::red:    return _attributes.magic_resistance.red;
			case Spell::Color::blue:   return _attributes.magic_resistance.blue;
			case Spell::Color::yellow: return _attributes.magic_resistance.yellow;
			default: throw logic_error{"Unrecognized spell color."};
		}
	}

	void Being::power(Spell::Color color, double value)
	{
		switch (color) {
			case Spell::Color::white:  _attributes.magic_power.white = value;
			case Spell::Color::black:  _attributes.magic_power.black = value;
			case Spell::Color::green:  _attributes.magic_power.green = value;
			case Spell::Color::red:    _attributes.magic_power.red = value;
			case Spell::Color::blue:   _attributes.magic_power.blue = value;
			case Spell::Color::yellow: _attributes.magic_power.yellow = value;
			default: throw logic_error{"Unrecognized spell color."};
		}
	}

	void Being::resistance(Spell::Color color, double value)
	{
		switch (color) {
			case Spell::Color::white:  _attributes.magic_resistance.white = value;
			case Spell::Color::black:  _attributes.magic_resistance.black = value;
			case Spell::Color::green:  _attributes.magic_resistance.green = value;
			case Spell::Color::red:    _attributes.magic_resistance.red = value;
			case Spell::Color::blue:   _attributes.magic_resistance.blue = value;
			case Spell::Color::yellow: _attributes.magic_resistance.yellow = value;
			default: throw logic_error{"Unrecognized spell color."};
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
			take_damage(burn, nullopt);
		} else if (temp < min_temp()) {
			auto freeze = Damage::from_freeze((min_temp() - temp) / (max_temp() - min_temp()) * temperature_damage_factor);
			take_damage(freeze, nullopt);
		}

		// Update items.

		for (Item* item : inventory().items()) {
			item->update();
		}
	}

	void Being::take_damage(Damage& damage, optional<BeingId> source_id)
	{
		// Get source.
		Being* source = nullptr;
		if (source_id) {
			source = game().being(*source_id);
		}

		// Target will take damage.
		before_take_damage(damage, source_id);
		// Source will deal damage.
		if (source != nullptr) {
			source->before_deal_damage(damage, id());
		}

		// Target loses health.
		Damage damage_reduction = Damage::zero();
		for (auto armor_it = _shields.rbegin(); armor_it != _shields.rend(); ++armor_it) {
			// Apply shields in reverse order so more recently equipped shields wear sooner.
			damage_reduction += (*armor_it)->apply(damage);
		}
		double applied_damage = (damage - damage_reduction).total() / (1.0 + endurance_factor * endurance());
		lose_health(applied_damage);

		// Target has taken damage.
		after_take_damage(damage, source_id);
		// Source has dealt damage.
		if (source != nullptr) {
			source->after_deal_damage(damage, id());
		}

		// Check for death.
		if (health() <= 0 && !dead()) {
			// Target will die.
			before_die(source_id);
			// Source will have killed target.
			if (source != nullptr) {
				source->before_kill(id());
			}
			
			// Target's death occurs here.
			/// @todo Make corpse, or do whatever is appropriate for the type of being. (Add abstract base classes for Corporeal, Incorporeal, etc.?)
			die();

			// Source has killed target.
			if (source != nullptr) {
				source->after_kill(id());
			}
			// Target has died.
			after_die(source_id);

			// Move to graveyard.
			game().add_to_graveyard(std::move(region().remove(*this)));
		}
	}

	void Being::heal(double amount, optional<BeingId> source_id)
	{
		// Get source.
		Being* source = nullptr;
		if (source_id) {
			source = game().being(*source_id);
		}

		// Source will give healing.
		if (source != nullptr) {
			source->before_give_heal(amount, id());
		}
		// Target will receive healing.
		before_receive_heal(amount, source_id);
		// Target gains health.
		gain_health(amount);
		// Target has received healing.
		after_receive_heal(amount, source_id);
		// Source has given healing.
		if (source != nullptr) {
			source->after_give_heal(amount, id());
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
