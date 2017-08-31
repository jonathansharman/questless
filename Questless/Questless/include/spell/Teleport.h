//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Teleports the caster some distance from its current location.
	class Teleport : public SpellBase<Teleport>
	{
	public:
		std::string name() const final { return "Teleport"; }

		Color color() const final { return Color::yellow; }\

		double cooldown() const final { return 15.0; }

		double required_gatestone_mana() const final { return 30.0; }
	private:
		static constexpr double _cost_factor = 5.0;
		static constexpr int _range = 15;

		Complete perform_cast(Being& caster, Action::cont_t cont) final;

		double base_incant_time() const final { return 20.0; }
	};
}
