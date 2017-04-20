//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "spell/EagleEye.h"
#include "Game.h"
#include "entities/beings/Being.h"
#include "effects/Effect.h"
#include "spell/ManaCost.h"
#include "entities/beings/statuses/EagleEyed.h"

namespace questless::spell
{
	Complete EagleEye::perform_cast(Being& caster, Action::cont_t cont)
	{
		if (caster.mana < _cost) {
			return caster.agent().send_message
				( std::make_unique<MessageNotEnoughMana>(_cost - caster.mana)
				, [cont] { return cont(Action::Result::aborted); }
				);
		}
		active_cooldown(cooldown());
		discharge();
		caster.mana -= _cost;
		caster.add_status(std::make_unique<EagleEyed>(caster.stats.vision.acuity, 50, caster.id)); //! @todo Get duration.
		game().add_effect(std::make_shared<EagleEyeEffect>(caster.coords, caster.id));

		return cont(Action::Result::success);
	}
}
