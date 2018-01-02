//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <deque>
#include <utility>
#include <functional>
#include <memory>

#include "entities/entity.hpp"
#include "entities/beings/abilities.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/statuses/status.hpp"
#include "entities/perception.hpp"
#include "items/armor/armor.hpp"
#include "items/inventory.hpp"
#include "items/item.hpp"
#include "items/weapons/weapon.hpp"
#include "magic/spell.hpp"
#include "stats/stats.hpp"
#include "utility/event.hpp"
#include "utility/dynamic_property.hpp"
#include "utility/lazy_bounded.hpp"
#include "utility/static_bounded.hpp"
#include "utility/id.hpp"
#include "utility/utility.hpp"

namespace ql
{
	class agent;

	struct health : tagged_type<double> { using tagged_type::tagged_type; };
	struct mana : tagged_type<double> { using tagged_type::tagged_type; };
	struct energy : tagged_type<double> { using tagged_type::tagged_type; };
	struct satiety : tagged_type<double> { using tagged_type::tagged_type; };
	struct alertness : tagged_type<double> { using tagged_type::tagged_type; };
	struct busy_time : tagged_type<double> { using tagged_type::tagged_type; };
	struct dead : tagged_type<bool> { using tagged_type::tagged_type; };

	//! An animate entity.
	class being : public entity
	{
	public:
		///////////
		// Types //
		///////////

		using ref_less_t = std::function<bool(being const&, being const&)>;
		using ptr_less_t = std::function<bool(uptr<being> const&, uptr<being> const&)>;

		///////////////
		// Constants //
		///////////////

		//! @todo Probably move some of these to private? (Sleeping stuff goes in sleeping?)

		// Energy
		static constexpr double energy_rate = 1.0; //!< Energy gained per turn (awake or asleep).
		static constexpr double energy_rate_asleep = 2.0; //!< Additional energy gained per turn asleep.
		static constexpr double energy_strength_penalty = 0.5; //!< Proportion of base strength removed at zero energy.
		static constexpr double energy_endurance_penalty = 0.5; //!< Proportion of base endurance removed at zero energy.

		// Satiety
		static constexpr double min_satiety = 0.0;
		static constexpr double max_satiety = 100.0;
		static constexpr double satiety_rate = -0.05; //!< Satiety gained per turn (awake or asleep).
		static constexpr double satiety_rate_asleep = 0.025; //!< Additional satiety gained per turn asleep.
		static constexpr double satiety_health_regen_penalty = 1.0; //!< Proportion of base health regeneration removed at zero satiety.
		static constexpr double satiety_mana_regen_penalty = 1.0; //!< Proportion of base mana regeneration removed at zero satiety.

		// Alertness
		static constexpr double min_alertness = 0.0;
		static constexpr double max_alertness = 100.0;
		static constexpr double alertness_rate = -0.1; //!< Alertness gained per turn (awake or asleep).
		static constexpr double alertness_rate_asleep = 0.3; //!< Additional alertness gained per turn asleep.
		static constexpr double alertness_agility_penalty = 0.75; //!< Proportion of base agility removed at zero alertness.
		static constexpr double alertness_dexterity_penalty = 0.75; //!< Proportion of base dexterity removed at zero alertness.
		static constexpr double alertness_intellect_penalty = 0.75; //!< Proportion of base intellect removed at zero alertness.
		static constexpr double health_regen_asleep_factor = 1.0; //!< Additional health regeneration multiplier while asleep.
		static constexpr double mana_regen_asleep_factor = 1.0; //!< Additional mana regeneration multiplier while asleep.

		// Mood
		static constexpr double min_mood = -100.0;
		static constexpr double max_mood = 100.0;

		// Strength
		static constexpr double strength_factor = 0.01; //!< Proportion of strength by which base damage is multiplied.

		// Endurance
		static constexpr double endurance_factor = 0.01; //!< Proportion of endurance by which damage after armor is divided.

		// Intellect
		static constexpr double intellect_factor = 0.01; //!< Proportion of intelligence by which spell incantations are divided.

		// Temperature
		static constexpr double temperature_damage_factor = 1.0; //!< Burn damage taken = (temp - max) / (max - min)) * factor. Freeze damage taken = (min - temp) / (max - min)) * factor.

		/////////////////
		// Public Data //
		/////////////////

		id<being> const id;

		body body;

		inventory inventory;

		// Stats

		stats base_stats; //!< Base stats, prior to application of status modifiers.
		stats stats; //!< Effective stats, accounting for all modifiers from base.

		// Conditions

		lazy_bounded<double> mana;
		lazy_bounded<double> energy;
		static_bounded<double, min_satiety, max_satiety> satiety;
		static_bounded<double, min_alertness, max_alertness> alertness;
		static_bounded<double, min_mood, max_mood> mood;
		dynamic_property<double> busy_time;
		bool dead;
		region_tile::direction direction;

		// Abilities

		abilities abilities;

		// Event Handlers

		event<dmg::group&, body_part&, std::optional<ql::id<being>>> before_take_damage;
		event<dmg::group&, body_part&, std::optional<ql::id<being>>> after_take_damage;

		event<dmg::group&, body_part&, ql::id<being>> before_deal_damage;
		event<dmg::group&, body_part&, ql::id<being>> after_deal_damage;

		event<double&, body_part&, std::optional<ql::id<being>>> before_receive_heal;
		event<double&, body_part&, std::optional<ql::id<being>>> after_receive_heal;

		event<double&, body_part&, ql::id<being>> after_give_heal;
		event<double&, body_part&, ql::id<being>> before_give_heal;

		event<std::optional<ql::id<being>>> before_die;
		event<std::optional<ql::id<being>>> after_die;

		event<ql::id<being>> before_kill;
		event<ql::id<being>> after_kill;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~being();

		//! @param out A stream object into which the serialized being is inserted.
		void serialize(std::ostream& out) const override;

		//! The agent responsible for this being.
		agent& agent() { return *_agent; }

		//! Whether the being is corporeal.
		virtual bool corporeal() const = 0;

		//! This being's perception of the tile at @p region_tile_coords in its region.
		perception perception_of(region_tile::point region_tile_coords) const;

		//! Causes the being to perform an action.
		void act();

		//! Adds the given delayed action to the being's delayed actions queue.
		//! @param delay The delay before the action is performed.
		//! @param cont The continuation function to call once the action completes.
		//! @param action The action to perform after the delay.
		complete add_delayed_action(double delay, action::cont cont, uptr<action> action);

		//! Clears the being's delayed actions queue.
		void clear_delayed_actions();

		//! The being's equipped shields.
		std::vector<uptr<armor>> const& shields() { return _shields; }

		//! Recalculates this being's stats. Called automatically with each update.
		void refresh_stats() { stats = get_stats(); }

		//! Advances the being one time unit.
		void update() final;

		//! Causes the being to take damage to the given part from the specified source being.
		//! @param damage Damage to be applied to this being.
		//! @param target_part The body part to damage.
		//! @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, body_part& target_part, std::optional<ql::id<being>> opt_source_id);

		//! Causes the being to be healed by the specified source being.
		//! @param amount Health to be restored to this being.
		//! @param part The body part to heal.
		//! @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(double amount, body_part& part, std::optional<ql::id<being>> opt_source_id);

		void add_status(uptr<status> status);
	protected:
		being(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::body body, const std::function<ql::stats()>& make_base_stats);
		being(std::istream& in, ql::body body);

		virtual ql::body make_body(ql::id<being> owner_id) = 0;
	private:
		uptr<ql::agent> _agent; //!< The agent responsible for this being.

		std::deque<double> _action_delays; //!< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<std::tuple<uptr<action>, action::cont>> _delayed_actions; //!< The queue of delayed actions and continuations to occur when this being is not busy.

		// Statuses

		std::vector<uptr<status>> _statuses;

		// Items

		std::vector<uptr<weapon>> _equipped_weapons;
		std::vector<uptr<armor>> _shields;

		// Methods

		ql::stats get_base_stats_plus_body_stats();
		ql::stats get_stats();

		std::function<void(double&, double const&)> busy_time_mutator();
	};

	DEFINE_ELEMENT_BASE(being, entity)
}
