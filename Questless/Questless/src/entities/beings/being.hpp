//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/entity.hpp"
#include "entities/beings/body.hpp"
#include "entities/perception.hpp"
#include "items/armor/armor.hpp"
#include "items/inventory.hpp"
#include "items/weapons/weapon.hpp"
#include "stats/being.hpp"
#include "utility/event.hpp"
#include "utility/dynamic_property.hpp"
#include "utility/lazy_bounded.hpp"
#include "utility/static_bounded.hpp"
#include "utility/id.hpp"

#include "utility/quantities.hpp"

#include <vector>
#include <deque>
#include <utility>
#include <functional>
#include <memory>

namespace ql {
	class agent;
	class status;

	enum class mortality : int { alive = 0, dead, undead, immortal };

	//! An animate entity.
	class being : public entity {
	public:
		///////////
		// Types //
		///////////

		using ref_less_t = bool(*)(being const&, being const&);
		using ptr_less_t = bool(*)(uptr<being> const&, uptr<being> const&);

		///////////////
		// Constants //
		///////////////

		//! @todo Probably move some of these to private? (Sleeping stuff goes in sleeping?)

		// Energy
		static constexpr auto energy_rate = 1.0_ep / 1.0_tick; //!< Energy gained per turn (awake or asleep).
		static constexpr auto energy_rate_asleep = 2.0_ep / 1.0_tick; //!< Additional energy gained per turn asleep.
		static constexpr double energy_strength_penalty = 0.5; //!< Proportion of base strength removed at zero energy.
		static constexpr double energy_endurance_penalty = 0.5; //!< Proportion of base endurance removed at zero energy.

		// Satiety
		static constexpr auto min_satiety = 0.0_sat;
		static constexpr auto max_satiety = 100.0_sat;
		static constexpr auto satiety_rate = -0.05_sat / 1.0_tick; //!< Satiety gained per turn (awake or asleep).
		static constexpr auto satiety_rate_asleep = 0.025_sat / 1.0_tick; //!< Additional satiety gained per turn asleep.
		static constexpr auto satiety_regen_penalty = 1.0; //!< Proportion of base health regeneration removed at zero satiety.

		// Alertness
		static constexpr auto min_alertness = 0.0_alert;
		static constexpr auto max_alertness = 100.0_alert;
		static constexpr auto alertness_rate = -0.1_alert / 1.0_tick; //!< Alertness gained per turn (awake or asleep).
		static constexpr auto alertness_rate_asleep = 0.3; //!< Additional alertness gained per turn asleep.
		static constexpr auto alertness_agility_penalty = 0.75; //!< Proportion of base agility removed at zero alertness.
		static constexpr auto alertness_dexterity_penalty = 0.75; //!< Proportion of base dexterity removed at zero alertness.
		static constexpr auto alertness_intellect_penalty = 0.75; //!< Proportion of base intellect removed at zero alertness.
		static constexpr double health_regen_asleep_factor = 1.0; //!< Additional health regeneration multiplier while asleep.

		// Mood
		static constexpr auto min_mood = -100.0_mood;
		static constexpr auto max_mood = 100.0_mood;

		// Temperature
		static constexpr auto temperature_burn_factor = 1.0_burn / 1.0_temp; //!< Burn damage taken = (temp - max) * factor.
		static constexpr auto temperature_freeze_factor = 1.0_freeze / 1.0_temp; //!< Freeze damage taken = (min - temp) * factor.

		/////////////////
		// Public Data //
		/////////////////

		id<being> const id;

		body body;

		inventory inventory;

		// Stats

		stats::being base_stats; //!< Base stats, prior to application of status modifiers.
		stats::being stats; //!< Effective stats, accounting for all modifiers from base.

		// Conditions

		lazy_bounded<ql::energy> energy;
		static_bounded<ql::satiety, min_satiety, max_satiety> satiety;
		static_bounded<ql::alertness, min_alertness, max_alertness> alertness;
		static_bounded<ql::mood, min_mood, max_mood> mood;
		dynamic_property<ql::ticks> busy_time;
		mortality mortality;
		region_tile::direction direction;
		ql::corporeal corporeal;

		// Event Handlers

		event<dmg::group&, body_part&, std::optional<ql::id<being>>> before_take_damage;
		event<dmg::group&, body_part&, std::optional<ql::id<being>>> after_take_damage;

		event<dmg::group&, body_part&, ql::id<being>> before_deal_damage;
		event<dmg::group&, body_part&, ql::id<being>> after_deal_damage;

		event<ql::health&, body_part&, std::optional<ql::id<being>>> before_receive_heal;
		event<ql::health&, body_part&, std::optional<ql::id<being>>> after_receive_heal;

		event<ql::health&, body_part&, ql::id<being>> after_give_heal;
		event<ql::health&, body_part&, ql::id<being>> before_give_heal;

		event<std::optional<ql::id<being>>> before_die;
		event<std::optional<ql::id<being>>> after_die;

		event<ql::id<being>> before_kill;
		event<ql::id<being>> after_kill;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~being();

		//! The agent responsible for this being.
		agent& agent() { return *_agent; }

		//! This being's perception of the tile at @p region_tile_coords in its region.
		perception perception_of(region_tile::point region_tile_coords) const;

		//! Causes the being to perform an action.
		complete act();

		//! Adds the given delayed action to the being's delayed actions queue.
		//! @param delay The delay before the action is performed.
		//! @param cont The continuation function to call once the action completes.
		//! @param action The action to perform after the delay.
		complete add_delayed_action(ticks delay, action::cont cont, uptr<action> action);

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
		void heal(ql::health amount, body_part& part, std::optional<ql::id<being>> opt_source_id);

		void add_status(uptr<status> status);
	protected:
		being
			( const std::function<uptr<ql::agent>(being&)>& make_agent
			, ql::id<being> id
			, ql::body body
			, std::function<stats::being()> const& make_base_stats
			);

		virtual ql::body make_body(ql::id<being> owner_id) = 0;

		static stats::being load_stats(char const* filepath);
	private:
		uptr<ql::agent> _agent; //!< The agent responsible for this being.

		std::deque<ticks> _action_delays; //!< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<std::tuple<uptr<action>, action::cont>> _delayed_actions; //!< The queue of delayed actions and continuations to occur when this being is not busy.

		// Statuses

		std::vector<uptr<status>> _statuses;

		// Items

		std::vector<uptr<weapon>> _equipped_weapons;
		std::vector<uptr<armor>> _shields;

		// Methods

		stats::being get_stats();

		std::function<void(ticks&, ticks const&)> busy_time_mutator();
	};

	DEFINE_ELEMENT_BASE(being, entity)
}
