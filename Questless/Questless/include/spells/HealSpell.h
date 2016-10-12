/**
* @file    HealSpell.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the HealSpell class.
*/

#ifndef HEAL_SPELL_H
#define HEAL_SPELL_H

#include "Spell.h"

namespace questless
{
	class HealSpell : public Spell
	{
	public:
		HealSpell() : Spell(10) {}

		Action::Complete perform(Being& caster, cont_t cont) override;

		Color color() const override { return Color::white; }

		boost::optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 5.0; }
	private:
		static constexpr double _cost_factor = 1.0;
		static constexpr double _cost_log = 2.0;
		static constexpr int _range = 10;
	};
}

#endif
