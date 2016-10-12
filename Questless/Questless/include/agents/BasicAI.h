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

		Action::Complete message
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete()> cont
			) const override;

		Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;
		Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_duration
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_magnitude
			( const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override;

		Action::Complete query_tile
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override;

		Action::Complete query_being
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override;

		Action::Complete query_range
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_item
			( const std::string& title
			, const std::string& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const override;
	};
}

#endif
