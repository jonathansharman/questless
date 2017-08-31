//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Heals a being.
	class Heal : public SpellBase<Heal>
	{
	public:
		std::string name() const final { return "Heal"; }

		Color color() const final { return Color::white; }

		double cooldown() const final { return 5.0; }

		double required_gatestone_mana() const final { return 10.0; }
	private:
		static constexpr double _cost_factor = 1.0;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 10;

		Complete perform_cast(Being& caster, Action::cont_t cont) final;

		double base_incant_time() const final { return 10.0; }
	};
}
