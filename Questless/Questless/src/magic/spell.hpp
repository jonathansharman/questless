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
		using spell = std::variant<heal, shock, teleport, telescope>;

		complete cast(spell const& spell, being& actor, gatestone& gatestone, action::cont cont);

		//! Creates an action that casts @p spell using @p gatestone.
		uptr<action> cast_action(spell const& spell, id<item> gatestone_id);

		//! The amount of time @p caster takes to incant @p spell.
		tick incant_time(spell const& spell, being const& caster);

		std::string name(spell const& spell);
		magic::color spell_color(spell const& spell);
		tick cooldown(spell const& spell);

		//! The base amount of time it takes to incant @p spell, ignoring the skill of the caster.
		tick base_incant_time(spell const& spell);
	}
}
