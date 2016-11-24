/**
* @file    Teleport.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the TeleportSpell class.
*/

#ifndef SPELL_TELEPORT_H
#define SPELL_TELEPORT_H

#include "Spell.h"

namespace questless::spell
{
	class Teleport : public Spell
	{
	public:
		Teleport() : Spell{10} {}

		Action::Complete perform(Being& caster, cont_t cont) override;

		Color color() const override { return Color::yellow; }

		boost::optional<int> max_charges() const override { return 10; }

		double cooldown() const override { return 15.0; }
	private:
		static constexpr double _cost_factor = 5.0;
		static constexpr int _range = 15;
	};
}

#endif
