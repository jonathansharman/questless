/**
* @file    LightningBolt.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the LightningBolt spell class.
*/

#ifndef SPELL_LIGHTNING_BOLT_H
#define SPELL_LIGHTNING_BOLT_H

#include "Spell.h"

namespace questless::spell
{
	class LightningBolt : public Spell
	{
	public:
		LightningBolt() : Spell{10} {}

		Action::Complete perform(Being& caster, cont_t cont) override;

		Color color() const override { return Color::yellow; }

		boost::optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 5.0; }
	private:
		static constexpr double _cost_factor = 0.2;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 3;
	};
}

#endif
