//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "world/coordinates.hpp"
#include "utility/complete.hpp"

namespace ql
{
	class being;

	//! An action that a being can perform via its controlling agent.
	class action
	{
	public:
		enum class result { success, aborted };

		using cont = std::function<complete(result)>;

		virtual ~action() = default;

		//! The action's name/description, to be displayed to the player.
		virtual std::string name() const = 0;

		//! Performs the action.
		//! @param actor The being that is performing the action.
		//! @param cont The continuation function to call once the action completes.
		//! @warning Beware of side-effects when calling @p perform more than once.
		virtual complete perform(being& actor, cont cont) = 0;

		//! @todo Do these belong here?
		static std::function<bool(region_tile::point)> tile_in_range_predicate(being& actor, int range);
		static std::function<bool(being&)> being_in_range_predicate(being& actor, int range);
	};
}
