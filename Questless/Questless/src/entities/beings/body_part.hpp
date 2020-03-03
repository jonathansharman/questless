//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "attachment.hpp"
#include "body_part_cond.hpp"
#include "body_part_status_set.hpp"

#include "damage/group.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "stats/part.hpp"
#include "ui/view_space.hpp"
#include "utility/reference.hpp"
#include "utility/utility.hpp"

#include "vecx/angle.hpp"

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace ql {
	//! A being's body part.
	struct body_part {
		enum class tag : int { head = 0, torso, arm, hand, leg, foot, wing, tail };

		id id;

		//! The ID of the being that owns this body part.
		ql::id owner_id;

		//! The player-visisble name of this body part.
		std::string name{};

		//! The tags that determine what kind of part this is.
		std::unordered_set<tag> tags{};

		//! This body part's conditions.
		body_part_cond cond{};

		//! This body part's stats.
		stats::part stats{};

		//! This body part's set of status modifiers.
		body_part_status_set status_set{};

		//! The draw layer, with smaller-numbered layers drawn first (i.e. in the background).
		int layer = 0;

		//! The body part attachment points on this body part.
		std::vector<attachment> attachments{};

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ql::id> equipped_item_id = std::nullopt;

		body_part(reg& reg, ql::id id, ql::id owner_id);

		//! Advances the body part by @p elapsed.
		auto update(tick elapsed) -> void;

		//! Causes this part to take damage.
		//! @param damage Damage to be applied to this part.
		//! @param o_source_id The ID of the being which caused the damage, if any.
		auto take_damage(dmg::group& damage, std::optional<ql::id> o_source_id) -> void;

		//! Recursively generates each missing attached body part using default parts.
		auto generate_attached_parts() -> void;

	private:
		reg_ptr _reg;
	};
}
