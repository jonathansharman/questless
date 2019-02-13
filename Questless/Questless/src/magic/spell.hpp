//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"
#include "heal.hpp"
#include "shock.hpp"
#include "teleport.hpp"
#include "telescope.hpp"

#include "utility/id.hpp"
#include "utility/reference.hpp"

#include <string>
#include <variant>

namespace ql {
	struct action;
	struct being;
	struct gatestone;
	struct item;

	namespace magic {
		//! A magical spell that can be cast by a being.
		struct spell {
			std::variant<heal, shock, teleport, telescope> value;

			//! The name of this spell to show the player.
			std::string name() const;

			//! The magic color of this spell.
			magic::color color() const;

			//! The amount of cooldown time this spell puts on a gatestone that casts it.
			tick cooldown() const;

			//! Causes @p caster to casts this spell using @p gatestone and invokes @p cont upon success.
			complete cast(being& caster, gatestone& gatestone, action::cont cont) const;

			//! Creates an action that casts this spell using the gatestone with ID @p gatestone_id.
			uptr<action> cast_action(id<item> gatestone_id) const;

			//! The amount of time @p caster takes to incant this spell.
			tick incant_time(being const& caster) const;

			//! The base amount of time it takes to incant this spell, ignoring the skill of the caster.
			tick base_incant_time() const;
		};
	}
}
