//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Heals a being.
	class Heal : public Spell
	{
	public:
		Heal() : Spell{10} {}

		std::string name() const override { return "Heal"; }

		Color color() const override { return Color::white; }

		std::optional<int> max_charges() const override { return 10; }

		double cast_time() const override { return 1.0; }

		double incant_time() const override { return 10.0; }

		double discharge_time() const override { return 1.0; }

		double cooldown() const override { return 5.0; }
	protected:
		Complete perform_cast(Being& caster, Action::cont_t cont) override;
	private:
		static constexpr double _cost_factor = 1.0;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 10;
	};
}
