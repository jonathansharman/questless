/**
* @file    Player.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Player class.
*/

#include "entities/beings/Player.h"
#include "Game.h"

using std::function;

namespace questless
{
	void Player::act(Game& game)
	{
		game.query_player_choice([this, &game](PlayerActionDialog::Choice player_choice) {
			switch (player_choice.type) {
				case PlayerActionDialog::Choice::Type::idle:
					_being.gain_busy_time(0.25);
					break;
				case PlayerActionDialog::Choice::Type::move:
					_being.gain_busy_time(1.0);
					game.region().move<Being>(_being, _being.coords().neighbor(static_cast<HexCoords::Direction>(player_choice.data)));
					break;
				case PlayerActionDialog::Choice::Type::use:
				{
					if (auto item_coords = game.hud().hotbar()[player_choice.data]) {
						Item* item = _being.inventory()[*item_coords];
						game.query_list<Action::ptr>
							( game.input().mouse_position()
							, item->name()
							, item->actions()
							, [](const Action::ptr& a) { return a->name(); }
							, [this, &game](optional<Action::ptr> opt_action) {
								if (!opt_action) {
									act(game);
									return;
								}
								Action::ptr action = std::move(*opt_action);
								action->perform(game, _being, [this, &game](Action::Result result) {
									if (result == Action::Result::aborted) {
										act(game);
										return;
									}
								});
							});
					}
					break;
				}
			}
		});
	}

	void Player::message
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, std::function<void()> cont
		) const
	{
		game.message(title, prompt, std::move(cont));
	}

	void Player::query_count
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, int default
		, optional<int> min
		, optional<int> max
		, std::function<void(optional<int>)> cont
		) const
	{
		game.query_count(title, prompt, default, min, max, std::move(cont));
	}
	void Player::query_count
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, int default
		, std::function<bool(int)> predicate
		, std::function<void(optional<int>)> cont
		) const
	{
		game.query_count(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void Player::query_duration
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		game.query_count(title, prompt, 1, 1, nullopt, std::move(cont));
	}

	void Player::query_magnitude
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, double default
		, std::function<bool(double)> predicate
		, std::function<void(optional<double>)> cont
		) const
	{
		game.query_magnitude(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void Player::query_tile
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, std::function<bool(HexCoords)> predicate
		, std::function<void(optional<HexCoords>)> cont
		) const
	{
		game.query_tile(title, prompt, std::move(predicate), std::move(cont));
	}

	void Player::query_being
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, std::function<bool(Being&)> predicate
		, std::function<void(optional<Being*>)> cont
		) const
	{
		game.query_being(title, prompt, std::move(predicate), std::move(cont));
	}

	void Player::query_range
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		game.query_count(title, prompt, 0, 0, nullopt, std::move(cont));
	}

	void Player::query_item
		( Game& game
		, const std::string& title
		, const std::string& prompt
		, const Being& source
		, std::function<bool(Being&)> predicate
		, std::function<void(optional<Item*>)> cont
		) const
	{
		game.query_item(title, prompt, source, std::move(predicate), std::move(cont));
	}
}
