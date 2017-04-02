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
#include "ui/DirectionDialog.h"

#include "ui/qte/LightningBolt.h"

using std::function;

namespace questless
{
	void Player::act()
	{
		game().query_player_choice([this](PlayerActionDialog::Choice player_choice) {
			switch (player_choice.type) {
				case PlayerActionDialog::Choice::Type::idle:
					if (player_choice.data == 1) {
						idle([this](Action::Result result) {
							if (result == Action::Result::aborted) {
								// Chosen action aborted. Player must try to act again.
								act();
							}
							return Complete{};
						});
					} else {
						idle(1.0);
					}
					break;
				case PlayerActionDialog::Choice::Type::move:
				{
					// Turn towards the chosen direction or move in that direction if already facing that way.
					auto direction = static_cast<RegionTileCoords::Direction>(player_choice.data);
					if (being.direction == direction) {
						walk(direction, [this](Action::Result result) {
							if (result == Action::Result::aborted) {
								// Chosen action aborted. Player must try to act again.
								act();
							}
							return Complete{};
						});
					} else {
						turn(direction, [this](Action::Result result) {
							if (result == Action::Result::aborted) {
								// Chosen action aborted. Player must try to act again.
								act();
							}
							return Complete{};
						});
					}
					break;
				}
				case PlayerActionDialog::Choice::Type::shift_move:
				{
					// Strafe.
					auto direction = static_cast<RegionTileCoords::Direction>(player_choice.data);
					walk(direction, [this](Action::Result result) {
						if (result == Action::Result::aborted) {
							// Chosen action aborted. Player must try to act again.
							act();
						}
						return Complete{};
					});
					break;
				}
				case PlayerActionDialog::Choice::Type::use:
				{
					if (auto item_coords = game().hud().hotbar()[player_choice.data]) {
						Item* item = being.inventory[*item_coords];
						if (item != nullptr) { /// @todo Sync the hotbar with changes to the inventory so this is unnecessary.
							// Get a list of the item's actions. It's shared so the lambda that captures it is copyable, so the lambda can be passed as a std::function.
							auto actions = std::make_shared<std::vector<Action::uptr>>(item->actions());
							// Get the action names from the list of actions.
							std::vector<std::string> action_names;
							std::transform(actions->begin(), actions->end(), std::back_inserter(action_names), [](Action::uptr const& action) { return action->name(); });
							// Open list dialog for the player to choose an action.
							auto dialog = std::make_unique<ListDialog>(sdl::input().mouse_position(), item->name(), std::move(action_names),
								[this, actions](std::optional<int> opt_action_idx) {
									if (!opt_action_idx) {
										// No action selected. Player must try to act again.
										act();
										return Complete{};
									} else {
										// Perform the chosen action.
										int action_idx = *opt_action_idx;
										return (*actions)[action_idx]->perform(being, [this](Action::Result result) {
											if (result == Action::Result::aborted) {
												// Chosen action aborted. Player must try to act again.
												act();
											}
											return Complete{};
										});
									}
								}
							);
							game().add_dialog(std::move(dialog));
						}
					}
					break;
				}
			}
		});
	}

	void Player::perceive(Effect::uptr const& effect)
	{
		_perceived_effects.push_back(effect);
	}

	std::vector<Effect::uptr> Player::poll_perceived_effects()
	{
		std::vector<Effect::uptr> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	Complete Player::send_message
		( Message::uptr message
		, function<Complete()> cont
		) const
	{
		struct MessageTitler : MessageVisitor
		{
			std::string title;
			void visit(MessageArrowMiss const&) override { title = "Ranged Attack"; }
			void visit(MessageEntityInTheWay const&) override { title = "Obstruction"; }
			void visit(MessageIncantFailedMute const&) override { title = "Incantation"; }
			void visit(MessageMeleeMiss const&) override { title = "Melee Attack"; }
			void visit(MessageNotEnoughAmmo const&) override { title = "Attack"; }
			void visit(MessageNotEnoughMana const&) override { title = "Spell Cast"; }
			void visit(MessageSpellNotEnoughCharges const&) override { title = "Spell Cast"; }
			void visit(MessageSpellOnCooldown const&) override { title = "Spell Cast"; }
		};
		struct MessagePrompter : MessageVisitor
		{
			std::string prompt;
			void visit(MessageArrowMiss const&) override { prompt = "Miss!"; }
			void visit(MessageEntityInTheWay const&) override { prompt = "There's something in the way!"; }
			void visit(MessageIncantFailedMute const&) override { prompt = "You cannot perform an incantation while mute!"; }
			void visit(MessageMeleeMiss const&) override { prompt = "Miss!"; }
			void visit(MessageNotEnoughAmmo const&) override { prompt = "Not enough ammo!"; }
			void visit(MessageNotEnoughMana const& m) override
			{
				prompt = "Not enough mana! You need " + std::to_string(m.mana_deficit) + " more mana to cast this.";
			}
			void visit(MessageSpellNotEnoughCharges const&) override { prompt = "This spell doesn't have enough charges! You need to incant it first."; }
			void visit(MessageSpellOnCooldown const& m) override
			{
				prompt = "This spell on cooldown! It will be ready in " + std::to_string(m.active_cooldown) + ".";
			}
		};

		MessageTitler titler;
		message->accept(titler);
		MessagePrompter prompter;
		message->accept(prompter);
		auto dialog = std::make_unique<MessageDialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_count
		( CountQuery::uptr query
		, int default
		, std::optional<int> min
		, std::optional<int> max
		, function<Complete(std::optional<int>)> cont
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
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_magnitude
		( MagnitudeQuery::uptr query
		, double default
		, std::optional<double> min
		, std::optional<double> max
		, function<Complete(std::optional<double>)> cont
		) const
	{
		struct MagnitudeQueryTitler : MagnitudeQueryVisitor
		{
			std::string title;
			void visit(MagnitudeQueryHeal const&) override { title = "Heal Amount"; }
			void visit(MagnitudeQueryLightningBolt const&) override { title = "Lightning Bolt Strength"; }
			void visit(MagnitudeQueryWaitTime const&) override { title = "Wait"; }
		};
		struct MagnitudeQueryPrompter : MagnitudeQueryVisitor
		{
			std::string prompt;
			void visit(MagnitudeQueryHeal const&) override { prompt = "Choose how much health to restore."; }
			void visit(MagnitudeQueryLightningBolt const&) override { prompt = "Choose how strong to make the lightning bolt."; }
			void visit(MagnitudeQueryWaitTime const&) override { prompt = "Enter wait time."; }
		};

		MagnitudeQueryTitler titler;
		query->accept(titler);
		MagnitudeQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<MagnitudeDialog>(std::move(titler.title), std::move(prompter.prompt), default, min, max, std::move(cont));
		return game().add_dialog(std::move(dialog));
	}
	
	Complete Player::query_tile
		( TileQuery::uptr query
		, std::optional<RegionTileCoords> origin
		, function<bool(RegionTileCoords)> predicate
		, function<Complete(std::optional<RegionTileCoords>)> cont
		) const
	{
		struct TileQueryTitler : TileQueryVisitor
		{
			std::string title;
			void visit(TileQueryLightningBoltTarget const&) override { title = "Lightning Bolt Target"; }
			void visit(TileQueryRangedAttackTarget const&) override { title = "Ranged Attack"; }
			void visit(TileQueryTeleportTarget const&) override { title = "Teleport Target"; }
		};
		struct TileQueryPrompter : TileQueryVisitor
		{
			std::string prompt;
			void visit(TileQueryLightningBoltTarget const&) override { prompt = "Select a tile to be zapped with a lightning bolt."; }
			void visit(TileQueryRangedAttackTarget const&) override { prompt = "Choose attack target."; }
			void visit(TileQueryTeleportTarget const&) override { prompt = "Select a tile to teleport to."; }
		};

		TileQueryTitler titler;
		query->accept(titler);
		TileQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<TileDialog>
			( std::move(titler.title)
			, std::move(prompter.prompt)
			, std::move(origin)
			, std::move(predicate)
			, std::move(cont)
			);
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_direction
		( DirectionQuery::uptr query
		, function<Complete(std::optional<RegionTileCoords::Direction>)> cont
		) const
	{
		struct DirectionQueryTitler : DirectionQueryVisitor
		{
			std::string title;
			void visit(DirectionQueryMeleeAttack const&) override { title = "Melee Attack"; }
		};
		struct DirectionQueryPrompter : DirectionQueryVisitor
		{
			std::string prompt;
			void visit(DirectionQueryMeleeAttack const&) override { prompt = "Choose attack direction."; }
		};

		DirectionQueryTitler titler;
		query->accept(titler);
		DirectionQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<DirectionDialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_being
		( BeingQuery::uptr //query
		, function<bool(Being&)> //predicate
		, function<Complete(std::optional<Being*>)> cont
		) const
	{
		// spell::Heal: "Heal Target", "Select a being to be healed."
		/// @todo This.
		return cont(std::nullopt);
	}

	Complete Player::query_item
		( ItemQuery::uptr //query
		, Being& //source
		, function<bool(Being&)> //predicate
		, function<Complete(std::optional<Item*>)> cont
		) const
	{
		/// @todo This.
		return cont(std::nullopt);
	}

	// Quick Time Events

	Complete Player::get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Complete(double)> cont) const
	{
		auto dialog = std::make_unique<qte::LightningBolt>(target_coords, std::move(cont));
		return game().add_dialog(std::move(dialog));
	}
}
