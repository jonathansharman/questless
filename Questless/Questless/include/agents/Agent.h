/**
* @file    Agent.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Agent abstract base class, representing an agent acting on behalf of some number of beings. E.g., the player agent.
*/

#pragma once

#include <functional>
#include <stdexcept>

#include <boost/optional.hpp>

#include "entities/beings/Being.h"
#include "entities/beings/WorldView.h"
#include "items/Item.h"
#include "effects/Effect.h"
#include "units/Point.h" /// @todo This shouldn't be needed in the base class.

namespace questless
{
	class Agent
	{
	public:
		using ptr = std::unique_ptr<Agent>;

		Agent(Being& being) : _being{being} {}

		template <typename Derived>
		static ptr make(Being& being) { return std::make_unique<Derived>(being); }

		virtual ~Agent() = default;

		Agent& operator =(const Agent& other) = delete;

		Being& being() { return _being; }
		const Being& being() const { return _being; }

		/// Chooses and executes an action for the agent's being to perform.
		virtual void act() = 0;

		/// Causes the agent to perceive the given effect, possibly updating its state accordingly.
		/// @param effect The effect to perceive.
		virtual void perceive(const Effect::ptr& effect) = 0;

		// Queries and messages

		/// @todo Update documentation here after refactoring to decouple these methods from dialogs.

		virtual Action::Complete message
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete()> cont
			) const = 0;

		virtual Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;
		virtual Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;

		virtual Action::Complete query_duration
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;

		virtual Action::Complete query_magnitude
			( const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const = 0;

		/// @todo Add overload with min/max.

		virtual Action::Complete query_tile
			( const std::string& title
			, const std::string& prompt
			, boost::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const = 0;

		virtual Action::Complete query_being
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const = 0;

		virtual Action::Complete query_range
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;

		virtual Action::Complete query_item
			( const std::string& title
			, const std::string& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const = 0;

		virtual Action::Complete query_list
			( units::ScreenPoint origin /// @todo This should not be information required for all agents' query_list methods
			, std::string title
			, std::vector<std::string> options
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const = 0;

		// Quick Time Events

		/// @todo Documentation, implementation file.
		virtual Action::Complete get_lightning_bolt_quality(units::GamePoint, std::function<Action::Complete(double)> cont) { return cont(1.0); } /// @todo This should not be passed a GamePoint. Maybe the target's location?
	private:
		Being& _being;
	};
}
