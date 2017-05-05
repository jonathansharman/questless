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
		EagleEye() : SpellBase<EagleEye>{10} {}

		std::string name() const final { return "Eagle Eye"; }

		Color color() const final { return Color::green; }

		std::optional<int> max_charges() const final { return 10; }

		double cast_time() const final { return 1.0; }

		double incant_time() const final { return 10.0; }

		double discharge_time() const final { return 1.0; }

		double cooldown() const final { return 50.0; }
	protected:
		Complete perform_cast(Being& caster, Action::cont_t cont) final;
	private:
		static constexpr double _cost = 10.0;
	};
}
