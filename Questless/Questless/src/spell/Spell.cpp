/**
* @file    Spell.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Spell abstract base class.
*/

#include "spell/Spell.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

namespace questless::spell
{
	Action::Complete Spell::Cast::perform(Being& actor, cont_t cont)
	{
		class CompleteCast : public Action
		{
		public:
			CompleteCast(Spell& spell) : _spell{spell} {}

			static ptr make(Spell& spell) { return std::make_unique<CompleteCast>(spell); }

			std::string name() const override { return ""; }

			Action::Complete perform(Being& actor, cont_t cont) override
			{
				return _spell.perform_cast(actor, cont);
			}
		private:
			Spell& _spell;
		};

		if (_spell.active_cooldown() > 0.0) {
			return actor.agent().message("This spell is on cooldown.", "This spell will be ready in " + std::to_string(_spell.active_cooldown()) + ".", [cont] { return cont(Action::Result::aborted); });
		}
		if (_spell.charges() <= 0) {
			return actor.agent().message("Out of charges!", "You need to incant this spell first.", [cont] { return cont(Action::Result::aborted); });
		}
		actor.add_delayed_action(_spell.cast_time() / (1.0 + Being::intellect_factor * actor.intellect()), cont, CompleteCast::make(_spell));
		return Action::Complete{};
	}

	Action::Complete Spell::Incant::perform(Being& actor, cont_t cont)
	{
		if (actor.mute()) {
			return actor.agent().message("You are mute!", "You can't incant a spell if you are mute.", [cont] { return cont(Action::Result::aborted); });
		}
		actor.gain_busy_time(_spell.incant_time() / (1.0 + Being::intellect_factor * actor.intellect()));
		_spell.gain_charge(1);
		return cont(Result::success);
	}

	Action::Complete Spell::Discharge::perform(Being& actor, cont_t cont)
	{
		actor.gain_busy_time(_spell.discharge_time() / (1.0 + Being::intellect_factor * actor.intellect()));
		_spell.lose_charge(1);
		return cont(Result::success);
	}

	/// @todo I don't like these optional things... There has to be a better way.

	void Spell::gain_charge(int amount)
	{
		if (boost::optional<int> max = max_charges()) {
			_charges += amount;
			if (_charges > *max) {
				_charges = *max;
			} else if (_charges < 0) {
				_charges = 0;
			}
		}
	}

	void Spell::lose_charge(int amount)
	{
		if (max_charges()) {
			_charges -= amount;
			if (_charges < 0) {
				_charges = 0;
			}
		}
	}

	void Spell::update()
	{
		_active_cooldown -= 1.0;
		if (_active_cooldown < 0.0) {
			_active_cooldown = 0.0;
		}
	}
}
