//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic {
	//! Teleports the caster some distance from its current location.
	class teleport : public spell_base<teleport> {
	public:
		std::string name() const final { return "Teleport"; }

		magic::color color() const final { return color::yellow; }

		double cooldown() const final { return 15.0; }
	private:
		static constexpr double _cost_factor = 5.0;
		static constexpr int _range = 15;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		double base_incant_time() const final { return 20.0; }
	};
}
