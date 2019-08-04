//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "hud.hpp"

#include "dialog/list_dialog.hpp"

#include "agents/actions.hpp"
#include "entities/beings/body.hpp"
#include "items/equipment.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "rsrc/hud.hpp"
#include "world/region.hpp"

#include "cancel/serialization.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace ql {
	namespace {
		sf::Vector2i const item_icon_size{55, 55};

		auto get_world_widget_resources(rsrc::hud const& resources) {
			return rsrc::world_widget{resources.entity, resources.fonts, resources.particle, resources.tile};
		}
	}

	hud::hud( //
		rsrc::fonts const& fonts,
		id region_id,
		id player_id)
		: _resources{fonts}
		, _region_id{region_id}
		, _player_id{player_id}
		, _world_widget{get_world_widget_resources(_resources)}
		, _hotbar{}
		, _inv{reg.get<inventory>(player_id), _hotbar} //
	{
		// When a hotbar item is selected, open a list dialog to choose an action.
		_hotbar.set_on_click([this](std::optional<id> o_item_id) {
			if (o_item_id) {
				_item_dialog = umake<list_dialog>(_resources.fonts, "Act...", get_item_options(*o_item_id));
				_item_dialog->on_parent_resize(get_size());
				_item_dialog->set_position(view::point_from_sfml(sf::Mouse::getPosition()));
			}
		});
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
	}

	auto hud::render_effect(effects::effect const& effect) -> void {
		_world_widget.render_effect(effect);
	}

	auto hud::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		constexpr int conditions_count = 2;
		constexpr int condition_bar_width = 10;
		constexpr int condition_bar_height = 100;

		//! @todo Condition bars.
		// Blood
		// Energy
		// Satiety
		// Alertness

		target.draw(_hotbar, states);

		// Draw the inventory if it's open.
		if (_show_inv) { target.draw(_inv, states); }

		{ // Draw the current time.
			auto const& region = reg.get<ql::region>(_region_id);
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
			sf::Text time_text{time_string, _resources.fonts.firamono, 20};
			time_text.setFillColor(sf::Color::White);
			time_text.setPosition({0, 50});
			target.draw(time_text, states);
		}
	}

	auto hud::set_position(view::point position) -> void {
		_position = position;
	}

	auto hud::get_position() const -> view::point {
		return _position;
	}

	auto hud::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
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
			case sf::Keyboard::E:
				move(_player_id, region_tile::direction::one, event.shift);
				break;
			case sf::Keyboard::W:
				move(_player_id, region_tile::direction::two, event.shift);
				break;
			case sf::Keyboard::Q:
				move(_player_id, region_tile::direction::three, event.shift);
				break;
			case sf::Keyboard::A:
				move(_player_id, region_tile::direction::four, event.shift);
				break;
			case sf::Keyboard::S:
				move(_player_id, region_tile::direction::five, event.shift);
				break;
			case sf::Keyboard::D:
				move(_player_id, region_tile::direction::six, event.shift);
				break;
			default:
				return event_handled::no;
		}
		return event_handled::yes;
	}

	auto hud::get_item_options(id item_id) -> std::vector<std::tuple<sf::String, std::function<void()>>> {
		std::vector<std::tuple<sf::String, std::function<void()>>> result;
		if (auto equipment = reg.try_get<ql::equipment>(item_id)) {
			if (equipment->equipped()) {
				if (auto bow = reg.try_get<ql::bow>(item_id)) {
					if (bow->nocked_arrow_id) {
						result.emplace_back("Draw", [bow] { bow->draw(); });
						result.emplace_back("Loose", [bow] { bow->loose(); });
					} else {
						result.emplace_back("Nock", [bow] {
							//! @todo Choose and nock arrow.
							// bow->nock(arrow_id);
						});
					}
				} else if (auto quarterstaff = reg.try_get<ql::quarterstaff>(item_id)) {
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
		result.emplace_back("Drop", [id = _player_id, item_id] { drop(id, item_id); });
		result.emplace_back("Toss", [id = _player_id, item_id] { toss(id, item_id); });
		return result;
	}

	auto hud::pass() -> void {
		// Pass control by satisfying the current pass promise.
		_pass_promise.set_value();
		// Reassign the pass promise in preparation for next turn.
		_pass_promise = std::promise<void>{};
	}
}
