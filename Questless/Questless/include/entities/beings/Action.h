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

#include "utility/hex-utilities.h"

/// @todo UPDATE ALL DOCUMENTATION IN THIS FILE. REMOVE UNUSED CLASSES AFTER CONTINUATION REFACTOR.

namespace questless
{
	class Game;
	class Being;

	class Action
	{
	public:
		enum class Result { success, aborted };

		using ptr = std::unique_ptr<Action>;
		using cont_t = std::function<void(Result)>;

		virtual ~Action() = default;

		/// @return The action's name/description, to be displayed to the player.
		virtual std::string name() const = 0;

		/// Performs the action.
		/// @param game The game object.
		/// @param actor The being that is performing the action.
		/// @param [in,out] cont The continuation function. If the action does not complete, this function can be invoked later to resume it.
		virtual void perform(Game& game, Being& actor, cont_t cont) = 0;

		static std::function<bool(HexCoords)> tile_in_range_predicate(Being& actor, int range);
		static std::function<bool(Being&)> being_in_range_predicate(Being& actor, int range);
	};
}

#endif
