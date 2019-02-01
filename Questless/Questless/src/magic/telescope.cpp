//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "telescope.hpp"

#include "agents/agent.hpp"
#include "agents/queries/message.hpp"
#include "game.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/statuses/telescoped.hpp"
#include "magic/charge_cost.hpp"

namespace ql::magic {
	complete telescope::perform_cast(being& caster, gatestone& gatestone, action::cont cont) {
		return charge_cost{gatestone, _cost}.check_and_incur(caster, [&caster, cont = std::move(cont)] {
			caster.add_status(umake<telescoped>(caster.stats.a.vision.acuity, 50_tick, caster.id)); //! @todo Get duration.
			caster.region->add_effect(smake<telescope_effect>(caster.coords, caster.id));

			return cont(action::result::success);
		});
	}
}
