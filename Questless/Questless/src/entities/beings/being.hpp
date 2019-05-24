//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "../perception.hpp"
#include "body.hpp"
#include "conditions.hpp"
#include "stats/being.hpp"

#include "items/armor/armor.hpp"
#include "items/weapons/weapon.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "utility/dynamic_property.hpp"
#include "utility/event.hpp"
#include "utility/lazy_bounded.hpp"
#include "utility/static_bounded.hpp"

namespace ql {
	struct agent;
	struct region;
	struct section;
	struct status;

	//! An animate entity.
	struct being {
		ent id;

		body body;

		location location;

		//! Base stats, prior to application of status modifiers.
		stats::being base_stats;
		//! Effective stats, accounting for all modifiers from base.
		stats::being stats;

		//! This being's current conditions.
		conditions cond;

		// Event Handlers

		event<dmg::group&, body_part&, std::optional<ent>> before_take_damage;
		event<dmg::group&, body_part&, std::optional<ent>> after_take_damage;

		event<dmg::group&, body_part&, ent> before_deal_damage;
		event<dmg::group&, body_part&, ent> after_deal_damage;

		event<ql::health&, body_part&, std::optional<ent>> before_receive_heal;
		event<ql::health&, body_part&, std::optional<ent>> after_receive_heal;

		event<ql::health&, body_part&, ent> after_give_heal;
		event<ql::health&, body_part&, ent> before_give_heal;

		event<std::optional<ent>> before_die;
		event<std::optional<ent>> after_die;

		event<ent> before_kill;
		event<ent> after_kill;

		being(ent id);

		//! This being's perception of the tile at @p region_tile_coords in its region.
		perception::level perception_of(region_tile::point region_tile_coords) const;

		//! Causes the being to perform an action.
		action::result act();

		//! Adds the given delayed action to the being's delayed actions queue.
		//! @param delay The delay before the action is performed.
		//! @param action The action to perform after the delay.
		void add_delayed_action(tick delay, uptr<action> action);

		//! Clears the being's delayed actions queue.
		void clear_delayed_actions();

		//! Recalculates this being's stats. Called automatically with each update.
		void refresh_stats() {
			stats = get_stats();
		}

		//! Advances the being in time by @p elapsed.
		void update(tick elapsed);

		//! Causes the being to take damage to the given part from the specified source being.
		//! @param damage Damage to be applied to this being.
		//! @param target_part The body part to damage.
		//! @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, body_part& target_part, std::optional<ent> opt_source_id);

		//! Causes the being to be healed by the specified source being.
		//! @param amount Health to be restored to this being.
		//! @param part The body part to heal.
		//! @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(ql::health amount, body_part& part, std::optional<ent> opt_source_id);

		void add_status(uptr<status> status);

	protected:
		static stats::being load_stats(char const* filepath);

	private:
		//!< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<tick> _action_delays;
		//! The queue of delayed actions and continuations to occur when this being is not busy.
		std::deque<uptr<action>> _delayed_actions;

		//! @todo Move this to its own component.
		std::vector<uptr<status>> _statuses;

		stats::being get_stats();
	};
}
