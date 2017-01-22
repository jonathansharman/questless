/**
* @file    Heal.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Heal spell class.
*/

#pragma once

#include "Spell.h"

namespace questless::spell
{
	class Heal : public Spell
	{
	public:
		Heal() : Spell{10} {}

		Color color() const override { return Color::white; }

		boost::optional<int> max_charges() const override { return 10; }

		double cast_time() const override { return 1.0; }

		double incant_time() const override { return 10.0; }

		double discharge_time() const override { return 1.0; }

		double cooldown() const override { return 5.0; }
	protected:
		Action::Complete perform_cast(Being& caster, Action::cont_t cont) override;
	private:
		static constexpr double _cost_factor = 1.0;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 10;
	};
}
