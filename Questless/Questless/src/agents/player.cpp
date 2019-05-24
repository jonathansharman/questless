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
#include "utility/utility.hpp"
#include "utility/visitation.hpp"

#include <fmt/format.h>

using std::function;

namespace ql {
	player::player(ent id, sf::Window const& window, rsrc::fonts const& fonts)
		: agent{id}, _window{window}, _fonts{fonts}, _hud{id} {}

	player::~player() = default;

	world_view const& player::world_view() const {
		return *_world_view;
	}

	void player::update_view() {
		_world_view = umake<ql::world_view>(reg.get<being>(id));
	}

	std::vector<std::tuple<sf::String>, std::function<void()>> player::get_item_options(agent& agent, ent item_id) {
		std::vector<std::tuple<sf::String>, std::function<void()>> result;
		result.emplace_back("Drop", [this, item_id] { drop(item_id); });
		result.emplace_back("Toss", [this, item_id] { toss(item_id); });
		return result;
	}

	action::result player::act() {
		return query_player_choice([this](player_action_dialog::choice player_choice) {
			return match(std::move(player_choice), //
				[this](player_action_dialog::idle const& idle) {
					if (idle.prolonged) {
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
				},
				[this](player_action_dialog::move const& move) {
					if (move.strafe) {
						// Strafe.
						return this->walk(move.direction, [this](action::result result) {
							if (result == action::result::aborted) {
								// Chosen action aborted. Player must try to act again.
								return this->act();
							}
							return complete{};
						});
					} else {
						// Turn towards the chosen direction or move in that direction if already
						// facing that way.
						if (reg.get<conditions>(id).direction == move.direction) {
							return this->walk(move.direction, [this](action::result result) {
								if (result == action::result::aborted) {
									// Chosen action aborted. Player must try to act again.
									return this->act();
								}
								return complete{};
							});
						} else {
							return this->turn(move.direction, [this](action::result result) {
								if (result == action::result::aborted) {
									// Chosen action aborted. Player must try to act again.
									return this->act();
								}
								return complete{};
							});
						}
					}
				},
				[this](player_action_dialog::use const& use) {
					//! @todo Sync the hotbar with changes to the inventory.
					if (std::optional<ent> opt_item_id = _hud.hotbar()[use.index]) {
						// Open list dialog for the player to choose an action.
						auto [action_names, cont] = get_item_action_names_and_conts(*opt_item_id);
						auto dialog = umake<list_dialog>(_window,
							_fonts,
							sf::Vector2f{sf::Mouse::getPosition()},
							item.name,
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
				});
		});
	}

	void player::perceive(effects::effect const& effect) {
		_perceived_effects.push_back(effect);
	}

	std::vector<effects::effect> player::poll_perceived_effects() {
		std::vector<effects::effect> perceived_effects = std::move(_perceived_effects);
		_perceived_effects.clear();
		return perceived_effects;
	}

	complete player::send_message(queries::message::query message, function<complete()> cont) {
		using namespace queries::message;
		using ret_t = std::array<sf::String, 2>;
		auto [title, prompt] = match(message,
			[&](arrow_miss const&) -> ret_t {
				return {"Ranged Attack", "Miss!"};
			},
			[](cannot_equip const&) -> ret_t {
				return {"Cannot Equip", "You don't have the requisite free body parts to equip this."};
			},
			[](entity_in_the_way const&) -> ret_t {
				return {"Obstruction", "There's something in the way."};
			},
			[](gatestone_missing const&) -> ret_t {
				return {"Gatestone Missing", "Your gatestone is missing."};
			},
			[](incant_failed_mute const&) -> ret_t {
				return {"Incantation", "You cannot perform an incantation while mute."};
			},
			[](melee_miss const&) -> ret_t {
				return {"Melee Attack", "Miss!"};
			},
			[](not_enough_ammo const&) -> ret_t {
				return {"Attack", "Not enough ammo!"};
			},
			[](not_enough_charge const& nec) -> ret_t {
				return {"Spell Cast",
					fmt::format("Not enough charge! Your gatestone needs {} more charge to cast this.",
						nec.charge_deficit.value)};
			});

		_dialogs.push_back(umake<message_dialog>(_window, _fonts, title, prompt, std::move(cont)));
		return complete{};
	}

	complete player::query_count(queries::count::query query,
		int default_value,
		std::optional<int> min,
		std::optional<int> max,
		function<complete(std::optional<int>)> cont) //
	{
		auto [title, prompt] = std::visit(
			[](queries::count::wait_time const&) -> std::array<sf::String, 2> {
				return {"Wait", "Enter wait time."};
			},
			query);

		_dialogs.push_back(
			umake<count_dialog>(_window, _fonts.dumbledor1, title, prompt, default_value, min, max, std::move(cont)));
		return complete{};
	}

	complete player::query_magnitude(queries::magnitude::query query,
		double default_value,
		std::optional<double> min,
		std::optional<double> max,
		function<complete(std::optional<double>)> cont) //
	{
		using ret_t = std::array<sf::String, 2>;
		auto [title, prompt] = match(query, //
			[&](queries::magnitude::heal const&) -> ret_t {
				return {"Heal Amount", "Choose how much health to restore."};
			},
			[&](queries::magnitude::shock const&) -> ret_t {
				return {"Lightning Bolt Strength", "Choose how strong to make the lightning bolt."};
			});

		_dialogs.push_back(umake<magnitude_dialog>(_window, _fonts, title, prompt, default_value, min, max, std::move(cont)));
		return complete{};
	}

	complete player::query_tile(queries::tile::query query,
		std::optional<region_tile::point> origin,
		function<bool(region_tile::point)> predicate,
		function<complete(std::optional<region_tile::point>)> cont) //
	{
		using ret_t = std::array<sf::String, 2>;
		using namespace queries::tile;
		auto [title, prompt] = match(query, //
			[](ranged_attack_target const&) -> ret_t {
				return {"Ranged Attack", "Choose attack target."};
			},
			[](shock_target const&) -> ret_t {
				return {"Shock Target", "Select a tile to be zapped with an electric discharge."};
			},
			[](teleport_target const&) -> ret_t {
				return {"Teleport Target", "Select a tile to teleport to."};
			});

		_dialogs.push_back(
			umake<tile_dialog>(_window, _fonts, title, prompt, std::move(origin), std::move(predicate), std::move(cont)));
		return complete{};
	}

	complete player::query_vector(queries::vector::query query,
		std::optional<region_tile::point> origin,
		std::function<bool(region_tile::vector)> predicate,
		std::function<complete(std::optional<region_tile::vector>)> cont) //
	{
		using ret_t = std::array<sf::String, 2>;
		auto [title, prompt] = std::visit(
			[](queries::vector::melee_attack const&) -> ret_t {
				return {"Melee Attack", "Choose attack direction."};
			},
			query);

		_dialogs.push_back(
			umake<vector_dialog>(_window, _fonts, title, prompt, origin, std::move(predicate), std::move(cont)));
		return complete{};
	}

	complete player::query_player_choice(function<void(player_action_dialog::choice)> cont) {
		_dialogs.push_back(umake<player_action_dialog>(_window, _fonts, _hud, move(cont)));
		return complete{};
	}
}
