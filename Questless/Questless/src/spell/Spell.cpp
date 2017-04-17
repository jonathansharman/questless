//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell/Spell.h"
#include "entities/beings/Being.h"
#include "agents/Agent.h"

namespace questless::spell
{
	Complete Spell::Cast::perform(Being& actor, cont_t cont)
	{
		class CompleteCast : public Action
		{
		public:
			CompleteCast(Spell& spell) : _spell{spell} {}
			static uptr make(Spell& spell) { return std::make_unique<CompleteCast>(spell); }
			std::string name() const override { return ""; }
			Complete perform(Being& actor, cont_t cont) override { return _spell.perform_cast(actor, cont); }
		private:
			Spell& _spell;
		};

		if (_spell.active_cooldown() > 0.0) {
			return actor.agent().send_message(std::make_unique<MessageSpellOnCooldown>(_spell.active_cooldown()), [cont] { return cont(Action::Result::aborted); });
		}
		if (_spell.charges() <= 0) {
			return actor.agent().send_message(std::make_unique<MessageSpellNotEnoughCharges>(), [cont] { return cont(Action::Result::aborted); });
		}
		actor.add_delayed_action(_spell.cast_time() / (1.0 + Being::intellect_factor * actor.stats.intellect), cont, CompleteCast::make(_spell));
		return Complete{};
	}

	Complete Spell::Incant::perform(Being& actor, cont_t cont)
	{
		if (actor.stats.mute) {
			return actor.agent().send_message(std::make_unique<MessageIncantFailedMute>(), [cont] { return cont(Action::Result::aborted); });
		}
		actor.busy_time += _spell.incant_time() / (1.0 + Being::intellect_factor * actor.stats.intellect);
		_spell.gain_charge(1);
		return cont(Result::success);
	}

	Complete Spell::Discharge::perform(Being& actor, cont_t cont)
	{
		actor.busy_time += _spell.discharge_time() / (1.0 + Being::intellect_factor * actor.stats.intellect);
		_spell.lose_charge(1);
		return cont(Result::success);
	}

	//! @todo I don't like these optional things... There has to be a better way. Also, make these properties.

	void Spell::gain_charge(int amount)
	{
		if (std::optional<int> max = max_charges()) {
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
