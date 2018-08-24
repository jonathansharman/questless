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

		double cooldown() const final { return 5.0; }
	private:
		static constexpr double _cost_factor = 1.0;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 10;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		double base_incant_time() const final { return 10.0; }
	};
}
