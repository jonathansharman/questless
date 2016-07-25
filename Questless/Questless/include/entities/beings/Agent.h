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
	class Agent
	{
	public:
		using ptr = std::unique_ptr<Agent>;

		Agent(Being& being) : _being{being} {}

		template <typename Derived>
		static ptr make(Being& being) { return ptr(new Derived{being}); }

		virtual ~Agent() = default;

		Agent& operator =(const Agent& other) = delete;

		Being& being() { return _being; }
		const Being& being() const { return _being; }

		/// Chooses an action for the being to perform and executes it, possibly finishing in a later update cycle.
		/// @param game The game object, used by the agent to query the game and world state.
		/// @param cont [out] The continuation to call in the calling code in the next update cycle if the action is not finished.
		/// @return whether the action is finished.
		/// @note Doing nothing counts as an action. AI always act immediately, so they will be always return true. The player agent may take multiple update cycles because it's waiting for player input.
		virtual void act() = 0;

		/// @todo UPDATE DOCUMENTATION HERE.

		// Queries and messages

		virtual void message
			( const std::string& title
			, const std::string& prompt
			, std::function<void()> cont
			) const = 0;

		virtual void query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, optional<int> min
			, optional<int> max
			, std::function<void(optional<int>)> cont
			) const = 0;
		virtual void query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_duration
			( const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_magnitude
			( const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<void(optional<double>)> cont
			) const = 0;

		virtual void query_tile
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<void(optional<RegionTileCoords>)> cont
			) const = 0;

		virtual void query_being
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Being*>)> cont
			) const = 0;

		virtual void query_range
			( const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const = 0;

		virtual void query_item
			( const std::string& title
			, const std::string& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Item*>)> cont
			) const = 0;
	private:
		Being& _being;
	};
}

#endif