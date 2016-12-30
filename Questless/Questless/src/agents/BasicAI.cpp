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
		// To to move to a random neighbor hex.
		RegionTileCoords neighbor_hex;
		switch (uniform(0, 5)) {
			case 0:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::one);
				break;
			case 1:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::two);
				break;
			case 2:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::three);
				break;
			case 3:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::four);
				break;
			case 4:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::five);
				break;
			case 5:
				neighbor_hex = being().coords().neighbor(RegionTileCoords::Direction::six);
				break;
		}
		being().region().move(being(), neighbor_hex);

		// Wait 1 time unit for the movement, then a random additional idle time, up to 10 time units.
		being().gain_busy_time(uniform(1.0, 11.0));
	}

	Action::Complete BasicAI::message
		( const std::string& //title
		, const std::string& //prompt
		, std::function<Action::Complete()> cont
		) const
	{
		return cont();
	}

	Action::Complete BasicAI::query_count
		( const std::string& //title
		, const std::string& //prompt
		, int //default
		, boost::optional<int> //min
		, boost::optional<int> //max
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}
	Action::Complete BasicAI::query_count
		( const std::string& //title
		, const std::string& //prompt
		, int //default
		, std::function<bool(int)> //predicate
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_duration
		( const std::string& //title
		, const std::string& //prompt
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_magnitude
		( const std::string& //title
		, const std::string& //prompt
		, double //default
		, std::function<bool(double)> //predicate
		, std::function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_tile
		( const std::string& //title
		, const std::string& //prompt
		, boost::optional<RegionTileCoords> //origin
		, std::function<bool(RegionTileCoords)> //predicate
		, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_being
		( const std::string& //title
		, const std::string& //prompt
		, std::function<bool(Being&)> //predicate
		, std::function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_range
		( const std::string& //title
		, const std::string& //prompt
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return cont(boost::none);
	}

	Action::Complete BasicAI::query_item
		( const std::string& //title
		, const std::string& //prompt
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
