/**
* @file    Player.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Player class, which is the agent responsible for the player's being.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Agent.h"

namespace questless
{
	class Player : public Agent
	{
	public:
		Player(Being& being) : Agent{being} {}

		void act(Game& game) override;

		// Queries and messages

		void message
			(Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void()> cont
			) const override;

		void query_count
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, int default
			, optional<int> min
			, optional<int> max
			, std::function<void(optional<int>)> cont
			) const override;
		void query_count
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<void(optional<int>)> cont
			) const override;

		void query_duration
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const override;

		void query_magnitude
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<void(optional<double>)> cont
			) const override;

		void query_tile
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<bool(HexCoords)> predicate
			, std::function<void(optional<HexCoords>)> cont
			) const override;

		void query_being
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Being*>)> cont
			) const override;

		void query_range
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, std::function<void(optional<int>)> cont
			) const override;

		void query_item
			( Game& game
			, const std::string& title
			, const std::string& prompt
			, const Being& source
			, std::function<bool(Being&)> predicate
			, std::function<void(optional<Item*>)> cont
			) const override;
	};
}

#endif
