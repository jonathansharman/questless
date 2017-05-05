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
		LightningBolt() : SpellBase<LightningBolt>{10} {}

		std::string name() const final { return "Lightning Bolt"; }

		Color color() const final { return Color::yellow; }

		std::optional<int> max_charges() const final { return 10; }

		double cast_time() const final { return 0.2; }

		double incant_time() const final { return 15.0; }

		double discharge_time() const final { return 1.0; }

		double cooldown() const final { return 5.0; }
	protected:
		Complete perform_cast(Being& caster, Action::cont_t cont) final;
	private:
		static constexpr double _cost_factor = 0.2;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 3;
	};
}
