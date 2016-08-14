/**
* @file    BasicAI.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BasicAI class, an AI suitable for most NPCs.
*/

#ifndef BASIC_AI_H
#define BASIC_AI_H

#include "Agent.h"

namespace questless
{
	class BasicAI : public Agent
	{
	public:
		BasicAI(Being& being) : Agent{being} {}

		void act() override;

		// Queries and messages

		void message
			( const std::string& title
			, const std::string& prompt
			, std::function<void()> cont
			) const override;

		void query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, optional<int> min
			, optional<int> max
			, std::function<void(optional<int>)> cont
			) const override;
		void query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<void(optional<int>)> cont
			) const override;

		void query_duration
			( const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const override;

		void query_magnitude
			( const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<void(optional<double>)> cont
			) const override;

		void query_tile
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<void(optional<RegionTileCoords>)> cont
			) const override;

		void query_being
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Being*>)> cont
			) const override;

		void query_range
			( const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const override;

		void query_item
			( const std::string& title
			, const std::string& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Item*>)> cont
			) const override;
	};
}

#endif
