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
#include "ui/MessageDialog.h"
#include "ui/CountDialog.h"
#include "ui/MagnitudeDialog.h"
#include "ui/TileDialog.h"

#include "ui/qte/LightningBolt.h"

using std::function;
using std::move;

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
							// Get a list of the item's actions. It's shared so the lambda that captures it is copyable, so the lambda can be passed as a std::function.
							auto actions = std::make_shared<std::vector<Action::ptr>>(item->actions());
							// Get the action names from the list of actions.
							std::vector<std::string> action_names;
							std::transform(actions->begin(), actions->end(), std::back_inserter(action_names), [](const Action::ptr& action) { return action->name(); });

							query_list
								( game.input().mouse_position()
								, item->name()
								, action_names
								, [this, &game, actions](boost::optional<int> opt_action_idx) {
									if (!opt_action_idx) {
										// No action selected. Player must try to act again.
										act();
										return Action::Complete{};
									} else {
										// Perform the chosen action.
										int action_idx = *opt_action_idx;
										return (*actions)[action_idx]->perform(being(), [this, &game](Action::Result result) {
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
		std::vector<Effect::ptr> perceived_effects = move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	Action::Complete Player::message
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete()> cont
		) const
	{
		auto dialog = std::make_unique<MessageDialog>(move(title), move(prompt), move(cont));
		return being().game().add_dialog(move(dialog));
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

		auto dialog = std::make_unique<CountDialog>(move(title), move(prompt), default, min, max, [](int) { return true; }, std::move(cont));
		return being().game().add_dialog(move(dialog));
	}
	Action::Complete Player::query_count
		( const std::string& title
		, const std::string& prompt
		, int default
		, function<bool(int)> predicate
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		auto dialog = std::make_unique<CountDialog>(move(title), move(prompt), default, boost::none, boost::none, move(predicate), std::move(cont));
		return being().game().add_dialog(move(dialog));
	}

	Action::Complete Player::query_duration
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return query_count(title, prompt, 1, 1, boost::none, std::move(cont));
	}

	Action::Complete Player::query_magnitude
		( const std::string& title
		, const std::string& prompt
		, double default
		, function<bool(double)> predicate
		, function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		auto dialog = std::make_unique<MagnitudeDialog>(move(title), move(prompt), default, boost::none, boost::none, move(predicate), std::move(cont));
		return being().game().add_dialog(move(dialog));
	}

	Action::Complete Player::query_tile
		( const std::string& title
		, const std::string& prompt
		, boost::optional<RegionTileCoords> origin
		, function<bool(RegionTileCoords)> predicate
		, function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		Game& game = being().game();
		auto dialog = std::make_unique<TileDialog>(move(title), move(prompt), game.camera(), std::move(origin), move(predicate), std::move(cont));
		return game.add_dialog(move(dialog));
	}

	Action::Complete Player::query_being
		( const std::string& //title
		, const std::string& //prompt
		, function<bool(Being&)> //predicate
		, function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		/// @todo This.
		return cont(boost::none);
	}

	Action::Complete Player::query_range
		( const std::string& title
		, const std::string& prompt
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		return query_count(title, prompt, 0, 0, boost::none, std::move(cont));
	}

	Action::Complete Player::query_item
		( const std::string& //title
		, const std::string& //prompt
		, Being& //source
		, function<bool(Being&)> //predicate
		, function<Action::Complete(boost::optional<Item*>)> cont
		) const
	{
		/// @todo This.
		return cont(boost::none);
	}

	Action::Complete Player::query_list
		( ScreenPoint origin
		, std::string title
		, std::vector<std::string> options
		, std::function<Action::Complete(boost::optional<int>)> cont
		) const
	{
		auto dialog = std::make_unique<ListDialog>(origin, move(title), move(options), std::move(cont));
		return being().game().add_dialog(move(dialog));
	}

	// Quick Time Events

	Action::Complete Player::get_lightning_bolt_quality(GamePoint target, std::function<Action::Complete(double)> cont)
	{
		auto dialog = std::make_unique<qte::LightningBolt>(being().game().camera(), target, move(cont));
		return being().game().add_dialog(move(dialog));
	}
}
