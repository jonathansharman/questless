/**
* @file    Action.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

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

	/// An action that a being can perform via its controlling agent.
	////
	class Action
	{
	public:
		enum class Result { success, aborted };

		using uptr = std::unique_ptr<Action>;
		using cont_t = std::function<Complete(Result)>;

		virtual ~Action() = default;

		/// @return The action's name/description, to be displayed to the player.
		virtual std::string name() const = 0;

		/// Performs the action.
		/// @param actor The being that is performing the action.
		/// @param cont The continuation function to call once the action completes.
		/// @return An Complete object.
		virtual Complete perform(Being& actor, cont_t cont) = 0;

		/// @todo Do these belong here?
		static std::function<bool(RegionTileCoords)> tile_in_range_predicate(Being& actor, int range);
		static std::function<bool(Being&)> being_in_range_predicate(Being& actor, int range);
	};
}
