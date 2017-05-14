//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/Player.h"
#include "Game.h"
#include "sdl/resources.h"
#include "ui/CountDialog.h"
#include "ui/DirectionDialog.h"
#include "ui/MagnitudeDialog.h"
#include "ui/MessageDialog.h"
#include "ui/TileDialog.h"
#include "ui/VectorDialog.h"

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
					auto direction = static_cast<RegionTile::Direction>(player_choice.data);
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
					auto direction = static_cast<RegionTile::Direction>(player_choice.data);
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
					//! @todo Sync the hotbar with changes to the inventory.
					if (std::optional<Id<Item>> opt_item_id = game().hud().hotbar()[player_choice.data]) {
						Item& item = game().items.get_ref(*opt_item_id);
						// Get a list of the item's actions. It's shared so the lambda that captures it is copyable, so the lambda can be passed as a std::function.
						auto actions = std::make_shared<std::vector<uptr<Action>>>(item.actions());
						// Get the action names from the list of actions.
						std::vector<std::string> action_names;
						std::transform(actions->begin(), actions->end(), std::back_inserter(action_names), [](uptr<Action> const& action) { return action->name(); });
						// Open list dialog for the player to choose an action.
						auto dialog = std::make_unique<ListDialog>(sdl::input().mouse_position(), item.name(), std::move(action_names),
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
					break;
				}
			}
		});
	}

	void Player::perceive(sptr<Effect> const& effect)
	{
		_perceived_effects.push_back(effect);
	}

	std::vector<sptr<Effect>> Player::poll_perceived_effects()
	{
		std::vector<sptr<Effect>> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	Complete Player::send_message
		( uptr<Message> message
		, function<Complete()> cont
		) const
	{
		struct MessageTitler : MessageConstVisitor
		{
			std::string title;
			void visit(MessageArrowMiss const&) final { title = "Ranged Attack"; }
			void visit(MessageCannotEquip const&) final { title = "Cannot Equip"; }
			void visit(MessageEntityInTheWay const&) final { title = "Obstruction"; }
			void visit(MessageIncantFailedMute const&) final { title = "Incantation"; }
			void visit(MessageMeleeMiss const&) final { title = "Melee Attack"; }
			void visit(MessageNotEnoughAmmo const&) final { title = "Attack"; }
			void visit(MessageNotEnoughMana const&) final { title = "Spell Cast"; }
			void visit(MessageSpellNotEnoughCharges const&) final { title = "Spell Cast"; }
			void visit(MessageSpellOnCooldown const&) final { title = "Spell Cast"; }
		};
		struct MessagePrompter : MessageConstVisitor
		{
			std::string prompt;
			void visit(MessageArrowMiss const&) final { prompt = "Miss!"; }
			void visit(MessageCannotEquip const&) final { prompt = "You don't have the requisite free body parts to equip this."; }
			void visit(MessageEntityInTheWay const&) final { prompt = "There's something in the way!"; }
			void visit(MessageIncantFailedMute const&) final { prompt = "You can't perform an incantation while mute!"; }
			void visit(MessageMeleeMiss const&) final { prompt = "Miss!"; }
			void visit(MessageNotEnoughAmmo const&) final { prompt = "Not enough ammo!"; }
			void visit(MessageNotEnoughMana const& m) final
			{
				prompt = "Not enough mana! You need " + std::to_string(m.mana_deficit) + " more mana to cast this.";
			}
			void visit(MessageSpellNotEnoughCharges const&) final { prompt = "This spell doesn't have enough charges! You need to incant it first."; }
			void visit(MessageSpellOnCooldown const& m) final
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
		( uptr<CountQuery> query
		, int default
		, std::optional<int> min
		, std::optional<int> max
		, function<Complete(std::optional<int>)> cont
		) const
	{
		struct CountQueryTitler : CountQueryConstVisitor
		{
			std::string title;
		};
		struct CountQueryPrompter : CountQueryConstVisitor
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
		( uptr<MagnitudeQuery> query
		, double default
		, std::optional<double> min
		, std::optional<double> max
		, function<Complete(std::optional<double>)> cont
		) const
	{
		struct MagnitudeQueryTitler : MagnitudeQueryConstVisitor
		{
			std::string title;
			void visit(MagnitudeQueryHeal const&) final { title = "Heal Amount"; }
			void visit(MagnitudeQueryLightningBolt const&) final { title = "Lightning Bolt Strength"; }
			void visit(MagnitudeQueryWaitTime const&) final { title = "Wait"; }
		};
		struct MagnitudeQueryPrompter : MagnitudeQueryConstVisitor
		{
			std::string prompt;
			void visit(MagnitudeQueryHeal const&) final { prompt = "Choose how much health to restore."; }
			void visit(MagnitudeQueryLightningBolt const&) final { prompt = "Choose how strong to make the lightning bolt."; }
			void visit(MagnitudeQueryWaitTime const&) final { prompt = "Enter wait time."; }
		};

		MagnitudeQueryTitler titler;
		query->accept(titler);
		MagnitudeQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<MagnitudeDialog>(std::move(titler.title), std::move(prompter.prompt), default, min, max, std::move(cont));
		return game().add_dialog(std::move(dialog));
	}
	
	Complete Player::query_tile
		( uptr<TileQuery> query
		, std::optional<RegionTile::Point> origin
		, function<bool(RegionTile::Point)> predicate
		, function<Complete(std::optional<RegionTile::Point>)> cont
		) const
	{
		struct TileQueryTitler : TileQueryConstVisitor
		{
			std::string title;
			void visit(TileQueryLightningBoltTarget const&) final { title = "Lightning Bolt Target"; }
			void visit(TileQueryRangedAttackTarget const&) final { title = "Ranged Attack"; }
			void visit(TileQueryTeleportTarget const&) final { title = "Teleport Target"; }
		};
		struct TileQueryPrompter : TileQueryConstVisitor
		{
			std::string prompt;
			void visit(TileQueryLightningBoltTarget const&) final { prompt = "Select a tile to be zapped with a lightning bolt."; }
			void visit(TileQueryRangedAttackTarget const&) final { prompt = "Choose attack target."; }
			void visit(TileQueryTeleportTarget const&) final { prompt = "Select a tile to teleport to."; }
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
		( uptr<DirectionQuery> query
		, function<Complete(std::optional<RegionTile::Direction>)> cont
		) const
	{
		struct DirectionQueryTitler : DirectionQueryConstVisitor
		{
			std::string title;
		};
		struct DirectionQueryPrompter : DirectionQueryConstVisitor
		{
			std::string prompt;
		};

		DirectionQueryTitler titler;
		query->accept(titler);
		DirectionQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<DirectionDialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_vector
		( uptr<VectorQuery> query
		, std::optional<RegionTile::Point> origin
		, std::function<bool(RegionTile::Vector)> predicate
		, std::function<Complete(std::optional<RegionTile::Vector>)> cont
		) const
	{
		struct VectorQueryTitler : VectorQueryConstVisitor
		{
			std::string title;
			void visit(VectorQueryMeleeAttack const&) final { title = "Melee Attack"; }
		};
		struct VectorQueryPrompter : VectorQueryConstVisitor
		{
			std::string prompt;
			void visit(VectorQueryMeleeAttack const&) final { prompt = "Choose attack direction."; }
		};

		VectorQueryTitler titler;
		query->accept(titler);
		VectorQueryPrompter prompter;
		query->accept(prompter);
		auto dialog = std::make_unique<VectorDialog>(std::move(titler.title), std::move(prompter.prompt), origin, std::move(predicate), std::move(cont));
		return game().add_dialog(std::move(dialog));
	}

	Complete Player::query_being
		( uptr<BeingQuery> //query
		, function<bool(Being&)> //predicate
		, function<Complete(std::optional<Being*>)> cont
		) const
	{
		// spell::Heal: "Heal Target", "Select a being to be healed."
		//! @todo This.
		return cont(std::nullopt);
	}

	Complete Player::query_item
		( uptr<ItemQuery> //query
		, Being& //source
		, function<bool(Being&)> //predicate
		, function<Complete(std::optional<Item*>)> cont
		) const
	{
		//! @todo This.
		return cont(std::nullopt);
	}

	// Quick Time Events

	Complete Player::get_lightning_bolt_quality(RegionTile::Point target_coords, std::function<Complete(double)> cont) const
	{
		auto dialog = std::make_unique<qte::LightningBolt>(target_coords, std::move(cont));
		return game().add_dialog(std::move(dialog));
	}
}
