//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "color.hpp"

#include "agents/action.hpp"
#include "items/magic/gatestone.hpp"
#include "quantities/quantities.hpp"
#include "spell_visitor.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"

#include <string>

namespace ql::magic {
	//! A magical spell that can be cast by a being.
	class spell : public element<spell_subtype_list> {
	public:
		virtual ~spell() = default;

		//! An action that casts @p spell using @p gatestone.
		static uptr<action> cast(uptr<spell> spell, id<item> gatestone_id);

		//! The amount of time @p caster takes to incant this spell.
		tick incant_time(being& caster) const;

		//! This spell's name.
		virtual std::string name() const = 0;

		//! This spell's color.
		virtual color color() const = 0;

		//! Time after casting this spell before the gatestone used to cast it can be used again.
		virtual tick cooldown() const = 0;
	private:
		friend class cast;

		//! The base amount of time it takes to incant this spell, ignoring the skill of the caster.
		virtual tick base_incant_time() const = 0;

		virtual complete perform_cast(being& actor, gatestone& gatestone, action::cont cont) = 0;
	};

	DEFINE_ELEMENT_BASE(spell, spell)
}
