//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

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
	//! An attachment point on a body part.
	struct attachment {
		//! The ID of the being that owns this body part attachment.
		ent owner_id;

		//! The body part currently attached here or nullopt if there is no attached part.
		std::optional<ent> o_part_id;

		//! The offset from the parent part's origin to the child part's origin.
		sf::Vector2f offset;

		//! The rotation of the attached part about the attachment point.
		vecx::radians rotation;

		//! The resource filepath to the default part for this attachment.
		char const* default_part;

		//! Creates an instance of the body part that attaches here by default.
		//! @param owner_id The ID of the being that owns the new body part.
		//! @return The ID of the new body part.
		ent make_default() {
			ent id = reg.create();
			//! @todo This component assignment is not compiling. Why?
			// reg.assign<body_part>(id, id, default_part, owner_id);
			return id;
		}
	};

	//! A being's body part.
	struct body_part {
		enum class tag : int { head = 0, torso, arm, hand, leg, foot, wing, tail };

		ent id;

		//! The ID of the being that owns this body part.
		ent owner_id;

		//! The player-visisble name of this body part.
		std::string name;

		//! The tags that determine what kind of part this is.
		std::unordered_set<tag> tags;

		//! This body part's conditions.
		body_part_cond cond;

		//! This body part's base stats, before any stat-modifying effects are applied.
		stats::part base_stats{};

		//! This body part's current stats.
		stats::part stats{};

		body_part_status_set status_set;

		//! The draw layer, with smaller-numbered layers drawn first (i.e. in the background).
		int layer = 0;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ent> equipped_item_id = std::nullopt;

		//! The body part attachment points on this body part.
		std::vector<attachment> attachments{};

		body_part() = default;

		//! Advances the body part by @p elapsed.
		void update(tick elapsed);

		//! Causes this part to take damage.
		//! @param damage Damage to be applied to this part.
		//! @param o_source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, std::optional<ent> o_source_id);

		//! Recursively generates each missing attached body part using default parts.
		void generate_attached_parts();
	};
}
