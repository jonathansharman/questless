/**
* @file    Item.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Action abstract base class, which encapsulates an action that a being can perform.
*/

#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "world/coordinates.h"

namespace questless
{
	class Being;

	class Action
	{
	public:
		/// Trivial type to return from the perform() method upon completion. The purpose of this return value is to enforce either calling the continuation at the end of the perform function or explicitly documenting the decision not to do so in that return path. Without this return type, it's too easy to forget to invoke the continuation or to invoke it without returning.
		struct Complete {};

		enum class Result { success, aborted };

		using ptr = std::unique_ptr<Action>;
		using cont_t = std::function<Complete(Result)>;

		virtual ~Action() = default;

		/// @return The action's name/description, to be displayed to the player.
		virtual std::string name() const = 0;

		/// Performs the action.
		/// @param actor The being that is performing the action.
		/// @param cont The continuation function to call once the action completes.
		/// @return An Action::Complete object.
		virtual Complete perform(Being& actor, cont_t cont) = 0;

		static std::function<bool(RegionTileCoords)> tile_in_range_predicate(Being& actor, int range);
		static std::function<bool(Being&)> being_in_range_predicate(Being& actor, int range);
	};
}

#endif
