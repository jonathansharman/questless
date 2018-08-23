//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magic/eagle_eye.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "game.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/statuses/eagle_eyed.hpp"
#include "magic/charge_cost.hpp"

namespace ql::magic {
	complete eagle_eye::perform_cast(being& caster, gatestone& gatestone, action::cont cont) {
		return charge_cost{gatestone, _cost}.check_and_incur(caster, [&caster, cont = std::move(cont)] {
			caster.add_status(umake<eagle_eyed>(caster.stats.vision.acuity, 50, caster.id)); //! @todo Get duration.
			caster.region->add_effect(smake<eagle_eye_effect>(caster.coords, caster.id));

			return cont(action::result::success);
		});
	}
}
