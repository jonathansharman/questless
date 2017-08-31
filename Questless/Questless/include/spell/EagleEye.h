//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Temporarily increases the caster's visual acuity.
	class EagleEye : public SpellBase<EagleEye>
	{
	public:
		std::string name() const final { return "Eagle Eye"; }

		Color color() const final { return Color::green; }

		double cooldown() const final { return 50.0; }

		double required_gatestone_mana() const final { return 10.0; }
	private:
		static constexpr double _cost = 10.0;

		Complete perform_cast(Being& caster, Action::cont_t cont) final;

		double base_incant_time() const final { return 10.0; }
	};
}
