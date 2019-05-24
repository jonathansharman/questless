//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "action.hpp"

#include "magic/spell.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "utility/complete.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include <functional>
#include <optional>

namespace ql {
	namespace effects {
		struct effect;
	}

	//! Facilitates control of a being. Includes AIs and the player.
	struct agent {
		ent id;

		agent(ent id) : id{id} {}

		//! Chooses and executes an action for the agent's being to perform.
		virtual action::result act() = 0;

		//! Causes the agent to perceive the given effect, possibly updating its state accordingly.
		//! @param effect The effect to perceive.
		virtual void perceive(effects::effect const& effect) = 0;

	protected:
		action::result idle(tick duration);
		action::result turn(region_tile::direction direction);
		action::result walk(region_tile::direction direction);
		action::result fly();
		action::result drop(ent item_id);
		action::result toss(ent item_id);
	};
}
