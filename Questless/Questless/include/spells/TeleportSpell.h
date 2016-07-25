/**
* @file    TeleportSpell.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the TeleportSpell class.
*/

#ifndef TELEPORT_SPELL_H
#define TELEPORT_SPELL_H

#include "Spell.h"

namespace questless
{
	class TeleportSpell : public Spell
	{
	public:
		TeleportSpell() : Spell(10) {}

		void perform(Being& caster, cont_t cont) override;

		Color color() const override { return Color::yellow; }

		optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 15.0; }
	private:
		static constexpr double _cost_factor = 5.0;
		static constexpr int _range = 15;
	};
}

#endif
