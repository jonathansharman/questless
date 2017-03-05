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
#include "sdl/resources.h"
#include "ui/MessageDialog.h"
#include "ui/CountDialog.h"
#include "ui/MagnitudeDialog.h"
#include "ui/TileDialog.h"

#include "ui/qte/LightningBolt.h"

using std::function;

namespace questless
{
	void Player::act()
	{
		Game& game = being.game;
		game.query_player_choice([this, &game](PlayerActionDialog::Choice player_choice) {
			switch (player_choice.type) {
				case PlayerActionDialog::Choice::Type::idle:
					if (player_choice.data == 1) {
						idle([this](Action::Result result) {
							if (result == Action::Result::aborted) {
								// Chosen action aborted. Player must try to act again.
								act();
							}
							return Action::Complete{};
						});
					} else {
						idle(1.0);
					}
					break;
				case PlayerActionDialog::Choice::Type::walk:
				{
					auto direction = static_cast<RegionTileCoords::Direction>(player_choice.data);
					walk(direction, [this](Action::Result result) {
						if (result == Action::Result::aborted) {
							// Chosen action aborted. Player must try to act again.
							act();
						}
						return Action::Complete{};
					});
					break;
				}
				case PlayerActionDialog::Choice::Type::use:
				{
					if (auto item_coords = game.hud().hotbar()[player_choice.data]) {
						Item* item = being.inventory()[*item_coords];
						if (item != nullptr) { /// @todo Sync the hotbar with changes to the inventory so this is unnecessary.
							// Get a list of the item's actions. It's shared so the lambda that captures it is copyable, so the lambda can be passed as a std::function.
							auto actions = std::make_shared<std::vector<Action::ptr>>(item->actions());
							// Get the action names from the list of actions.
							std::vector<std::string> action_names;
							std::transform(actions->begin(), actions->end(), std::back_inserter(action_names), [](Action::ptr const& action) { return action->name(); });
							// Open list dialog for the player to choose an action.
							auto dialog = std::make_unique<ListDialog>(sdl::input().mouse_position(), item->name(), std::move(action_names),
								[this, &game, actions](boost::optional<int> opt_action_idx) {
									if (!opt_action_idx) {
										// No action selected. Player must try to act again.
										act();
										return Action::Complete{};
									} else {
										// Perform the chosen action.
										int action_idx = *opt_action_idx;
										return (*actions)[action_idx]->perform(being, [this, &game](Action::Result result) {
											if (result == Action::Result::aborted) {
												// Chosen action aborted. Player must try to act again.
												act();
											}
											return Action::Complete{};
										});
									}
								}
							);
							being.game.add_dialog(std::move(dialog));
						}
					}
					break;
				}
			}
		});
	}

	void Player::perceive(Effect::ptr const& effect)
	{
		_perceived_effects.push_back(effect);
	}

	std::vector<Effect::ptr> Player::poll_perceived_effects()
	{
		std::vector<Effect::ptr> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	Action::Complete Player::send_message
		( Message::ptr message
		, function<Action::Complete()> cont
		) const
	{
		struct MessageTitler : MessageVisitor
		{
			std::string title;
			void visit(MessageMeleeMiss const&) override { title = "Melee Attack"; }
			void visit(MessageNotEnoughMana const&) override { title = "Spell Cast"; }
			void visit(MessageSpellOutOfCharges const&) override { title = "Spell Cast"; }
			void visit(MessageSpellOnCooldown const&) override { title = "Spell Cast"; }
			void visit(MessageIncantFailedMute const&) override { title = "Incantation"; }
		};
		struct MessagePrompter : MessageVisitor
		{
			std::string prompt;
			void visit(MessageMeleeMiss const&) override { prompt = "Miss!"; }
			void visit(MessageNotEnoughMana const& m) override
			{
				prompt = "Not enough mana! You need " + std::to_string(m.mana_deficit) + " more mana to cast this.";
			}
			void visit(MessageSpellOutOfCharges const&) override { prompt = "This spell is out of charges! You need to incant it first."; }
			void visit(MessageSpellOnCooldown const& m) override
			{
				prompt = "This spell on cooldown! It will be ready in " + std::to_string(m.active_cooldown) + ".";
			}
			void visit(MessageIncantFailedMute const&) override { prompt = "You cannot perform an incantation while mute!"; }
		};

		MessageTitler titler;
		message->accept(titler);
		MessagePrompter prompter;
		message->accept(prompter);
		auto dialog = std::make_unique<MessageDialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return being.game.add_dialog(std::move(dialog));
	}

	Action::Complete Player::query_count
		( CountQuery::ptr query
		, int default
		, boost::optional<int> min
		, boost::optional<int> max
		, function<Action::Complete(boost::optional<int>)> cont
		) const
	{

		struct CountQueryTitler : CountQueryVisitor
		{
			std::string title;
		};
		struct CountQueryPrompter : CountQueryVisitor
		{
			std::string prompt;
		};

		CountQueryTitler titler;
		query->accept(titler);
		CountQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<CountDialog>(std::move(titler.title), std::move(prompter.prompt), default, min, max, std::move(cont));
		return being.game.add_dialog(std::move(dialog));
	}

	Action::Complete Player::query_magnitude
		( MagnitudeQuery::ptr query
		, double default
		, boost::optional<double> min
		, boost::optional<double> max
		, function<Action::Complete(boost::optional<double>)> cont
		) const
	{
		struct MagnitudeQueryTitler : MagnitudeQueryVisitor
		{
			std::string title;
			void visit(MagnitudeQueryWaitTime const&) override { title = "Wait"; }
			void visit(MagnitudeQueryLightningBolt const&) override { title = "Lightning Bolt Strength"; }
			void visit(MagnitudeQueryHeal const&) override { title = "Heal Amount"; }
		};
		struct MagnitudeQueryPrompter : MagnitudeQueryVisitor
		{
			std::string prompt;
			void visit(MagnitudeQueryWaitTime const&) override { prompt = "Enter wait time."; }
			void visit(MagnitudeQueryLightningBolt const&) override { prompt = "Choose how strong to make the lightning bolt."; }
			void visit(MagnitudeQueryHeal const&) override { prompt = "Choose how much health to restore."; }
		};

		MagnitudeQueryTitler titler;
		query->accept(titler);
		MagnitudeQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<MagnitudeDialog>(std::move(titler.title), std::move(prompter.prompt), default, min, max, std::move(cont));
		return being.game.add_dialog(std::move(dialog));
	}

	Action::Complete Player::query_tile
		( TileQuery::ptr query
		, boost::optional<RegionTileCoords> origin
		, function<bool(RegionTileCoords)> predicate
		, function<Action::Complete(boost::optional<RegionTileCoords>)> cont
		) const
	{
		struct TileQueryTitler : TileQueryVisitor
		{
			std::string title;
			void visit(TileQueryMeleeTarget const&) override { title = "Melee Attack"; }
			void visit(TileQueryLightningBoltTarget const&) override { title = "Lightning Bolt Target"; }
			void visit(TileQueryTeleportTarget const&) override { title = "Teleport Target"; }
		};
		struct TileQueryPrompter : TileQueryVisitor
		{
			std::string prompt;
			void visit(TileQueryMeleeTarget const&) override { prompt = "Choose attack target."; }
			void visit(TileQueryLightningBoltTarget const&) override { prompt = "Select a tile to be zapped with a lightning bolt."; }
			void visit(TileQueryTeleportTarget const&) override { prompt = "Select a tile to teleport to."; }
		};

		TileQueryTitler titler;
		query->accept(titler);
		TileQueryPrompter prompter;
		query->accept(prompter);
		Game& game = being.game;
		auto dialog = std::make_unique<TileDialog>(std::move(titler.title), std::move(prompter.prompt), game.camera(), std::move(origin), std::move(predicate), std::move(cont));
		return game.add_dialog(std::move(dialog));
	}

	Action::Complete Player::query_being
		( BeingQuery::ptr //query
		, function<bool(Being&)> //predicate
		, function<Action::Complete(boost::optional<Being*>)> cont
		) const
	{
		// spell::Heal: "Heal Target", "Select a being to be healed."
		/// @todo This.
		return cont(boost::none);
	}

	Action::Complete Player::query_item
		( ItemQuery::ptr //query
		, Being& //source
		, function<bool(Being&)> //predicate
		, function<Action::Complete(boost::optional<Item*>)> cont
		) const
	{
		/// @todo This.
		return cont(boost::none);
	}

	// Quick Time Events

	Action::Complete Player::get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Action::Complete(double)> cont) const
	{
		auto dialog = std::make_unique<qte::LightningBolt>(being.game.camera(), target_coords, std::move(cont));
		return being.game.add_dialog(std::move(dialog));
	}
}
