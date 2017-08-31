//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "world/coordinates.h"
#include "utility/Complete.h"

namespace questless
{
	class Being;

	//! An action that a being can perform via its controlling agent.
	class Action
	{
	public:
		enum class Result { success, aborted };

		using cont_t = std::function<Complete(Result)>;

		virtual ~Action() = default;

		//! The action's name/description, to be displayed to the player.
		virtual std::string name() const = 0;

		//! Performs the action.
		//! @param actor The being that is performing the action.
		//! @param cont The continuation function to call once the action completes.
		//! @warning Beware of side-effects when calling @p perform more than once.
		virtual Complete perform(Being& actor, cont_t cont) = 0;

		//! @todo Do these belong here?
		static std::function<bool(RegionTile::Point)> tile_in_range_predicate(Being& actor, int range);
		static std::function<bool(Being&)> being_in_range_predicate(Being& actor, int range);
	};
}
