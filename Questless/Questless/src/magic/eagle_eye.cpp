//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/eagle_eye.hpp"
#include "game.hpp"
#include "entities/beings/being.hpp"
#include "effects/effect.hpp"
#include "magic/mana_cost.hpp"
#include "entities/beings/statuses/eagle_eyed.hpp"

namespace ql::magic
{
	complete eagle_eye::perform_cast(being& caster, action::cont cont)
	{
		if (caster.mana < _cost) {
			return caster.agent().send_message
				( umake<message_not_enough_mana>(_cost - caster.mana)
				, [cont] { return cont(action::result::aborted); }
				);
		}
		caster.mana -= _cost;
		caster.add_status(umake<eagle_eyed>(caster.stats.vision.acuity, 50, caster.id)); //! @todo Get duration.
		the_game().add_effect(smake<eagle_eye_effect>(caster.coords, caster.id));

		return cont(action::result::success);
	}
}
