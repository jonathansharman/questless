//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/quantities.hpp"

#include <variant>

namespace ql::queries::message {
	struct arrow_miss {};
	struct cannot_equip {};
	struct entity_in_the_way {};
	struct gatestone_missing {};
	struct incant_failed_mute {};
	struct melee_miss {};
	struct not_enough_ammo {};
	struct not_enough_charge {
		mana charge_deficit;
	};

	using query = std::variant< //
		arrow_miss,
		cannot_equip,
		entity_in_the_way,
		gatestone_missing,
		incant_failed_mute,
		melee_miss,
		not_enough_ammo,
		not_enough_charge>;
}
