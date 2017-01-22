/**
* @file    Being.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A sentient entity. Includes the player and all NPCs.
*/

#pragma once

#include <vector>
#include <deque>
#include <utility>
#include <functional>
#include <memory>

#include "entities/Entity.h"
#include "entities/beings/BeingId.h"
#include "entities/beings/Body.h"
#include "entities/beings/statuses/Status.h"
#include "stats/Stats.h"
#include "items/Inventory.h"
#include "items/Item.h"
#include "items/weapons/Weapon.h"
#include "items/armor/Armor.h"
#include "spell/Spell.h"
#include "utility/utility.h"
#include "utility/Event.h"
#include "utility/Property.h"
#include "utility/DynamicProperty.h"

namespace questless
{
	class Agent;

	struct Health : TaggedType<double> { using TaggedType::TaggedType; };
	struct Mana : TaggedType<double> { using TaggedType::TaggedType; };
	struct Energy : TaggedType<double> { using TaggedType::TaggedType; };
	struct Satiety : TaggedType<double> { using TaggedType::TaggedType; };
	struct Alertness : TaggedType<double> { using TaggedType::TaggedType; };
	struct BusyTime : TaggedType<double> { using TaggedType::TaggedType; };
	struct Dead : TaggedType<bool> { using TaggedType::TaggedType; };

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

		///////////////
		// Constants //
		///////////////

		/// @todo Probably move some of these to private? (Sleeping stuff goes in Sleeping?)

		// Energy
		static constexpr double energy_rate = 1.0; ///< Energy gained per turn (awake or asleep).
		static constexpr double energy_rate_asleep = 2.0; ///< Additional energy gained per turn asleep.
		static constexpr double energy_strength_penalty = 0.5; ///< Proportion of base strength removed at zero energy.
		static constexpr double energy_endurance_penalty = 0.5; ///< Proportion of base endurance removed at zero energy.

		// Satiety
		static constexpr double max_satiety = 100.0;
		static constexpr double satiety_rate = -1.0; ///< Satiety gained per turn (awake or asleep).
		static constexpr double satiety_rate_asleep = 0.5; ///< Additional satiety gained per turn asleep.
		static constexpr double satiety_health_regen_penalty = 1.0; ///< Proportion of base health regeneration removed at zero satiety.
		static constexpr double satiety_mana_regen_penalty = 1.0; ///< Proportion of base mana regeneration removed at zero satiety.

		// Alertness
		static constexpr double max_alertness = 100.0;
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

		// Intellect
		static constexpr double intellect_factor = 0.01; ///< Proportion of intelligence by which spell incantations are divided.

		// Temperature
		static constexpr double temperature_damage_factor = 1.0; ///< Burn damage taken = (temp - max) / (max - min)) * factor. Freeze damage taken = (min - temp) / (max - min)) * factor.

		/////////////////
		// Public Data //
		/////////////////

		Body body;

		// Stats

		Stats base_stats; ///< Base stats, prior to application of status modifiers.
		Stats stats; ///< Effective stats, accounting for all modifiers from base.

		// Conditions

		DynamicProperty<double> health;
		DynamicProperty<double> mana;
		DynamicProperty<double> energy;

		static void satiety_mutator(double& satiety, const double& new_satiety)
		{
			satiety = new_satiety < 0.0
				? 0.0
				: new_satiety > max_satiety
					? max_satiety
					: new_satiety
				;
		}
		Property<double, satiety_mutator> satiety;

		static void alertness_mutator(double& alertness, const double& new_alertness)
		{
			alertness = new_alertness < 0.0
				? 0.0
				: new_alertness > max_alertness
					? max_alertness
					: new_alertness
				;
		}
		Property<double, alertness_mutator> alertness;

		DynamicProperty<double> busy_time;
		bool dead;
		RegionTileCoords::Direction direction;

		// Event Handlers

		Event<Damage&, BodyPart*, boost::optional<BeingId>> before_take_damage;
		Event<Damage&, BodyPart*, boost::optional<BeingId>> after_take_damage;

		Event<Damage&, BodyPart*, BeingId> before_deal_damage;
		Event<Damage&, BodyPart*, BeingId> after_deal_damage;

		Event<double&, BodyPart*, boost::optional<BeingId>> before_receive_heal;
		Event<double&, BodyPart*, boost::optional<BeingId>> after_receive_heal;

		Event<double&, BodyPart*, BeingId> after_give_heal;
		Event<double&, BodyPart*, BeingId> before_give_heal;

		Event<boost::optional<BeingId>> before_die;
		Event<boost::optional<BeingId>> after_die;

		Event<BeingId> before_kill;
		Event<BeingId> after_kill;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Being();

		/// @param out A stream object into which the serialized being is inserted.
		void serialize(std::ostream& out) const override;

		BeingId id() const { return _id; }

		/// @return The agent responsible for this being.
		Agent& agent() { return *_agent; }

		/// @return Whether the being is corporeal.
		virtual bool corporeal() const = 0;

		/// Causes the being to perform an action.
		void act();

		/// Adds the given delayed action to the being's delayed actions queue.
		/// @param delay The delay before the action is performed.
		/// @param cont The continuation function to call once the action completes.
		/// @param action The action to perform after the delay.
		void add_delayed_action(double delay, Action::cont_t cont, Action::ptr action);

		/// Clears the being's delayed actions queue.
		void clear_delayed_actions();

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

		/// Advances the being one time unit.
		void update() override;

		/// Causes the being to take damage from the specified source being.
		/// @param damage Damage to be applied to this being.
		/// @param part The body part to damage, or nullptr if the damage should be applied directly to the being.
		/// @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(Damage& damage, BodyPart* part, boost::optional<BeingId> opt_source_id);

		/// Causes the being to be healed by the specified source being.
		/// @param amount Health to be restored to this being.
		/// @param part The body part to heal, or nullopt if the damage should be applied directly to the being.
		/// @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(double amount, BodyPart* part, boost::optional<BeingId> opt_source_id);

		void add_status(std::unique_ptr<Status> status);
	protected:
		Being(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id, Body body, const std::function<Stats()>& make_base_stats);
		Being(Game& game, std::istream& in, Body body);

		virtual Body make_body() = 0;
	private:
		BeingId _id;

		std::unique_ptr<Agent> _agent; ///< The agent responsible for this being.

		/// @todo Maybe use a std::pair of actions and conts, and then use the auto [action, cont] = ... syntax in act() once supported?
		std::deque<double> _action_delays; ///< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<Action::ptr> _delayed_actions; ///< The queue of delayed actions to occur when this being is not busy.
		std::deque<Action::cont_t> _delayed_action_conts; ///< The queue of delayed action continuations.

		// Statuses

		std::vector<std::unique_ptr<Status>> _statuses;
		bool _need_to_calculate_stats; /// @todo Is there a way to avoid this?

		// Items

		Inventory _inventory;
		std::vector<std::unique_ptr<Weapon>> _equipped_weapons;
		std::vector<std::unique_ptr<Armor>> _shields;

		// Methods

		Stats base_stats_plus_body_stats();
		Stats effective_stats();

		std::function<void(double&, const double&)> health_mutator();
		std::function<void(double&, const double&)> mana_mutator();
		std::function<void(double&, const double&)> energy_mutator();
		std::function<void(double&, const double&)> busy_time_mutator();
	};
}
