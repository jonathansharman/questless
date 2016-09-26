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
#include "entities/beings/BeingId.h"
#include "entities/beings/Body.h"
#include "entities/beings/statuses/Status.h"
#include "attributes/Attributes.h"
#include "items/Inventory.h"
#include "items/Item.h"
#include "items/weapons/Weapon.h"
#include "items/armor/Armor.h"
#include "spells/Spell.h"
#include "utility/Event.h"

namespace questless
{
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
			bool dead;

			Conditions() {}

			Conditions(double health, double mana, double energy, double satiety, double alertness, double busy_time, bool dead)
				: health{health}, mana{mana}, energy{energy}, satiety{satiety}, alertness{alertness}, busy_time{busy_time}, dead{dead}
			{}

			friend std::ostream& operator <<(std::ostream& out, const Conditions& c)
			{
				out << c.health << ' ' << c.mana << ' ' << c.energy << ' ' << c.satiety << ' ' << c.alertness << ' ' << c.busy_time << ' ' << c.dead;
				return out;
			}

			friend std::istream& operator >> (std::istream& in, Conditions& c)
			{
				in >> c.health >> c.mana >> c.energy >> c.satiety >> c.alertness >> c.busy_time >> c.dead;
				return in;
			}
		};

		///////////////
		// Constants //
		///////////////

		/// @todo Probably move some of these to private? (Sleeping stuff goes in Sleeping?)

		// Maxima
		static constexpr double max_satiety = 100.0;
		static constexpr double max_alertness = 100.0;

		// Default Stats
		static constexpr double dflt_vitality = 100.0;
		static constexpr double dflt_spirit = 100.0;
		static constexpr double dflt_health_regen = 0.0;
		static constexpr double dflt_mana_regen = 0.0;
		static constexpr double dflt_strength = 100.0;
		static constexpr double dflt_endurance = 100.0;
		static constexpr double dflt_stamina = 100.0;
		static constexpr double dflt_agility = 100.0;
		static constexpr double dflt_dexterity = 100.0;
		static constexpr double dflt_stealth = 100.0;
		static constexpr double dflt_vision = 100.0;
		static constexpr double dflt_light_affinity = 100.0;
		static constexpr double dflt_hearing = 100.0;
		static constexpr double dflt_intellect = 100.0;
		static constexpr double dflt_min_temp = -100.0;
		static constexpr double dflt_max_temp = 100.0;
		static constexpr bool dflt_mute = true;

		static constexpr double dflt_white_power = 100.0;
		static constexpr double dflt_black_power = 100.0;
		static constexpr double dflt_green_power = 100.0;
		static constexpr double dflt_red_power = 100.0;
		static constexpr double dflt_blue_power = 100.0;
		static constexpr double dflt_yellow_power = 100.0;

		static constexpr double dflt_white_resistance = 100.0;
		static constexpr double dflt_black_resistance = 100.0;
		static constexpr double dflt_green_resistance = 100.0;
		static constexpr double dflt_red_resistance = 100.0;
		static constexpr double dflt_blue_resistance = 100.0;
		static constexpr double dflt_yellow_resistance = 100.0;

		// Energy
		static constexpr double energy_rate = 1.0; ///< Energy gained per turn (awake or asleep).
		static constexpr double energy_rate_asleep = 2.0; ///< Additional energy gained per turn asleep.
		static constexpr double energy_strength_penalty = 0.5; ///< Proportion of base strength removed at zero energy.
		static constexpr double energy_endurance_penalty = 0.5; ///< Proportion of base endurance removed at zero energy.

		// Satiety
		static constexpr double satiety_rate = -1.0; ///< Satiety gained per turn (awake or asleep).
		static constexpr double satiety_rate_asleep = 0.5; ///< Additional satiety gained per turn asleep.
		static constexpr double satiety_health_regen_penalty = 1.0; ///< Proportion of base health regeneration removed at zero satiety.
		static constexpr double satiety_mana_regen_penalty = 1.0; ///< Proportion of base mana regeneration removed at zero satiety.

		// Alertness
		static constexpr double alertness_rate = -1.0; ///< Alertness gained per turn (awake or asleep).
		static constexpr double alertness_rate_asleep = 3.0; ///< Additional alertness gained per turn asleep.
		static constexpr double alertness_agility_penalty = 0.75; ///< Proportion of base agility removed at zero alertness.
		static constexpr double alertness_dexterity_penalty = 0.75; ///< Proportion of base dexterity removed at zero alertness.
		static constexpr double alertness_intellect_penalty = 0.75; ///< Proportion of base intellect removed at zero alertness.
		static constexpr double health_regen_asleep_factor = 1.0; ///< Additional health regeneration multiplier while asleep.
		static constexpr double mana_regen_asleep_factor = 1.0; ///< Additional mana regeneration multiplier while asleep.

		// Strength
		static constexpr double strength_factor = 0.01; ///< Proportion of strength by which base damage is multiplied.

		// Endurance
		static constexpr double endurance_factor = 0.01; ///< Proportion of endurance by which damage after armor is divided.

		// Temperature
		static constexpr double temperature_damage_factor = 1.0; ///< Burn damage taken = (temp - max) / (max - min)) * factor. Freeze damage taken = (min - temp) / (max - min)) * factor.

		/////////////////
		// Public Data //
		/////////////////

		// Event Handlers

		Event<Damage&, optional<BeingId>> before_take_damage;
		Event<Damage&, optional<BeingId>> after_take_damage;

		Event<Damage&, BeingId> before_deal_damage;
		Event<Damage&, BeingId> after_deal_damage;

		Event<double&, optional<BeingId>> before_receive_heal;
		Event<double&, optional<BeingId>> after_receive_heal;

		Event<double&, BeingId> after_give_heal;
		Event<double&, BeingId> before_give_heal;

		Event<BeingId> before_kill;
		Event<BeingId> after_kill;

		Event<optional<BeingId>> before_die;
		Event<optional<BeingId>> after_die;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Being() = default;

		/// @param out A stream object into which the serialized being is inserted.
		void serialize(std::ostream& out) const override;

		BeingId id() const { return _id; }

		/// @return The agent responsible for this being.
		Agent& agent() { return *_agent; }

		/// @return The being's inventory.
		Inventory& inventory() { return _inventory; }

		/// @return The being's equipped shields.
		const std::vector<std::unique_ptr<Armor>>& shields() { return _shields; }

		/// Adds the given item to the being's inventory.
		/// @param item An item to be added to the inventory.
		void give_item(Item::ptr item) { _inventory.add(std::move(item)); }

		/// Takes the given item from the being's inventory.
		/// @param item An item to be taken from the inventory.
		/// @return The item taken.
		Item::ptr take_item(const Item& item) { return _inventory.remove(item); }

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
		bool dead() const { return _conditions.dead; }

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
		Vision vision() const { return _attributes.vision; }
		double visual_acuity() const { return _attributes.vision.acuity; }
		double ideal_light() const { return _attributes.vision.ideal_light; }
		double light_tolerance() const { return _attributes.vision.light_tolerance; }
		double hearing() const { return _attributes.hearing; }
		double intellect() const { return _attributes.intellect; }
		double min_temp() const { return _attributes.min_temp; }
		double max_temp() const { return _attributes.max_temp; }
		bool mute() const { return _attributes.mute; }

		MagicPower power() const { return _attributes.magic_power; }
		template <Spell::Color color> double power() const { return _attributes.magic_power.get<color>(); }
		double power(Spell::Color color) const;

		MagicResistance resistance() const { return _attributes.magic_resistance; }
		template <Spell::Color color> double resistance() const { return _attributes.magic_resistance.get<color>(); }
		double resistance(Spell::Color color) const;

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

		void die() { _conditions.dead = true; }
		void resurrect() { _conditions.dead = false; }

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
		void vision(Vision value) { _attributes.vision = value; }
		void visual_acuity(double value) { _attributes.vision.acuity = value; }
		void ideal_light(double value) { _attributes.vision.ideal_light = value; }
		void light_tolerance(double value) { _attributes.vision.light_tolerance = value; }
		void hearing(double value) { _attributes.hearing = value; }
		void intellect(double value) { _attributes.intellect = value; }
		void min_temp(double value) { _attributes.min_temp = value; }
		void max_temp(double value) { _attributes.max_temp = value; }
		void mute(bool value) { _attributes.mute = value; }

		void power(MagicPower value) { _attributes.magic_power = value; }
		template <Spell::Color color> void power(double value) { _attributes.magic_power.get<color> = value; }
		void power(Spell::Color color, double value);

		void resistance(MagicResistance value) { _attributes.magic_resistance = value; }
		template <Spell::Color color> void resistance(double value) { _attributes.magic_resistance.get<color> = value; }
		void resistance(Spell::Color color, double value);

		/// Advances the being one time unit.
		void update() override;

		/// Causes the being to take damage from the specified source being.
		/// @param damage Damage to be applied to this being.
		/// @param source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, optional<BeingId> source_id);

		/// Causes the being to be healed by the specified source being.
		/// @param amount Health to be restored to this being.
		/// @param source The ID of the being which caused the healing, if any.
		void heal(double amount, optional<BeingId> source_id);

		void add_status(std::unique_ptr<Status> status);
	protected:
		Being(Game& game, std::function<std::unique_ptr<Agent>(Being&)> agent_factory, BeingId id, Body body, Attributes base_attributes);
		Being(Game& game, std::istream& in, Body body);

		virtual Body make_body() = 0;
	private:
		BeingId _id;

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
		bool _need_to_calculate_attributes; /// @todo Is there a way to avoid this?

		// Items

		Inventory _inventory;
		std::vector<std::unique_ptr<Weapon>> _equipped_weapons;
		std::vector<std::unique_ptr<Armor>> _shields;

		// Methods

		void calculate_attributes();
	};
}

#endif
