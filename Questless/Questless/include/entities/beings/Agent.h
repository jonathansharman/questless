/**
* @file    Agent.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Agent abstract base class, representing an agent acting on behalf of some number of beings. E.g., the player agent.
*/

#ifndef AGENT_H
#define AGENT_H

#include <functional>
#include <stdexcept>

#include "Being.h"
#include "WorldView.h"
#include "items/Item.h"
#include "utility/optional.h"

namespace questless
{
	class Game;

	class Agent : public std::enable_shared_from_this<Agent>
	{
	public:
		using ptr = std::unique_ptr<Agent>;

		Agent(Being& being) : _being{being} {}

		template <typename Derived>
		static ptr make(Being& being) { return ptr(new Derived{being}); }

		virtual ~Agent() = default;

		Agent& operator =(const Agent& other) = delete;

		/// Chooses an action for the being to perform and executes it, possibly finishing in a later update cycle.
		/// @param game The game object, used by the agent to query the game and world state.
		/// @param cont [out] The continuation to call in the calling code in the next update cycle if the action is not finished.
		/// @return whether the action is finished.
		/// @note Doing nothing counts as an action. AI always act immediately, so they will be always return true. The player agent may take multiple update cycles because it's waiting for player input.
		virtual void act(Game& game) = 0;

		/// @todo UPDATE DOCUMENTATION HERE.

		// Queries and messages

		virtual void message
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void()> cont
			) const = 0;

		virtual void query_count
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, int default
			, optional<int> min
			, optional<int> max
			, std::function<void(optional<int>)> cont
			) const = 0;
		virtual void query_count
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_duration
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_magnitude
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<void(optional<double>)> cont
			) const = 0;

		virtual void query_tile
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<bool(HexCoords)> predicate
			, std::function<void(optional<HexCoords>)> cont
			) const = 0;

		virtual void query_being
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Being*>)> cont
			) const = 0;

		virtual void query_range
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_item
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, const Being& source
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Item*>)> cont
			) const = 0;
	protected:
		Being& _being;
	};
}

#endif