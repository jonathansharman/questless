//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic {
	//! Heals a being.
	class heal : public spell_base<heal> {
	public:
		std::string name() const final { return "Heal"; }

		magic::color color() const final { return color::white; }

		tick cooldown() const final { return 5_tick; }
	private:
		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		tick base_incant_time() const final { return 10_tick; }
	};
}
