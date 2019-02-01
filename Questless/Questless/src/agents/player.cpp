//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "player.hpp"

#include "entities/beings/being.hpp"
#include "entities/beings/world_view.hpp"
#include "game.hpp"
#include "rsrc/fonts.hpp"
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
#include "utility/type_switch.hpp"
#include "utility/utility.hpp"

#include <fmt/format.h>

using std::function;

namespace ql {
	player::player(sf::Window const& window, rsrc::fonts const& fonts, ql::being& being)
		: agent{being}, _window{window}, _fonts{fonts}, _hud{being.id} {}

	player::~player() = default;

	world_view const& player::world_view() const {
		return *_world_view;
	}

	void player::update_view() {
		_world_view = umake<ql::world_view>(being);
	}

	complete player::act() {
		return query_player_choice([this](player_action_dialog::choice player_choice) {
			return std::visit(
				[this](auto&& player_choice) {
					SWITCH_TYPE(player_choice) {
						MATCH_TYPE(player_action_dialog::idle) {
							if (player_choice.prolonged) {
								return this->idle([this](action::result result) {
									if (result == action::result::aborted) {
										// Chosen action aborted. Player must try to act again.
										return this->act();
									}
									return complete{};
								});
							} else {
								return this->idle(1_tick);
							}
						}
						MATCH_TYPE(player_action_dialog::move) {
							if (player_choice.strafe) {
								// Strafe.
								return this->walk(player_choice.direction, [this](action::result result) {
									if (result == action::result::aborted) {
										// Chosen action aborted. Player must try to act again.
										return this->act();
									}
									return complete{};
								});
							} else {
								// Turn towards the chosen direction or move in that direction if already facing that way.
								if (this->being.direction == player_choice.direction) {
									return this->walk(player_choice.direction, [this](action::result result) {
										if (result == action::result::aborted) {
											// Chosen action aborted. Player must try to act again.
											return this->act();
										}
										return complete{};
									});
								} else {
									return this->turn(player_choice.direction, [this](action::result result) {
										if (result == action::result::aborted) {
											// Chosen action aborted. Player must try to act again.
											return this->act();
										}
										return complete{};
									});
								}
							}
						}
						MATCH_TYPE(player_action_dialog::use) {
							//! @todo Sync the hotbar with changes to the inventory.
							if (std::optional<id<item>> opt_item_id = _hud.hotbar()[player_choice.index]) {
								item& item = the_game().items.ref(*opt_item_id);
								// Get a list of the item's actions. It's shared, so the lambda that captures it is
								// copyable, so the lambda can be passed as a std::function.
								auto actions = smake<std::vector<uptr<action>>>(item.actions());
								// Get the action names from the list of actions.
								std::vector<sf::String> action_names;
								std::transform(actions->begin(),
									actions->end(),
									std::back_inserter(action_names),
									[](uptr<action> const& action) { return action->name(); });
								// Open list dialog for the player to choose an action.
								auto dialog = umake<list_dialog>(_window,
									_fonts,
									sf::Vector2f{sf::Mouse::getPosition()},
									item.name(),
									std::move(action_names),
									[this, actions = std::move(actions)](std::optional<int> opt_action_idx) {
										if (!opt_action_idx) {
											// No action selected. Player must try to act again.
											return this->act();
										} else {
											// Perform the chosen action.
											int action_idx = *opt_action_idx;
											return (*actions)[action_idx]->perform(this->being, [this](action::result result) {
												if (result == action::result::aborted) {
													// Chosen action aborted. Player must try to act again.
													return this->act();
												}
												return complete{};
											});
										}
									});
								_dialogs.push_back(std::move(dialog));
							}
							return complete{};
						}
					}
				},
				std::move(player_choice));

			//! @todo Why doesn't the "overloaded" version work here?

			// return std::visit(
			//	overloaded(
			//		[this](player_action_dialog::idle i) {
			//			if (i.prolonged) {
			//				return this->idle([this](action::result result) {
			//					if (result == action::result::aborted) {
			//						// Chosen action aborted. Player must try to act again.
			//						return this->act();
			//					}
			//					return complete{};
			//				});
			//			} else {
			//				return this->idle(1.0);
			//			}
			//		},
			//		[this](player_action_dialog::move m) {
			//			if (m.strafe) {
			//				// Strafe.
			//				return this->walk(m.direction, [this](action::result result) {
			//					if (result == action::result::aborted) {
			//						// Chosen action aborted. Player must try to act again.
			//						return this->act();
			//					}
			//					return complete{};
			//				});
			//			} else {
			//				// Turn towards the chosen direction or move in that direction if already facing that way.
			//				if (this->being.direction == m.direction) {
			//					return this->walk(m.direction, [this](action::result result) {
			//						if (result == action::result::aborted) {
			//							// Chosen action aborted. Player must try to act again.
			//							return this->act();
			//						}
			//						return complete{};
			//					});
			//				} else {
			//					return this->turn(m.direction, [this](action::result result) {
			//						if (result == action::result::aborted) {
			//							// Chosen action aborted. Player must try to act again.
			//							return this->act();
			//						}
			//						return complete{};
			//					});
			//				}
			//			}
			//		},
			//		[this](player_action_dialog::use u) {
			//			//! @todo Sync the hotbar with changes to the inventory.
			//			if (std::optional<id<item>> opt_item_id = the_game().hud().hotbar()[u.index]) {
			//				item& item = the_game().items.ref(*opt_item_id);
			//				// Get a list of the item's actions. It's shared so the lambda that captures it is copyable,
			// so the lambda can be passed as a std::function. 				auto actions =
			// smake<std::vector<uptr<action>>>(item.actions());
			//				// Get the action names from the list of actions.
			//				std::vector<std::string> action_names;
			//				std::transform(actions->begin(), actions->end(), std::back_inserter(action_names),
			//[](uptr<action> const& action) { return action->name(); });
			//				// Open list dialog for the player to choose an action.
			//				auto dialog = umake<list_dialog>(im.mouse_position(), item.name(), std::move(action_names),
			//					[this, actions = std::move(actions)](std::optional<int> opt_action_idx) {
			//						if (!opt_action_idx) {
			//							// No action selected. Player must try to act again.
			//							return this->act();
			//						} else {
			//							// Perform the chosen action.
			//							int action_idx = *opt_action_idx;
			//							return (*actions)[action_idx]->perform(this->being, [this](action::result
			// result) { 								if (result == action::result::aborted) {
			//									// Chosen action aborted. Player must try to act again.
			//									return this->act();
			//								}
			//								return complete{};
			//							});
			//						}
			//					}
			//				);
			//				return the_game().add_dialog(std::move(dialog));
			//			}
			//		}
			//	),
			//	std::move(player_choice)
			//);
		});
	}

	void player::perceive(sptr<effect> const& effect) {
		_perceived_effects.push_back(effect);
	}

	std::vector<sptr<effect>> player::poll_perceived_effects() {
		std::vector<sptr<effect>> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	complete player::send_message(queries::message::any message, function<complete()> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& message) -> std::array<std::string, 2> {
				using namespace queries::message;
				SWITCH_TYPE(message) {
					MATCH_TYPE(arrow_miss) return {"Ranged Attack", "Miss!"};
					MATCH_TYPE(cannot_equip)
					return {"Cannot Equip", "You don't have the requisite free body parts to equip this."};
					MATCH_TYPE(entity_in_the_way) return {"Obstruction", "There's something in the way."};
					MATCH_TYPE(gatestone_missing) return {"Gatestone Missing", "Your gatestone is missing."};
					MATCH_TYPE(incant_failed_mute)
					return {"Incantation", "You cannot perform an incantation while mute."};
					MATCH_TYPE(melee_miss) return {"Melee Attack", "Miss!"};
					MATCH_TYPE(not_enough_ammo) return {"Attack", "Not enough ammo!"};
					MATCH_TYPE(not_enough_charge)
					return {"Spell Cast",
						fmt::format("Not enough charge! Your gatestone needs {} more charge to cast this.",
							message.charge_deficit.value)};
				}
			},
			message);

		_dialogs.push_back(umake<message_dialog>(_window, _fonts, title, prompt, std::move(cont)));
		return complete{};
	}

	complete player::query_count(queries::count::any query,
		int default_value,
		std::optional<int> min,
		std::optional<int> max,
		function<complete(std::optional<int>)> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& query) -> std::array<sf::String, 2> {
				SWITCH_TYPE(query) {
					MATCH_TYPE(queries::count::wait_time) return {"Wait", "Enter wait time."};
				}
			},
			query);

		_dialogs.push_back(
			umake<count_dialog>(_window, _fonts.dumbledor1, title, prompt, default_value, min, max, std::move(cont)));
		return complete{};
	}

	complete player::query_magnitude(queries::magnitude::any query,
		double default_value,
		std::optional<double> min,
		std::optional<double> max,
		function<complete(std::optional<double>)> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& query) -> std::array<sf::String, 2> {
				SWITCH_TYPE(query) {
					MATCH_TYPE(queries::magnitude::heal) return {"Heal Amount", "Choose how much health to restore."};
					MATCH_TYPE(queries::magnitude::shock)
					return {"Lightning Bolt Strength", "Choose how strong to make the lightning bolt."};
				}
			},
			query);

		_dialogs.push_back(umake<magnitude_dialog>(_window, _fonts, title, prompt, default_value, min, max, std::move(cont)));
		return complete{};
	}

	complete player::query_tile(queries::tile::any query,
		std::optional<region_tile::point> origin,
		function<bool(region_tile::point)> predicate,
		function<complete(std::optional<region_tile::point>)> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& query) -> std::array<sf::String, 2> {
				SWITCH_TYPE(query) {
					MATCH_TYPE(queries::tile::ranged_attack_target) return {"Ranged Attack", "Choose attack target."};
					MATCH_TYPE(queries::tile::shock_target)
					return {"Shock Target", "Select a tile to be zapped with an electric discharge."};
					MATCH_TYPE(queries::tile::teleport_target)
					return {"Teleport Target", "Select a tile to teleport to."};
				}
			},
			query);

		_dialogs.push_back(
			umake<tile_dialog>(_window, _fonts, title, prompt, std::move(origin), std::move(predicate), std::move(cont)));
		return complete{};
	}

	complete player::query_direction(queries::direction::any query,
		function<complete(std::optional<region_tile::direction>)> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& query) -> std::array<sf::String, 2> {
				(void)query;
				return {"", ""}; // No direction queries currently.
			},
			query);

		_dialogs.push_back(umake<direction_dialog>(_window, _fonts, title, prompt, std::move(cont)));
		return complete{};
	}

	complete player::query_vector(queries::vector::any query,
		std::optional<region_tile::point> origin,
		std::function<bool(region_tile::vector)> predicate,
		std::function<complete(std::optional<region_tile::vector>)> cont) {
		auto [title, prompt] = std::visit(
			[&](auto&& query) -> std::array<std::string, 2> {
				SWITCH_TYPE(query) {
					MATCH_TYPE(queries::vector::melee_attack) return {"Melee Attack", "Choose attack direction."};
				}
			},
			query);

		_dialogs.push_back(
			umake<vector_dialog>(_window, _fonts, title, prompt, origin, std::move(predicate), std::move(cont)));
		return complete{};
	}

	complete player::query_being(queries::being::any // query
		,
		function<bool(ql::being&)> // predicate
		,
		function<complete(std::optional<ql::being*>)> cont) {
		// magic::heal: "heal Target", "Select a being to be healed."
		//! @todo This.
		return cont(std::nullopt);
	}

	complete player::query_item(queries::item::any // query
		,
		ql::being& // source
		,
		function<bool(ql::being&)> // predicate
		,
		function<complete(std::optional<item*>)> cont) {
		//! @todo This.
		return cont(std::nullopt);
	}

	complete player::query_player_choice(function<void(player_action_dialog::choice)> cont) {
		_dialogs.push_back(umake<player_action_dialog>(_window, _fonts, _hud, move(cont)));
		return complete{};
	}

	// Quick Time Events

	complete player::aim_missile(region_tile::point source_coords,
		ql::being& target_being,
		std::function<complete(body_part*)> cont) {
		_dialogs.push_back(umake<qte::aim_missile>(source_coords, target_being, std::move(cont)));
		return complete{};
	}

	complete player::get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) {
		_dialogs.push_back(umake<qte::shock>(_window, _fonts, target_coords, std::move(cont)));
		return complete{};
	}

	complete player::incant(gatestone& gatestone, std::function<complete(uptr<magic::spell>)> cont) {
		_dialogs.push_back(umake<qte::incant>(gatestone, std::move(cont)));
		return complete{};
	}
}
