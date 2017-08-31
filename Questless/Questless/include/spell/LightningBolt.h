//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Summons a lightning bolt to strike a tile.
	class LightningBolt : public SpellBase<LightningBolt>
	{
	public:
		std::string name() const final { return "Lightning Bolt"; }

		Color color() const final { return Color::yellow; }

		double cooldown() const final { return 5.0; }

		double required_gatestone_mana() const final { return 20.0; }
	private:
		static constexpr double _cost_factor = 0.2;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 3;

		Complete perform_cast(Being& caster, Action::cont_t cont) final;

		double base_incant_time() const final { return 15.0; }
	};
}
