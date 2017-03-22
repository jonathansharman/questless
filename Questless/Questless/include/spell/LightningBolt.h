/**
* @file    LightningBolt.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the LightningBolt spell class.
*/

#pragma once

#include "Spell.h"

namespace questless::spell
{
	class LightningBolt : public Spell
	{
	public:
		LightningBolt() : Spell{10} {}

		std::string name() const override { return "Lightning Bolt"; }

		Color color() const override { return Color::yellow; }

		boost::optional<int> max_charges() const override { return 10; }

		double cast_time() const override { return 0.2; }

		double incant_time() const override { return 15.0; }

		double discharge_time() const override { return 1.0; }

		double cooldown() const override { return 5.0; }
	protected:
		Action::Complete perform_cast(Being& caster, Action::cont_t cont) override;
	private:
		static constexpr double _cost_factor = 0.2;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 3;
	};
}
