//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic {
	//! Discharges a bolt of electricity to strike a tile.
	class shock : public spell_base<shock> {
	public:
		std::string name() const final { return "Lightning Bolt"; }

		magic::color color() const final { return color::red; }

		tick cooldown() const final { return 5_tick; }
	private:
		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		tick base_incant_time() const final { return 15_tick; }
	};
}
