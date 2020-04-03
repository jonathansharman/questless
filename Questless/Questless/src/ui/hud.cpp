//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "hud.hpp"

#include "dialog/list_dialog.hpp"

#include "agents/actions.hpp"
#include "agents/agent.hpp"
#include "entities/beings/body.hpp"
#include "items/equipment.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "rsrc/hud.hpp"
#include "world/region.hpp"

#include "cancel/serialization.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/take.hpp>

namespace ql {
	namespace {
		sf::Vector2i const item_icon_size{55, 55};
	}

	hud::hud( //
		reg& reg,
		uptr<widget>& root,
		rsrc::fonts const& fonts,
		id region_id,
		id player_id)
		: _reg{&reg}
		, _root{root}
		, _rsrc{fonts}
		, _region_id{region_id}
		, _player_id{player_id}
		, _world_widget{reg, rsrc::world_widget{_rsrc.entity, _rsrc.fonts, _rsrc.particle, _rsrc.tile}}
		, _hotbar{reg, _rsrc.item, _rsrc.spell}
		, _inv{reg.get<inventory>(player_id), _hotbar}
		, _state{state::player_input}
		, _game_logic_thread{make_game_logic_thread()} //
	{
		// Set the player's HUD pointer.
		std::get<player>(reg.get<agent>(_player_id).value).set_hud(*this);

		// When a hotbar item is selected, open a list dialog to choose an action.
		_hotbar.set_on_click([this](std::optional<id> o_item_id, view::point mouse_position) {
			if (o_item_id) {
				_item_dialog = umake<list_dialog>(this, _rsrc.fonts, "Act...", get_item_options(*o_item_id));
				_item_dialog->on_parent_resize(get_size());
				_item_dialog->set_position(mouse_position);
			}
		});

		{ // Initialize hotbar with as many items as possible.
			auto& inv = reg.get<inventory>(_player_id);
			for (auto [idx, item] : ranges::views::enumerate(inv.item_ids) | ranges::views::take(hotbar::item_count)) {
				_hotbar.set_item(idx, item);
			}
		}

		// Render the initial world view.
		_world_widget.render_view(world_view{*_reg, _player_id});

		// Begin game loop.
		_state.store(state::game_loop);
	}

	hud::~hud() {
		// Pass the turn so the game loop can finish its current iteration.
		_pass_promise.set_value();

		// Inform the game loop that the game is ending.
		_state.store(state::ending);

		// Await the game loop.
		_game_logic_thread.join();
	}

	auto hud::set_player_id(id player_id) -> void {
		_player_id = player_id;
	}

	auto hud::render_effect(effects::effect const& effect) -> void {
		_world_widget.render_effect(effect);
	}

	auto hud::pass_future() -> std::future<void> {
		_world_widget.render_view(world_view{*_reg, _player_id});
		_state.store(state::player_input);
		return _pass_promise.get_future();
	}

	auto hud::get_size() const -> view::vector {
		return _size;
	}

	auto hud::update(sec elapsed_time) -> void {
		if (_item_dialog) {
			_item_dialog->update(elapsed_time);
		} else if (_show_inv) {
			_inv.update(elapsed_time);
		} else {
			// Pass the turn as long as X is held.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) { return pass(); }

			_hotbar.update(elapsed_time);
		}
		_world_widget.update(elapsed_time);
	}

	auto hud::set_position(view::point position) -> void {
		_position = position;
		// Set world widget position.
		_world_widget.set_position(_position);
		{ // Set hotbar position.
			auto const hotbar_size = _hotbar.get_size();
			_hotbar.set_position(_position + view::vector{(_size[0] - hotbar_size[0]) / 2.0f, _size[1] - hotbar_size[1]});
		}
		// Set inventory position.
		_inv.set_position(_position + (_size - _inv.get_size()) / 2.0f);
	}

	auto hud::get_position() const -> view::point {
		return _position;
	}

	auto hud::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
		_world_widget.on_parent_resize(_size);
		_hotbar.on_parent_resize(_size);
		_inv.on_parent_resize(_size);
		// Call set_position to trigger sub-widget repositioning.
		set_position(_position);
	}

	auto hud::on_key_press(sf::Event::KeyEvent const& event) -> event_handled {
		switch (event.code) {
			// Hide inventory.
			case sf::Keyboard::Escape:
				_show_inv = false;
				break;
			// Toggle inventory.
			case sf::Keyboard::Tab:
				_show_inv = !_show_inv;
				break;
			// Pass a single turn if Z or Return is pressed.
			case sf::Keyboard::Z:
				[[fallthrough]];
			case sf::Keyboard::Return:
				pass();
				return event_handled::yes;
			// Movement commands.
			case sf::Keyboard::Q:
				move(*_reg, _player_id, hex_direction::ul, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			case sf::Keyboard::W:
				move(*_reg, _player_id, hex_direction::u, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			case sf::Keyboard::E:
				move(*_reg, _player_id, hex_direction::ur, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			case sf::Keyboard::A:
				move(*_reg, _player_id, hex_direction::dl, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			case sf::Keyboard::S:
				move(*_reg, _player_id, hex_direction::d, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			case sf::Keyboard::D:
				move(*_reg, _player_id, hex_direction::dr, event.shift);
				_world_widget.render_view(world_view{*_reg, _player_id});
				break;
			// Snap camera to player.
			case sf::Keyboard::Space:
				_world_widget.set_position(
					view::point{} - tile_layout.to_world(_reg->get<location>(_player_id).coords) + view::point{});
				return event_handled::yes;
			default:
				return event_handled::no;
		}
		return event_handled::yes;
	}

	auto hud::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		auto result = _hotbar.on_mouse_press(event);
		if (result == event_handled::no) { result = _inv.on_mouse_press(event); }
		if (result == event_handled::no) { result = _world_widget.on_mouse_press(event); }
		return result;
	}

	auto hud::on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled {
		auto result = _hotbar.on_mouse_release(event);
		if (result == event_handled::no) { result = _inv.on_mouse_release(event); }
		if (result == event_handled::no) { result = _world_widget.on_mouse_release(event); }
		return result;
	}

	auto hud::on_mouse_move(view::point mouse_position) -> void {
		_hotbar.on_mouse_move(mouse_position);
		_inv.on_mouse_move(mouse_position);
		_world_widget.on_mouse_move(mouse_position);
	}

	auto hud::on_request_quit() -> event_handled {
		_root = nullptr;
		return event_handled::yes;
	}

	auto hud::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(_world_widget, states);

		//! @todo Condition bars.
		// Blood
		// Energy
		// Satiety
		// Alertness

		target.draw(_hotbar, states);

		// Draw the inventory if it's open.
		if (_show_inv) { target.draw(_inv, states); }

		{ // Draw the current time.
			auto const& region = _reg->get<ql::region>(_region_id);
			tick const time_of_day = region.time_of_day();
			std::string time_name;
			switch (region.period_of_day()) {
				case period_of_day::morning:
					time_name = "Morning";
					break;
				case period_of_day::afternoon:
					time_name = "Afternoon";
					break;
				case period_of_day::dusk:
					time_name = "Dusk";
					break;
				case period_of_day::evening:
					time_name = "Evening";
					break;
				case period_of_day::night:
					time_name = "Night";
					break;
				case period_of_day::dawn:
					time_name = "Dawn";
					break;
			}
			std::string time_string = fmt::format("Time: {} ({}, {})", region.time(), time_of_day, time_name);
			sf::Text time_text{time_string, _rsrc.fonts.firamono, 20};
			time_text.setOutlineColor(sf::Color::Black);
			time_text.setOutlineThickness(1.0f);
			time_text.setFillColor(sf::Color::White);
			time_text.setPosition({0, 50});
			target.draw(time_text, states);
		}
	}

	auto hud::get_item_options(id item_id) -> std::vector<std::tuple<sf::String, std::function<void()>>> {
		std::vector<std::tuple<sf::String, std::function<void()>>> result;
		if (auto equipment = _reg->try_get<ql::equipment>(item_id)) {
			if (equipment->equipped()) {
				if (auto bow = _reg->try_get<ql::bow>(item_id)) {
					if (bow->nocked_arrow_id) {
						result.emplace_back("Draw", [bow] { bow->draw(); });
						result.emplace_back("Loose", [bow] { bow->loose(); });
					} else {
						result.emplace_back("Nock", [bow] {
							//! @todo Choose and nock arrow.
							// bow->nock(arrow_id);
						});
					}
				} else if (auto quarterstaff = _reg->try_get<ql::quarterstaff>(item_id)) {
					result.emplace_back("Strike", [quarterstaff] { quarterstaff->strike(); });
					result.emplace_back("Jab", [quarterstaff] { quarterstaff->jab(); });
				}
				result.emplace_back("Unequip", [equipment] { equipment->unequip(); });
				return result;
			} else {
				result.emplace_back("Equip", [id = _player_id, equipment] { equipment->equip(id); });
				// Fall through to the drop and toss actions.
			}
		}
		result.emplace_back("Drop", [this, id = _player_id, item_id] { drop(*_reg, id, item_id); });
		result.emplace_back("Toss", [this, id = _player_id, item_id] { toss(*_reg, id, item_id); });
		return result;
	}

	auto hud::pass() -> void {
		// Pass control by satisfying the current pass promise.
		_pass_promise.set_value();
		// Reassign the pass promise in preparation for next turn.
		_pass_promise = std::promise<void>{};
		// Resume the game loop.
		_state.store(state::game_loop);
	}

	auto hud::make_game_logic_thread() -> std::thread {
		return std::thread{[this] {
			for (;;) {
				switch (_state.load()) {
					case state::player_input:
						// Awaiting player input.
						break;
					case state::game_loop: {
						constexpr auto elapsed_ticks = 1_tick;

						// Update the region.
						_reg->get<region>(_region_id).update(elapsed_ticks);

						// For each being, update its body and allow it to act.
						_reg->view<body, agent>().each([this, elapsed_ticks](body& body, agent& agent) {
							body.update(elapsed_ticks);
							agent.act().get();
						});

						break;
					}
					case state::ending:
						return;
				}
			}
		}};
	}
}
