//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "hud.hpp"

#include "dialog/list_dialog.hpp"

#include "entities/beings/body.hpp"
#include "items/equipment.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "rsrc/hud.hpp"

#include "agents/actions.hpp"

namespace ql {
	namespace {
		sf::Vector2i const item_icon_size{55, 55};

		auto get_world_widget_resources(rsrc::hud const& resources) {
			return rsrc::world_widget{resources.entity, resources.fonts, resources.particle, resources.tile};
		}
	}

	hud::hud( //
		widget& parent,
		rsrc::hud const& resources,
		ent player_id)
		: widget{&parent}
		, _resources{resources}
		, _panel{*this}
		, _player_id{player_id}
		, _world_widget{_panel, get_world_widget_resources(resources)}
		, _hotbar{_panel}
		, _inv{_panel, reg.get<inventory>(player_id)} //
	{
		_panel.children.push_back(&_world_widget);
		_panel.children.push_back(&_hotbar);
		_panel.children.push_back(&_inv);
	}

	view::vector hud::get_size() const {
		return parent()->get_size();
	}

	void hud::update(sec elapsed_time, std::vector<sf::Event>& events) {
		body const& player_body = reg.get<body>(_player_id);

		for (auto event : events) {
			switch (event.type) {
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							_show_inv = false;
							break;
						case sf::Keyboard::Tab:
							_show_inv = !_show_inv;
							break;
						default:
							break;
					}
			}
		}

		if (_show_inv) {
			_inv.update(elapsed_time, events);
		} else {
			//! @todo Move the "move" and "use" functions.
			auto move = [&](region_tile::direction direction, bool strafe) {
				if (strafe) {
					// Strafe.
					walk(_player_id, direction);
				} else {
					// Turn towards the chosen direction or move in that direction if already facing that way.
					if (player_body.cond.direction == direction) {
						walk(_player_id, direction);
					} else {
						turn(_player_id, direction);
					}
				}
			};

			// Pass the turn as long as X is held.
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) { return pass(); }

			for (auto event : events) {
				switch (event.type) {
					case sf::Event::KeyPressed:
						switch (event.key.code) {
							// Pass a single turn if Z or Return is pressed.
							case sf::Keyboard::Z:
								[[fallthrough]];
							case sf::Keyboard::Return:
								return pass();
							// Movement commands.
							case sf::Keyboard::E:
								move(region_tile::direction::one, event.key.shift);
							case sf::Keyboard::W:
								move(region_tile::direction::two, event.key.shift);
							case sf::Keyboard::Q:
								move(region_tile::direction::three, event.key.shift);
							case sf::Keyboard::A:
								move(region_tile::direction::four, event.key.shift);
							case sf::Keyboard::S:
								move(region_tile::direction::five, event.key.shift);
							case sf::Keyboard::D:
								move(region_tile::direction::six, event.key.shift);
							default:
								break;
						}
					default:
						break;
				}
			}

			// Update hotbar and check for item use.
			_hotbar.update(elapsed_time, events);
			for (int idx : _hotbar.poll_selections()) {
				if (std::optional<ent> o_item_id = _hotbar.o_item_ids()[idx]) {
					// Open list dialog for the player to choose an action.
					auto dialog = umake<list_dialog>( //
						_window,
						_resources.fonts,
						view::point_from_sfml(sf::Mouse::getPosition()),
						"Act...",
						get_item_options(*o_item_id));
					_dialogs.push_back(std::move(dialog));
				}
			}
		}
	}

	void hud::render_effect(effects::effect const& effect) {
		_world_widget.render_effect(effect);
	}

	void hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		constexpr int conditions_count = 2;
		constexpr int condition_bar_width = 10;
		constexpr int condition_bar_height = 100;

		// Draw condition bars.
		auto [body, cond, stats] = reg.get<ql::body, body_cond, ql::stats::being>(_player_id);
		the_renderer().draw_box(spaces::window::box{spaces::window::point{0, screen_bottom},
									spaces::window::vector{condition_bar_width * conditions_count, condition_bar_height},
									{spaces::window::align_left, spaces::window::align_bottom}},
			colors::black());
		auto draw_bar = [condition_bar_width, condition_bar_height, screen_bottom, left = 0](
							double ratio, sf::Color const& color) mutable {
			the_renderer().draw_box(
				spaces::window::box{spaces::window::point{left + 1, screen_bottom},
					spaces::window::vector{condition_bar_width - 2, lround(condition_bar_height * ratio) - 1},
					{spaces::window::align_left, spaces::window::align_bottom}},
				color);
			left += condition_bar_width;
		};
		// Blood
		draw_bar((body.blood.value() / body.total_vitality() / body_part::blood_per_vitality).value, sf::Color::Red);
		// Energy
		draw_bar((cond.energy.value() / stats.a.stamina.value()).value, sf::Color::Cyan);
		// Satiety
		draw_bar((cond.satiety.value() / body_cond::max_satiety).value, sf::Color{130, 80, 40});
		// Alertness
		draw_bar((cond.alertness.value() / body_cond::max_alertness).value, sf::Color::Yellow);

		_hotbar.draw(target, states);

		// Draw the inventory if it's open.
		if (_show_inv) { _inv.draw(target, states); }
	}

	std::vector<std::tuple<sf::String, std::function<void()>>> hud::get_item_options(ent item_id) {
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

	void hud::pass() {
		// Pass control by satisfying the current pass promise.
		_pass_promise.set_value();
		// Reassign the pass promise in preparation for next turn.
		_pass_promise = std::promise<void>{};
	}
}
