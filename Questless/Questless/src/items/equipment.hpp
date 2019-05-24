//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"

#include "entities/beings/body_part.hpp"

#include "cancel/quantity.hpp"

#include <optional>

namespace ql {
	//! Component for items that can be equipped.
	struct equipment {
		//! "Slots" into a body part.
		struct tab {
			//! The type of part this tab can slot into.
			body_part::tag tag;
			//! The body part this tab is slotted into or nullopt if none.
			std::optional<ent> opt_part_id;
		};

		ent id;

		//! The ID of the bearer of this equipment, or nullopt if not equipped.
		std::optional<ent> opt_bearer_id;

		//! The list of "tabs" that "slot" into the bearer's body parts.
		std::vector<tab> tabs;

		//! The amount of time required to equip the item.
		tick equip_time;

		//! The amount of time required to unequip the item.
		tick unequip_time;

		//! Whether the item is currently equipped to some being.
		bool equipped() const {
			return opt_bearer_id.has_value();
		}

		//! Unequips the item from its bearer, without incurring the unequip time.
		void unequip();
	};

	struct equip : action {
		ent equipment_id;
		result perform(being& actor) final;
	};

	struct unequip : action {
		ent equipment_id;
		result perform(being& actor) final;
	};
}
