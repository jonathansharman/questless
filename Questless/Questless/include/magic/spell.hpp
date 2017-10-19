//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "agents/action.hpp"
#include "spell_visitor.hpp"
#include "utility/dynamic_bounded.hpp"
#include "utility/reference.hpp"

namespace ql::magic
{
	enum class color
		{ white
		, black
		, green
		, red
		, blue
		, yellow
		, violet
		, orange
		};

	//! A magical spell that can be cast by a being.
	class spell : public element<spell_subtype_list>
	{
	public:
		virtual ~spell() = default;

		//! An action that casts @p spell.
		static uptr<action> cast(uptr<spell> spell);

		//! The amount of time @p caster takes to incant this spell.
		double incant_time(being& caster) const;

		//! This spell's name.
		virtual std::string name() const = 0;

		//! This spell's color.
		virtual color color() const = 0;

		//! Time after casting this spell before the gatestone used to cast it can be used again.
		virtual double cooldown() const = 0;

		//! The amount of gatestone mana required to cast this spell from a gatestone.
		virtual double required_gatestone_mana() const = 0;
	private:
		//! The base amount of time it takes to incant this spell, ignoring the skill of the caster.
		virtual double base_incant_time() const = 0;

		virtual complete perform_cast(being& actor, action::cont cont) = 0;
	};

	DEFINE_ELEMENT_BASE(spell, spell)
}
