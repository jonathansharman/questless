//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/eagle_eye.h"
#include "game.h"
#include "entities/beings/being.h"
#include "effects/effect.h"
#include "magic/mana_cost.h"
#include "entities/beings/statuses/eagle_eyed.h"

namespace ql::magic
{
	complete eagle_eye::perform_cast(being& caster, action::cont cont)
	{
		if (caster.mana < _cost) {
			return caster.agent().send_message
				( std::make_unique<message_not_enough_mana>(_cost - caster.mana)
				, [cont] { return cont(action::result::aborted); }
				);
		}
		caster.mana -= _cost;
		caster.add_status(std::make_unique<eagle_eyed>(caster.stats.vision.acuity, 50, caster.id)); //! @todo Get duration.
		the_game().add_effect(std::make_shared<eagle_eye_effect>(caster.coords, caster.id));

		return cont(action::result::success);
	}
}
