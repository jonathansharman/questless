//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic {
	//! Temporarily increases the caster's visual acuity.
	class telescope : public spell_base<telescope> {
	public:
		std::string name() const final {
			return "Telescope";
		}

		magic::color color() const final {
			return color::green;
		}

		tick cooldown() const final {
			return 50_tick;
		}

	private:
		static constexpr mana _cost = 10.0_mp;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		tick base_incant_time() const final {
			return 10_tick;
		}
	};
}
