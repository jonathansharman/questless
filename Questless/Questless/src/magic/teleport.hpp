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

		tick cooldown() const final { return 15_tick; }
	private:
		static constexpr auto _cost_factor = 5.0_mp / 1_span;
		static constexpr span _range = 15_span;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		tick base_incant_time() const final { return 20_tick; }
	};
}
