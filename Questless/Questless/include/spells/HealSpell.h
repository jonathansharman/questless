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
	namespace HealSpellK
	{
		const double cost_factor = 1.0;
		const int range = 10;
	}

	class HealSpell : public Spell
	{
	public:
		HealSpell() : Spell(10) {}

		void perform(Game& game, Being& caster, cont_t cont) override;

		Color color() const override { return Color::white; }

		optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 5.0; }
	};
}

#endif