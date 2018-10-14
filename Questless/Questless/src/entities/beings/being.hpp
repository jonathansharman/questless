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
#include "utility/dynamic_property.hpp"
#include "utility/event.hpp"
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
		using ref_less_t = bool(*)(being const&, being const&);
		using ptr_less_t = bool(*)(uptr<being> const&, uptr<being> const&);

		///////////////////////////
		// Public Data/Accessors //
		///////////////////////////

		id<being> const id;

		body body;

		inventory inventory;

		// Stats

		stats::being base_stats; //!< Base stats, prior to application of status modifiers.
		stats::being stats; //!< Effective stats, accounting for all modifiers from base.

		// Conditions

		lazy_bounded<ql::energy> energy;
		constexpr bool weary() const { return energy.value() < 0.5 * stats.a.stamina.value(); }

		static constexpr auto min_satiety = 0.0_sat;
		static constexpr auto max_satiety = 100.0_sat;
		static_bounded<ql::satiety, min_satiety, max_satiety> satiety;
		constexpr bool hungry() const { return satiety.value() < min_satiety + 0.5 * (max_satiety - min_satiety); }
		constexpr bool starving() const { return satiety.value() == min_satiety; }

		static constexpr auto min_alertness = 0.0_alert;
		static constexpr auto max_alertness = 100.0_alert;
		static_bounded<ql::alertness, min_alertness, max_alertness> alertness;
		constexpr bool sleepy() const { return alertness.value() < min_alertness + 0.5 * (max_alertness - min_alertness); }

		static constexpr auto min_joy = -100.0_joy;
		static constexpr auto max_joy = 100.0_joy;
		static_bounded<ql::joy, min_joy, max_joy> joy;
		constexpr bool sad() const { return joy.value() < min_joy + 0.25 * (max_joy - min_joy); }
		constexpr bool happy() const { return joy.value() > min_joy + 0.75 * (max_joy - min_joy); }

		static constexpr auto min_courage = -100.0_courage;
		static constexpr auto max_courage = 100.0_courage;
		static_bounded<ql::courage, min_courage, max_courage> courage;
		constexpr bool afraid() const { return courage.value() < min_courage + 0.25 * (max_courage - min_courage); }
		constexpr bool brave() const { return courage.value() > min_courage + 0.75 * (max_courage - min_courage); }

		static constexpr auto min_serenity = -100.0_serenity;
		static constexpr auto max_serenity = 100.0_serenity;
		static_bounded<ql::serenity, min_serenity, max_serenity> serenity;
		constexpr bool angry() const { return serenity.value() < min_serenity + 0.25 * (max_serenity - min_serenity); }
		constexpr bool calm() const { return serenity.value() > min_serenity + 0.75 * (max_serenity - min_serenity); }

		dynamic_property<ql::tick> busy_time;

		mortality mortality;

		region_tile::direction direction;

		ql::awake awake;

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
		perception::level perception_of(region_tile::point region_tile_coords) const;

		//! Causes the being to perform an action.
		complete act();

		//! Adds the given delayed action to the being's delayed actions queue.
		//! @param delay The delay before the action is performed.
		//! @param cont The continuation function to call once the action completes.
		//! @param action The action to perform after the delay.
		complete add_delayed_action(tick delay, action::cont cont, uptr<action> action);

		//! Clears the being's delayed actions queue.
		void clear_delayed_actions();

		//! The being's equipped shields.
		std::vector<uptr<armor>> const& shields() { return _shields; }

		//! Recalculates this being's stats. Called automatically with each update.
		void refresh_stats() { stats = get_stats(); }

		//! Advances the being in time by @p elapsed.
		void update(tick elapsed) final;

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

		std::deque<tick> _action_delays; //!< The queue of delays before the next action in the delayed actions queue should begin.
		std::deque<std::tuple<uptr<action>, action::cont>> _delayed_actions; //!< The queue of delayed actions and continuations to occur when this being is not busy.

		std::vector<uptr<status>> _statuses;

		std::vector<uptr<weapon>> _equipped_weapons;
		std::vector<uptr<armor>> _shields;

		stats::being get_stats();

		std::function<void(tick&, tick const&)> busy_time_mutator();
	};

	DEFINE_ELEMENT_BASE(being, entity)
}
