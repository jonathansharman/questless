/**
* @file    LightningBoltSpell.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the LightningBoltSpell class.
*/

#ifndef LIGHTNING_BOLT_SPELL_H
#define LIGHTNING_BOLT_SPELL_H

#include "Spell.h"

namespace questless
{
	namespace LightningBoltSpellK
	{
		const double cost_factor = 0.2;
		const int range = 3;
	}

	class LightningBoltSpell : public Spell
	{
	public:
		LightningBoltSpell() : Spell(10) {}

		void perform(Game& game, Being& caster, cont_t cont) override;

		Color color() const override { return Color::yellow; }

		optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 5.0; }
	};
}

#endif