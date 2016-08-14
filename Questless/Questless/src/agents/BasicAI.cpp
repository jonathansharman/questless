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
		HexCoords neighbor_hex;
		switch (uniform(0, 5)) {
			case 0:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::one);
				break;
			case 1:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::two);
				break;
			case 2:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::three);
				break;
			case 3:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::four);
				break;
			case 4:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::five);
				break;
			case 5:
				neighbor_hex = being().coords().hex.neighbor(HexCoords::Direction::six);
				break;
		}
		being().region().move(being(), RegionTileCoords{neighbor_hex});

		// Wait 1 time unit for the movement, then a random additional idle time, up to 10 time units.
		being().gain_busy_time(uniform(1.0, 11.0));
	}

	void BasicAI::message
		( const std::string& title
		, const std::string& prompt
		, std::function<void()> cont
		) const
	{
		being().game().message(title, prompt, std::move(cont));
	}

	void BasicAI::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, optional<int> min
		, optional<int> max
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, default, min, max, std::move(cont));
	}
	void BasicAI::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, std::function<bool(int)> predicate
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void BasicAI::query_duration
		( const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, 1, 1, nullopt, std::move(cont));
	}

	void BasicAI::query_magnitude
		( const std::string& title
		, const std::string& prompt
		, double default
		, std::function<bool(double)> predicate
		, std::function<void(optional<double>)> cont
		) const
	{
		being().game().query_magnitude(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void BasicAI::query_tile
		( const std::string& title
		, const std::string& prompt
		, std::function<bool(RegionTileCoords)> predicate
		, std::function<void(optional<RegionTileCoords>)> cont
		) const
	{
		being().game().query_tile(title, prompt, std::move(predicate), std::move(cont));
	}

	void BasicAI::query_being
		( const std::string& title
		, const std::string& prompt
		, std::function<bool(Being&)> predicate
		, std::function<void(optional<Being*>)> cont
		) const
	{
		being().game().query_being(title, prompt, std::move(predicate), std::move(cont));
	}

	void BasicAI::query_range
		( const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, 0, 0, nullopt, std::move(cont));
	}

	void BasicAI::query_item
		( const std::string& title
		, const std::string& prompt
		, Being& source
		, std::function<bool(Being&)> predicate
		, std::function<void(optional<Item*>)> cont
		) const
	{
		being().game().query_item(title, prompt, source, std::move(predicate), std::move(cont));
	}
}
