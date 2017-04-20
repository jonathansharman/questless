//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Spell.h"

namespace questless::spell
{
	//! Temporarily increases the caster's visual acuity.
	class EagleEye : public Spell
	{
	public:
		EagleEye() : Spell{10} {}

		std::string name() const override { return "Eagle Eye"; }

		Color color() const override { return Color::green; }

		std::optional<int> max_charges() const override { return 10; }

		double cast_time() const override { return 1.0; }

		double incant_time() const override { return 10.0; }

		double discharge_time() const override { return 1.0; }

		double cooldown() const override { return 50.0; }
	protected:
		Complete perform_cast(Being& caster, Action::cont_t cont) override;
	private:
		static constexpr double _cost = 10.0;
	};
}
