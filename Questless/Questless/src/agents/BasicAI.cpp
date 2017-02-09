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
		switch (_state) {
			case State::move:
			{
				// Move to or turn towards a random neighbor hex, favoring movement.
				auto direction = uniform(0, 1) ? being.direction : static_cast<RegionTileCoords::Direction>(uniform(1, 6));
				walk(direction, [this](Action::Result result) {
					_state = State::idle;
					if (result == Action::Result::aborted) {
						// Walk failed. Wait for up to 10 time units instead.
						idle(uniform(0.0, 10.0));
					}
					return Action::Complete{};
				});
				break;
			}
			case State::idle:
				// Wait for up to 10 time units.
				idle(uniform(0.0, 10.0));
				_state = State::move;
				break;
		}
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

	Action::Complete BasicAI::query_magnitude
		( std::string const& //title
		, std::string const& //prompt
		, double //default
		, boost::optional<double> //min
		, boost::optional<double> //max
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
