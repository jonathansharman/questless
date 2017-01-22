/**
* @file    BasicAI.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BasicAI class.
*/

#include "agents/BasicAI.h"
#include "Game.h"

using std::function;

namespace questless
{
	void BasicAI::act()
	{
		// Move to or turn towards a random neighbor hex, favoring movement.
		auto direction = uniform(0, 1) ? being().direction : static_cast<RegionTileCoords::Direction>(uniform(1, 6));
		if (direction == being().direction) {
			being().busy_time += 1.0; /// @todo Calculate move time from stats and terrain.
			being().region().move(being(), being().coords().neighbor(direction));
		} else {
			/// @todo Calculate turn time from stats and terrain.
			being().busy_time += 0.05 + 0.05 * RegionTileCoords::distance(being().direction, direction);
			being().direction = direction;
		}

		// Wait for a random additional idle time, up to 10 time units.
		being().busy_time += uniform(0.0, 10.0);
	}

	Action::Complete BasicAI::message
		( std::string const& //title
		, std::string const& //prompt
		, std::function<Action::Complete()> cont
		) const
	{
		return cont();
	}

	Action::Complete BasicAI::query_count
		( std::string const& //title
		, std::string const& //prompt
		, int //default
		, boost::optional<int> //min
		, boost::optional<int> //max
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}
	Action::Complete BasicAI::query_count
		( std::string const& //title
		, std::string const& //prompt
		, int //default
		, std::function<bool(int)> //predicate
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_duration
		( std::string const& //title
		, std::string const& //prompt
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_magnitude
		( std::string const& //title
		, std::string const& //prompt
		, double //default
		, std::function<bool(double)> //predicate
		, std::function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_tile
		( std::string const& //title
		, std::string const& //prompt
		, boost::optional<RegionTileCoords> //origin
		, std::function<bool(RegionTileCoords)> //predicate
		, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_being
		( std::string const& //title
		, std::string const& //prompt
		, std::function<bool(Being&)> //predicate
		, std::function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_range
		( std::string const& //title
		, std::string const& //prompt
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_item
		( std::string const& //title
		, std::string const& //prompt
		, Being& //source
		, std::function<bool(Being&)> //predicate
		, std::function<Action::Complete(boost::optional<Item*>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_list
		( units::ScreenPoint //origin
		, std::string //title
		, std::vector<std::string> //options
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}
}
