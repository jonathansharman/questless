//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "agents/Action.h"
#include "SpellVisitor.h"
#include "utility/DynamicBounded.h"
#include "utility/reference.h"

namespace questless::spell
{
	enum class Color { white, black, green, red, blue, yellow };

	//! A magical spell that can be cast by a being.
	class Spell : public Element<SpellSubtypeList>
	{
	public:
		virtual ~Spell() = default;

		//! An action that casts @p spell.
		static uptr<Action> cast(uptr<Spell> spell);

		//! The amount of time @p caster takes to incant this spell.
		double incant_time(Being& caster) const;

		//! This spell's name.
		virtual std::string name() const = 0;

		//! This spell's color.
		virtual Color color() const = 0;

		//! Time after casting this spell before the gatestone used to cast it can be used again.
		virtual double cooldown() const = 0;

		//! The amount of gatestone mana required to cast this spell from a gatestone.
		virtual double required_gatestone_mana() const = 0;
	private:
		//! The base amount of time it takes to incant this spell, ignoring the skill of the caster.
		virtual double base_incant_time() const = 0;

		virtual Complete perform_cast(Being& actor, Action::cont_t cont) = 0;
	};

	DEFINE_ELEMENT_BASE(Spell, Spell)
}
