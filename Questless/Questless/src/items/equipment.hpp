//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "item.hpp"

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
			std::optional<id> o_part_id;
		};

		id id;

		//! The ID of the bearer of this equipment, or nullopt if not equipped.
		std::optional<ql::id> o_bearer_id;

		//! The list of "tabs" that "slot" into the bearer's body parts.
		std::vector<tab> tabs;

		//! The action points required to equip this.
		action equip_cost;

		//! The action points required to unequip this.
		action unequip_cost;

		//! Whether the item is currently equipped to some being.
		auto equipped() const -> bool {
			return o_bearer_id.has_value();
		}

		//! Equips this to the being with ID @p bearer_id.
		auto equip(ql::id bearer_id) -> void;

		//! If equipped, unequips from the bearer, incurring costs.
		auto unequip() -> void;

		//! If equipped, immediately unequips from the bearer, without incurring costs.
		auto forced_unequip() -> void;
	};

	auto make_equipment(id equipment_id, std::optional<id> o_bearer_id, std::vector<equipment::tab> tabs, action equip_cost, action unequip_cost)
		-> id;
}
