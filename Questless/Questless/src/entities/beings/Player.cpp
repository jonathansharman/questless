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
	void Player::act()
	{
		Game& game = being().game();
		game.query_player_choice([this, &game](PlayerActionDialog::Choice player_choice) {
			switch (player_choice.type) {
				case PlayerActionDialog::Choice::Type::idle:
					being().gain_busy_time(0.25);
					break;
				case PlayerActionDialog::Choice::Type::move:
					being().gain_busy_time(1.0);
					game.region().move(being(), RegionTileCoords{being().coords().hex.neighbor(static_cast<HexCoords::Direction>(player_choice.data))});
					break;
				case PlayerActionDialog::Choice::Type::use:
				{
					if (auto item_coords = game.hud().hotbar()[player_choice.data]) {
						Item* item = being().inventory()[*item_coords];
						if (item != nullptr) { /// @todo Sync the hotbar with changes to the inventory so this is unnecessary.
							game.query_list<Action::ptr>
								( game.input().mouse_position()
								, item->name()
								, item->actions()
								, [](const Action::ptr& a) { return a->name(); }
								, [this, &game](optional<Action::ptr> opt_action) {
									if (!opt_action) {
										act();
										return;
									}
									Action::ptr action = std::move(*opt_action);
									action->perform(being(), [this, &game](Action::Result result) {
										if (result == Action::Result::aborted) {
											act();
											return;
										}
									});
								});
						}
					}
					break;
				}
			}
		});
	}

	void Player::message
		( const std::string& title
		, const std::string& prompt
		, std::function<void()> cont
		) const
	{
		being().game().message(title, prompt, std::move(cont));
	}

	void Player::query_count
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
	void Player::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, std::function<bool(int)> predicate
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void Player::query_duration
		( const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, 1, 1, nullopt, std::move(cont));
	}

	void Player::query_magnitude
		( const std::string& title
		, const std::string& prompt
		, double default
		, std::function<bool(double)> predicate
		, std::function<void(optional<double>)> cont
		) const
	{
		being().game().query_magnitude(title, prompt, default, std::move(predicate), std::move(cont));
	}

	void Player::query_tile
		( const std::string& title
		, const std::string& prompt
		, std::function<bool(RegionTileCoords)> predicate
		, std::function<void(optional<RegionTileCoords>)> cont
		) const
	{
		being().game().query_tile(title, prompt, std::move(predicate), std::move(cont));
	}

	void Player::query_being
		( const std::string& title
		, const std::string& prompt
		, std::function<bool(Being&)> predicate
		, std::function<void(optional<Being*>)> cont
		) const
	{
		being().game().query_being(title, prompt, std::move(predicate), std::move(cont));
	}

	void Player::query_range
		( const std::string& title
		, const std::string& prompt
		, std::function<void(optional<int>)> cont
		) const
	{
		being().game().query_count(title, prompt, 0, 0, nullopt, std::move(cont));
	}

	void Player::query_item
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
