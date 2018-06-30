//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "agents/player.hpp"

#include "agents/queries/all_queries.hpp"
#include "entities/beings/world_view.hpp"
#include "game.hpp"
#include "sdl/resources.hpp"
#include "ui/count_dialog.hpp"
#include "ui/direction_dialog.hpp"
#include "ui/list_dialog.hpp"
#include "ui/magnitude_dialog.hpp"
#include "ui/message_dialog.hpp"
#include "ui/qte/aim_missile.hpp"
#include "ui/qte/incant.hpp"
#include "ui/qte/shock.hpp"
#include "ui/tile_dialog.hpp"
#include "ui/vector_dialog.hpp"
#include "utility/reference.hpp"

using std::function;

namespace ql
{
	player::player(ql::being& being)
		: agent{being}
		, _world_view{nullptr}
	{}

	player::~player() = default;

	world_view const& player::world_view() const { return *_world_view; }

	void player::update_world_view() { _world_view = umake<ql::world_view>(being, true); }

	complete player::act()
	{
		the_game().query_player_choice([this](player_action_dialog::choice player_choice) {
			struct player_choice_executor
			{
				player& player;

				void operator ()(player_action_dialog::idle const& i)
				{
					if (i.prolonged) {
						player.idle([&player = player](action::result result) {
							if (result == action::result::aborted) {
								// Chosen action aborted. Player must try to act again.
								player.act();
							}
							return complete{};
						});
					} else {
						player.idle(1.0);
					}
				}
				void operator ()(player_action_dialog::move const& m)
				{
					if (m.strafe) {
						// Strafe.
						player.walk(m.direction, [&player = player](action::result result) {
							if (result == action::result::aborted) {
								// Chosen action aborted. Player must try to act again.
								player.act();
							}
							return complete{};
						});
					} else {
						// Turn towards the chosen direction or move in that direction if already facing that way.
						if (player.being.direction == m.direction) {
							player.walk(m.direction, [&player = player](action::result result) {
								if (result == action::result::aborted) {
									// Chosen action aborted. Player must try to act again.
									player.act();
								}
								return complete{};
							});
						} else {
							player.turn(m.direction, [&player = player](action::result result) {
								if (result == action::result::aborted) {
									// Chosen action aborted. Player must try to act again.
									player.act();
								}
								return complete{};
							});
						}
					}
				}
				void operator ()(player_action_dialog::use const& u)
				{
					//! @todo Sync the hotbar with changes to the inventory.
					if (std::optional<id<item>> opt_item_id = the_game().hud().hotbar()[u.index]) {
						item& item = the_game().items.ref(*opt_item_id);
						// Get a list of the item's actions. It's shared so the lambda that captures it is copyable, so the lambda can be passed as a std::function.
						auto actions = smake<std::vector<uptr<action>>>(item.actions());
						// Get the action names from the list of actions.
						std::vector<std::string> action_names;
						std::transform(actions->begin(), actions->end(), std::back_inserter(action_names), [](uptr<action> const& action) { return action->name(); });
						// Open list dialog for the player to choose an action.
						auto dialog = umake<list_dialog>(sdl::the_input().mouse_position(), item.name(), std::move(action_names),
							[&player = player, actions](std::optional<int> opt_action_idx) {
								if (!opt_action_idx) {
									// No action selected. Player must try to act again.
									player.act();
									return complete{};
								} else {
									// Perform the chosen action.
									int action_idx = *opt_action_idx;
									return (*actions)[action_idx]->perform(player.being, [&player = player](action::result result) {
										if (result == action::result::aborted) {
											// Chosen action aborted. Player must try to act again.
											player.act();
										}
										return complete{};
									});
								}
							}
						);
						the_game().add_dialog(std::move(dialog));
					}
				}
			};
			std::visit(player_choice_executor{*this}, player_choice);
		});
	}

	void player::perceive(sptr<effect> const& effect)
	{
		_perceived_effects.push_back(effect);
	}

	std::vector<sptr<effect>> player::poll_perceived_effects()
	{
		std::vector<sptr<effect>> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	complete player::send_message
		( uptr<message> message
		, function<complete()> cont
		) const
	{
		struct message_titler : message_const_visitor
		{
			std::string title;
			void visit(message_arrow_miss const&) final { title = "Ranged Attack"; }
			void visit(message_cannot_equip const&) final { title = "Cannot Equip"; }
			void visit(message_entity_in_the_way const&) final { title = "Obstruction"; }
			void visit(message_incant_failed_mute const&) final { title = "Incantation"; }
			void visit(message_melee_miss const&) final { title = "Melee Attack"; }
			void visit(message_not_enough_ammo const&) final { title = "Attack"; }
			void visit(message_not_enough_charge const&) final { title = "Spell Cast"; }
		};
		struct message_prompter : message_const_visitor
		{
			std::string prompt;
			void visit(message_arrow_miss const&) final { prompt = "Miss!"; }
			void visit(message_cannot_equip const&) final { prompt = "You don't have the requisite free body parts to equip this."; }
			void visit(message_entity_in_the_way const&) final { prompt = "There's something in the way!"; }
			void visit(message_incant_failed_mute const&) final { prompt = "You can't perform an incantation while mute!"; }
			void visit(message_melee_miss const&) final { prompt = "Miss!"; }
			void visit(message_not_enough_ammo const&) final { prompt = "Not enough ammo!"; }
			void visit(message_not_enough_charge const& m) final
			{
				prompt = "Not enough charge! Your gatestone needs " + std::to_string(m.charge_deficit) + " more charge to cast this.";
			}
		};

		message_titler titler;
		message->accept(titler);
		message_prompter prompter;
		message->accept(prompter);
		auto dialog = umake<message_dialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return the_game().add_dialog(std::move(dialog));
	}

	complete player::query_count
		( uptr<count_query> query
		, int default_value
		, std::optional<int> min
		, std::optional<int> max
		, function<complete(std::optional<int>)> cont
		) const
	{
		struct count_query_titler : count_query_const_visitor
		{
			std::string title;
		};
		struct count_query_prompter : count_query_const_visitor
		{
			std::string prompt;
		};

		count_query_titler titler;
		query->accept(titler);
		count_query_prompter prompter;
		query->accept(prompter);
		auto dialog = umake<count_dialog>(std::move(titler.title), std::move(prompter.prompt), default_value, min, max, std::move(cont));
		return the_game().add_dialog(std::move(dialog));
	}

	complete player::query_magnitude
		( uptr<magnitude_query> query
		, double default_value
		, std::optional<double> min
		, std::optional<double> max
		, function<complete(std::optional<double>)> cont
		) const
	{
		struct magnitude_query_titler : magnitude_query_const_visitor
		{
			std::string title;
			void visit(magnitude_query_heal const&) final { title = "Heal Amount"; }
			void visit(magnitude_query_shock const&) final { title = "Lightning Bolt Strength"; }
			void visit(magnitude_query_wait_time const&) final { title = "Wait"; }
		};
		struct magnitude_query_prompter : magnitude_query_const_visitor
		{
			std::string prompt;
			void visit(magnitude_query_heal const&) final { prompt = "Choose how much health to restore."; }
			void visit(magnitude_query_shock const&) final { prompt = "Choose how strong to make the lightning bolt."; }
			void visit(magnitude_query_wait_time const&) final { prompt = "Enter wait time."; }
		};

		magnitude_query_titler titler;
		query->accept(titler);
		magnitude_query_prompter prompter;
		query->accept(prompter);
		auto dialog = umake<magnitude_dialog>(std::move(titler.title), std::move(prompter.prompt), default_value, min, max, std::move(cont));
		return the_game().add_dialog(std::move(dialog));
	}
	
	complete player::query_tile
		( uptr<tile_query> query
		, std::optional<region_tile::point> origin
		, function<bool(region_tile::point)> predicate
		, function<complete(std::optional<region_tile::point>)> cont
		) const
	{
		struct tile_query_titler : tile_query_const_visitor
		{
			std::string title;
			void visit(tile_query_ranged_attack_target const&) final { title = "Ranged Attack"; }
			void visit(tile_query_shock_target const&) final { title = "Shock Target"; }
			void visit(tile_query_teleport_target const&) final { title = "Teleport Target"; }
		};
		struct tile_query_prompter : tile_query_const_visitor
		{
			std::string prompt;
			void visit(tile_query_ranged_attack_target const&) final { prompt = "Choose attack target."; }
			void visit(tile_query_shock_target const&) final { prompt = "Select a tile to be zapped with an electric discharge."; }
			void visit(tile_query_teleport_target const&) final { prompt = "Select a tile to teleport to."; }
		};

		tile_query_titler titler;
		query->accept(titler);
		tile_query_prompter prompter;
		query->accept(prompter);
		auto dialog = umake<tile_dialog>
			( std::move(titler.title)
			, std::move(prompter.prompt)
			, std::move(origin)
			, std::move(predicate)
			, std::move(cont)
			);
		return the_game().add_dialog(std::move(dialog));
	}

	complete player::query_direction
		( uptr<direction_query> query
		, function<complete(std::optional<region_tile::direction>)> cont
		) const
	{
		struct direction_query_titler : direction_query_const_visitor
		{
			std::string title;
		};
		struct direction_query_prompter : direction_query_const_visitor
		{
			std::string prompt;
		};

		direction_query_titler titler;
		query->accept(titler);
		direction_query_prompter prompter;
		query->accept(prompter);
		auto dialog = umake<direction_dialog>(std::move(titler.title), std::move(prompter.prompt), std::move(cont));
		return the_game().add_dialog(std::move(dialog));
	}

	complete player::query_vector
		( uptr<vector_query> query
		, std::optional<region_tile::point> origin
		, std::function<bool(region_tile::vector)> predicate
		, std::function<complete(std::optional<region_tile::vector>)> cont
		) const
	{
		struct vector_query_titler : vector_query_const_visitor
		{
			std::string title;
			void visit(vector_query_melee_attack const&) final { title = "Melee Attack"; }
		};
		struct vector_query_prompter : vector_query_const_visitor
		{
			std::string prompt;
			void visit(vector_query_melee_attack const&) final { prompt = "Choose attack direction."; }
		};

		vector_query_titler titler;
		query->accept(titler);
		vector_query_prompter prompter;
		query->accept(prompter);
		auto dialog = umake<vector_dialog>(std::move(titler.title), std::move(prompter.prompt), origin, std::move(predicate), std::move(cont));
		return the_game().add_dialog(std::move(dialog));
	}

	complete player::query_being
		( uptr<being_query> //query
		, function<bool(ql::being&)> //predicate
		, function<complete(std::optional<ql::being*>)> cont
		) const
	{
		// magic::heal: "heal Target", "Select a being to be healed."
		//! @todo This.
		return cont(std::nullopt);
	}

	complete player::query_item
		( uptr<item_query> //query
		, ql::being& //source
		, function<bool(ql::being&)> //predicate
		, function<complete(std::optional<item*>)> cont
		) const
	{
		//! @todo This.
		return cont(std::nullopt);
	}

	// Quick Time Events

	complete player::aim_missile(region_tile::point source_coords, ql::being& target_being, std::function<complete(body_part*)> cont) const
	{
		return the_game().add_dialog
			( umake<qte::aim_missile>(source_coords, target_being, std::move(cont))
			);
	}

	complete player::get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) const
	{
		return the_game().add_dialog
			( umake<qte::shock>(target_coords, std::move(cont))
			);
	}

	complete player::incant(gatestone& gatestone, std::function<complete(uptr<magic::spell>)> cont) const
	{
		return the_game().add_dialog
			( umake<qte::incant>(gatestone, std::move(cont))
			);
	}
}
