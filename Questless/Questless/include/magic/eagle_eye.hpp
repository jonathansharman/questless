//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic {
	//! Temporarily increases the caster's visual acuity.
	class eagle_eye : public spell_base<eagle_eye> {
	public:
		std::string name() const final { return "Eagle Eye"; }

		magic::color color() const final { return color::green; }

		double cooldown() const final { return 50.0; }
	private:
		static constexpr double _cost = 10.0;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		double base_incant_time() const final { return 10.0; }
	};
}
