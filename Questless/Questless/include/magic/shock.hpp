//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spell.hpp"

namespace ql::magic
{
	//! Discharges a bolt of electricity to strike a tile.
	class shock : public spell_base<shock>
	{
	public:
		std::string name() const final { return "Lightning Bolt"; }

		magic::color color() const final { return color::red; }

		double cooldown() const final { return 5.0; }
	private:
		static constexpr double _cost_factor = 0.2;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 3;

		complete perform_cast(being& caster, gatestone& gatestone, action::cont cont) final;

		double base_incant_time() const final { return 15.0; }
	};
}
