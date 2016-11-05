/**
* @file    Player.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Player class.
*/

#include "agents/Player.h"
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
					being().gain_busy_time(player_choice.data == 1 ? 10.0 : 0.25);
					break;
				case PlayerActionDialog::Choice::Type::move:
					being().gain_busy_time(1.0);
					game.region().move(being(), RegionTileCoords{being().coords().neighbor(static_cast<RegionTileCoords::Direction>(player_choice.data))});
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
								, [this, &game](boost::optional<Action::ptr> opt_action) {
									if (!opt_action) {
										// No action selected. Player must try to act again.
										act();
										return Action::Complete{};
									} else {
										// Perform the chosen action.
										Action::ptr action = std::move(*opt_action);
										return action->perform(being(), [this, &game](Action::Result result) {
											if (result == Action::Result::aborted) {
												// Chosen action aborted. Player must try to act again.
												act();
											}
											return Action::Complete{};
										});
									}
								});
						}
					}
					break;
				}
			}
		});
	}

	void Player::perceive(const Effect::ptr& effect)
	{
		_perceived_effects.push_back(effect);
	}

	std::vector<Effect::ptr> Player::poll_perceived_effects()
	{
		std::vector<Effect::ptr> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	Action::Complete Player::message
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete()> cont
		) const
	{
		return being().game().message(title, prompt, std::move(cont));
	}

	Action::Complete Player::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, boost::optional<int> min
		, boost::optional<int> max
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return being().game().query_count(title, prompt, default, min, max, std::move(cont));
	}
	Action::Complete Player::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, function<bool(int)> predicate
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return being().game().query_count(title, prompt, default, std::move(predicate), std::move(cont));
	}

	Action::Complete Player::query_duration
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return being().game().query_count(title, prompt, 1, 1, boost::none, std::move(cont));
	}

	Action::Complete Player::query_magnitude
		( const std::string& title
		, const std::string& prompt
		, double default
		, function<bool(double)> predicate
		, function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		return being().game().query_magnitude(title, prompt, default, std::move(predicate), std::move(cont));
	}

	Action::Complete Player::query_tile
		( const std::string& title
		, const std::string& prompt
		, function<bool(RegionTileCoords)> predicate
		, function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		return being().game().query_tile(title, prompt, std::move(predicate), std::move(cont));
	}

	Action::Complete Player::query_being
		( const std::string& title
		, const std::string& prompt
		, function<bool(Being&)> predicate
		, function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		return being().game().query_being(title, prompt, std::move(predicate), std::move(cont));
	}

	Action::Complete Player::query_range
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return being().game().query_count(title, prompt, 0, 0, boost::none, std::move(cont));
	}

	Action::Complete Player::query_item
		( const std::string& title
		, const std::string& prompt
		, Being& source
		, function<bool(Being&)> predicate
		, function<Action::Complete(boost::optional<Item*>)> cont
		) const
	{
		return being().game().query_item(title, prompt, source, std::move(predicate), std::move(cont));
	}
}
