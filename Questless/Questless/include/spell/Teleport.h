/**
* @file    Teleport.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "Spell.h"

namespace questless::spell
{
	/// A spell that teleports the caster some distance from its current location.
	////
	class Teleport : public Spell
	{
	public:
		Teleport() : Spell{10} {}

		std::string name() const override { return "Teleport"; }

		Color color() const override { return Color::yellow; }

		std::optional<int> max_charges() const override { return 10; }

		double cast_time() const override { return 0.2; }

		double incant_time() const override { return 20.0; }

		double discharge_time() const override { return 1.0; }

		double cooldown() const override { return 15.0; }
	protected:
		Action::Complete perform_cast(Being& caster, Action::cont_t cont) override;
	private:
		static constexpr double _cost_factor = 5.0;
		static constexpr int _range = 15;
	};
}
