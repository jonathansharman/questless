/**
* @file    Being.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Being class, which includes the player and all NPCs.
*/

#ifndef BEING_H
#define BEING_H

#include <vector>
#include <utility>
#include <functional>
#include <memory>

#include "entities/Entity.h"
#include "entities/beings/Body.h"
#include "entities/beings/statuses/Status.h"
#include "Attributes.h"
#include "items/Inventory.h"
#include "items/Item.h"
#include "items/weapons/Weapon.h"
#include "items/armor/Armor.h"
#include "spells/Spell.h"
#include "utility/Event.h"

namespace questless
{
	namespace BeingK
	{
		// Maxima
		const double max_satiety = 100.0;
		const double max_alertness = 100.0;

		// Default Stats
		const double dflt_vitality = 100.0;
		const double dflt_spirit = 100.0;
		const double dflt_health_regen = 0.0;
		const double dflt_mana_regen = 0.0;
		const double dflt_strength = 100.0;
		const double dflt_endurance = 100.0;
		const double dflt_stamina = 100.0;
		const double dflt_agility = 100.0;
		const double dflt_dexterity = 100.0;
		const double dflt_stealth = 100.0;
		const double dflt_vision = 100.0;
		const double dflt_light_affinity = 100.0;
		const double dflt_hearing = 100.0;
		const double dflt_intellect = 100.0;
		const double dflt_min_temp = -100.0;
		const double dflt_max_temp = 100.0;
		const unsigned dflt_hands = 0;
		const bool dflt_mute = true;

		const double dflt_white_power = 100.0;
		const double dflt_black_power = 100.0;
		const double dflt_green_power = 100.0;
		const double dflt_red_power = 100.0;
		const double dflt_blue_power = 100.0;
		const double dflt_yellow_power = 100.0;

		const double dflt_white_resistance = 100.0;
		const double dflt_black_resistance = 100.0;
		const double dflt_green_resistance = 100.0;
		const double dflt_red_resistance = 100.0;
		const double dflt_blue_resistance = 100.0;
		const double dflt_yellow_resistance = 100.0;

		// Energy
		const double energy_rate = 1.0; ///< Energy gained per turn (awake or asleep).
		const double energy_rate_asleep = 2.0; ///< Additional energy gained per turn asleep.
		const double energy_strength_penalty = 0.5; ///< Proportion of base strength removed at zero energy.
		const double energy_endurance_penalty = 0.5; ///< Proportion of base endurance removed at zero energy.

		// Satiety
		const double satiety_rate = -1.0; ///< Satiety gained per turn (awake or asleep).
		const double satiety_rate_asleep = 0.5; ///< Additional satiety gained per turn asleep.
		const double satiety_health_regen_penalty = 1.0; ///< Proportion of base health regeneration removed at zero satiety.
		const double satiety_mana_regen_penalty = 1.0; ///< Proportion of base mana regeneration removed at zero satiety.

		// Alertness
		const double alertness_rate = -1.0; ///< Alertness gained per turn (awake or asleep).
		const double alertness_rate_asleep = 3.0; ///< Additional alertness gained per turn asleep.
		const double alertness_agility_penalty = 0.75; ///< Proportion of base agility removed at zero alertness.
		const double alertness_dexterity_penalty = 0.75; ///< Proportion of base dexterity removed at zero alertness.
		const double alertness_intellect_penalty = 0.75; ///< Proportion of base intellect removed at zero alertness.
		const double health_regen_asleep_factor = 1.0; ///< Additional health regeneration multiplier while asleep.
		const double mana_regen_asleep_factor = 1.0; ///< Additional mana regeneration multiplier while asleep.

		// Strength
		const double strength_factor = 0.01; ///< Proportion of strength by which base damage is multiplied.

		// Endurance
		const double endurance_factor = 0.01; ///< Proportion of endurance by which damage after armor is divided.

		// Temperature
		const double temperature_damage_factor = 1.0; ///< Burn damage taken = (temp - max) / (max - min)) * factor. Freeze damage taken = (min - temp) / (max - min)) * factor.

		// Vision
		// Visibility at a tile = (vision - distance^2 * vision distance factor) / (1 + abs(light level at tile - light affinity) * vision affinity factor).
		const double vision_distance_factor = 1;
		const double vision_affinity_factor = 0.1;
	}

	// Forward class declarations.

	class Agent;

	class Being : public Entity
	{
	public:
		///////////
		// Types //
		///////////

		using ptr = std::unique_ptr<Being>;
		using ref = std::reference_wrapper<Being>;
		using ptr_less_t = std::function<bool(const Being::ptr&, const Being::ptr&)>;
		using ref_less_t = std::function<bool(const Being&, const Being&)>;

		struct Conditions
		{
			double health;
			double mana;
			double energy;
			double satiety;
			double alertness;
			double busy_time;

			Conditions() {}

			Conditions(double health, double mana, double energy, double satiety, double alertness, double busy_time)
				: health{health}, mana{mana}, energy{energy}, satiety{satiety}, alertness{alertness}, busy_time{busy_time}
			{}
		};

		/////////////////
		// Public Data //
		/////////////////

		// Delegates

		Event<Damage&, Being*> before_take_damage;
		Event<Damage&, Being*> after_take_damage;

		Event<Damage&, Being*> before_deal_damage;
		Event<Damage&, Being*> after_deal_damage;

		Event<double&, Being*> before_receive_heal;
		Event<double&, Being*> after_receive_heal;

		Event<double&, Being*> after_give_heal;
		Event<double&, Being*> before_give_heal;

		Event<Being*> before_kill;
		Event<Being*> after_kill;

		Event<Being*> before_die;
		Event<Being*> after_die;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Being() = default;

		Being& as_being() { return dynamic_cast<Being&>(*this); }

		/// @param out A stream object into which the serialized being is inserted.
		void serialize(std::ostream& out) const override;

		/// @return The agent responsible for this being.
		Agent& agent() { return *_agent; }

		/// @return The being's inventory.
		Inventory& inventory() { return _inventory; }

		/// Adds the given item to the being's inventory.
		/// @param item An item to be added to the inventory.
		void give_item(Item::ptr item) { _inventory.add(std::move(item)); }

		/// @return The being's body.
		const Body& body() const { return _body; }
		/// @return The being's body.
		Body& body() { return _body; }

		// Condition accessors

		double health() const { return _conditions.health; }
		double mana() const { return _conditions.mana; }
		double energy() const { return _conditions.energy; }
		double satiety() const { return _conditions.satiety; }
		double alertness() const { return _conditions.alertness; }
		double busy_time() const { return _conditions.busy_time; }

		bool dead() const { return _conditions.health <= 0.0; }

		// Attribute accessors

		double vitality() const { return _attributes.vitality; }
		double spirit() const { return _attributes.spirit; }
		double health_regen() const { return _attributes.health_regen; }
		double mana_regen() const { return _attributes.mana_regen; }
		double strength() const { return _attributes.strength; }
		double endurance() const { return _attributes.endurance; }
		double stamina() const { return _attributes.stamina; }
		double agility() const { return _attributes.agility; }
		double dexterity() const { return _attributes.dexterity; }
		double stealth() const { return _attributes.stealth; }
		double vision() const { return _attributes.vision; }
		double light_affinity() const { return _attributes.light_affinity; }
		double hearing() const { return _attributes.hearing; }
		double intellect() const { return _attributes.intellect; }
		double min_temp() const { return _attributes.min_temp; }
		double max_temp() const { return _attributes.max_temp; }
		bool mute() const { return _attributes.mute; }

		double power(Spell::Color color) const;

		double white_power() const { return _attributes.white_power; }
		double black_power() const { return _attributes.black_power; }
		double green_power() const { return _attributes.green_power; }
		double red_power() const { return _attributes.red_power; }
		double blue_power() const { return _attributes.blue_power; }
		double yellow_power() const { return _attributes.yellow_power; }

		double resistance(Spell::Color color) const;

		double white_resistance() const { return _attributes.white_resistance; }
		double black_resistance() const { return _attributes.black_resistance; }
		double green_resistance() const { return _attributes.green_resistance; }
		double red_resistance() const { return _attributes.red_resistance; }
		double blue_resistance() const { return _attributes.blue_resistance; }
		double yellow_resistance() const { return _attributes.yellow_resistance; }

		// Condition mutators

		void health(double value) { _conditions.health = value; }
		void gain_health(double amount);
		void lose_health(double amount);

		void mana(double value) { _conditions.mana = value; }
		void gain_mana(double amount);
		void lose_mana(double amount);

		void energy(double value) { _conditions.energy = value; }
		void gain_energy(double amount);
		void lose_energy(double amount);

		void satiety(double value) { _conditions.satiety = value; }
		void gain_satiety(double amount);
		void lose_satiety(double amount);

		void alertness(double value) { _conditions.alertness = value; }
		void gain_alertness(double amount);
		void lose_alertness(double amount);

		void busy_time(double value);
		void gain_busy_time(double amount);
		void lose_busy_time(double amount);

		// Attribute mutators

		void vitality(double value) { _attributes.vitality = value; }
		void spirit(double value) { _attributes.spirit = value; }
		void health_regen(double value) { _attributes.health_regen = value; }
		void mana_regen(double value) { _attributes.mana_regen = value; }
		void strength(double value) { _attributes.strength = value; }
		void endurance(double value) { _attributes.endurance = value; }
		void stamina(double value) { _attributes.stamina = value; }
		void agility(double value) { _attributes.agility = value; }
		void dexterity(double value) { _attributes.dexterity = value; }
		void stealth(double value) { _attributes.stealth = value; }
		void vision(double value) { _attributes.vision = value; }
		void light_affinity(double value) { _attributes.light_affinity = value; }
		void hearing(double value) { _attributes.hearing = value; }
		void intellect(double value) { _attributes.intellect = value; }
		void min_temp(double value) { _attributes.min_temp = value; }
		void max_temp(double value) { _attributes.max_temp = value; }
		void mute(bool value) { _attributes.mute = value; }

		double power(Spell::Color color, double value);

		void white_power(double value) { _attributes.white_power = value; }
		void black_power(double value) { _attributes.black_power = value; }
		void green_power(double value) { _attributes.green_power = value; }
		void red_power(double value) { _attributes.red_power = value; }
		void blue_power(double value) { _attributes.blue_power = value; }
		void yellow_power(double value) { _attributes.yellow_power = value; }

		void resistance(Spell::Color color, double value);

		void white_resistance(double value) { _attributes.white_resistance = value; }
		void black_resistance(double value) { _attributes.black_resistance = value; }
		void green_resistance(double value) { _attributes.green_resistance = value; }
		void red_resistance(double value) { _attributes.red_resistance = value; }
		void blue_resistance(double value) { _attributes.blue_resistance = value; }
		void yellow_resistance(double value) { _attributes.yellow_resistance = value; }

		/// Advances the being one time unit.
		void update() override;

		/// Causes the being to take damage from the specified source being.
		/// @param damage Damage to be applied to this being.
		/// @param source The being which caused the damage, if any.
		void take_damage(Damage& damage, Being* source = nullptr);

		/// Causes the being to be healed by the specified source being.
		/// @param amount Health to be restored to this being.
		/// @param source The being which caused the healing, if any.
		void heal(double amount, Being* source = nullptr);

		void add_status(std::unique_ptr<Status> status);
	protected:
		Being(std::function<std::unique_ptr<Agent>(Being&)> agent_factory, id_t id, Body body, Attributes base_attributes);
		Being(std::istream& in, Body body);

		virtual Body make_body() = 0;
	private:
		std::shared_ptr<Agent> _agent; ///< The agent responsible for this being.

		// Body

		Body _body;

		// Attributes

		Attributes _base_attributes; ///< Base attributes, prior to application of status modifiers.
		Attributes _attributes; ///< Effective attributes, accounting for status modifiers.

		// Conditions

		Conditions _conditions;

		// Statuses

		std::vector<std::unique_ptr<Status>> _statuses;
		bool _need_to_calculate_attributes;

		// Items

		Inventory _inventory;
		std::vector<std::unique_ptr<Weapon>> _equipped_weapons;
		std::vector<std::unique_ptr<Armor>> _equipped_amor;

		// Methods

		void calculate_attributes();
	};
}

#endif