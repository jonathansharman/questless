//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "hud.hpp"

#include "entities/beings/being.hpp"
#include "entities/beings/conditions.hpp"
#include "items/equipment.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"

#include "agents/actions.hpp"

namespace ql {
	namespace {
		sf::Vector2i const item_icon_size{55, 55};
	}

	sf::FloatRect hud::get_bounding_box() const {
		return {sf::Vector2f{_window.getPosition()}, sf::Vector2f{_window.getSize()}};
	}

	void hud::update(sec elapsed_time, input_manager& im) {
		body const& player_body = reg.get<body>(_player_id);
		update_displayed_items(_player_id);

		if (im.pressed(sf::Keyboard::Escape)) {
			_inv_open = false;
		} else if (im.pressed(sf::Keyboard::Tab)) {
			_inv_open = !_inv_open;
		}

		if (_inv_open) {
			int row = (sf::Mouse::getPosition().y - _inv_layout.top) / item_icon_size.y;
			int column = (sf::Mouse::getPosition().x - _inv_layout.left) / item_icon_size.x;
			int index = row * _inv_column_count + column;

			bool row_in_bounds = _inv_layout.top <= sf::Mouse::getPosition().y && row < _inv_row_count;
			bool column_in_bounds = _inv_layout.left <= sf::Mouse::getPosition().x && column < _inv_column_count;
			bool index_in_bounds = 0 <= index && index < static_cast<int>(_displayed_items.size());

			if (row_in_bounds && column_in_bounds && index_in_bounds) {
				for (std::size_t key_index = 0; key_index < 10; ++key_index) {
					if (im.pressed(input_manager::index_to_num_key(key_index))) {
						_hotbar[key_index] = _displayed_items[index];
					}
				}
			}
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
			auto use = [this](int idx) {
				_last_used_hotbar_idx = idx;

				//! @todo Sync the hotbar with changes to the inventory.
				if (std::optional<ent> o_item_id = _hotbar[idx]) {
					// Open list dialog for the player to choose an action.
					auto [action_names, actions] = get_item_options(*o_item_id);
					auto dialog = umake<list_dialog>(_window,
						_fonts,
						sf::Vector2f{sf::Mouse::getPosition()},
						item.name,
						std::move(action_names),
						[this, actions = std::move(actions)](std::optional<int> o_action_idx) {
							if (!o_action_idx) {
								// No action selected. Player must try to act again.
								return this->act();
							} else {
								// Perform the chosen action.
								int action_idx = *o_action_idx;
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
			};
			if (im.down(sf::Keyboard::X) || im.pressed({sf::Keyboard::Z, sf::Keyboard::Return})) {
				// Pass control by satisfying the current pass promise.
				_pass_promise.set_value();
				// Reassign the pass promise in preparation for next turn.
				_pass_promise = std::promise<void>{};
				return;
			} else if (im.pressed(sf::Keyboard::E)) {
				_cont(move{im.shift(), region_tile::direction::one});
				return;
			} else if (im.pressed(sf::Keyboard::W)) {
				_cont(move{im.shift(), region_tile::direction::two});
				return;
			} else if (im.pressed(sf::Keyboard::Q)) {
				_cont(move{im.shift(), region_tile::direction::three});
				return;
			} else if (im.pressed(sf::Keyboard::A)) {
				_cont(move{im.shift(), region_tile::direction::four});
				return;
			} else if (im.pressed(sf::Keyboard::S)) {
				_cont(move{im.shift(), region_tile::direction::five});
				return;
			} else if (im.pressed(sf::Keyboard::D)) {
				_cont(move{im.shift(), region_tile::direction::six});
				return;
			} else if (im.pressed(sf::Keyboard::Space)) {
				use(_last_used);
			} else if (im.pressed(sf::Keyboard::Num1)) {
				use(0);
			} else if (im.pressed(sf::Keyboard::Num2)) {
				use(1);
			} else if (im.pressed(sf::Keyboard::Num3)) {
				use(2);
			} else if (im.pressed(sf::Keyboard::Num4)) {
				use(3);
			} else if (im.pressed(sf::Keyboard::Num5)) {
				use(4);
			} else if (im.pressed(sf::Keyboard::Num6)) {
				use(5);
			} else if (im.pressed(sf::Keyboard::Num7)) {
				use(6);
			} else if (im.pressed(sf::Keyboard::Num8)) {
				use(7);
			} else if (im.pressed(sf::Keyboard::Num9)) {
				use(8);
			} else if (im.pressed(sf::Keyboard::Num0)) {
				use(9);
			}
		}
	}

	void hud::render_effect(effects::effect const& effect) {
		_world_widget.render_effect(effect);
	}

	void hud::set_player_being_id(ent player_id) {
		_player_id = player_id;
		_inv_page = 0;

		if (being const* player_being = the_game().beings.cptr(player_being_id)) {
			update_displayed_items(player_id);
		} else {
			std::fill(_hotbar.begin(), _hotbar.end(), std::nullopt);
		}
	}

	void hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (being const* player_being = the_game().beings.cptr(_player_being_id)) {
			constexpr int conditions_count = 2;
			constexpr int condition_bar_width = 10;
			constexpr int condition_bar_height = 100;

			constexpr int hotbar_interslot_gap = 2;
			constexpr int hotbar_bottom_gap = 2;
			constexpr int hotbar_slot_width = 65;
			constexpr int hotbar_slot_h_padding = 5;
			constexpr int hotbar_slot_v_padding = 5;

			// Draw condition bars.
			auto [body, cond, stats] = reg.get<ql::body, body_cond, ql::stats::being>(_player_id);
			int const screen_bottom = _window.getSize().y - 1;
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

			{ // Draw the body.
				body_renderer renderer;
				renderer.visit(player_being->body);
				uptr<texture> texture = texturer.texture();
				texture->draw(spaces::window::point{0, screen_bottom - condition_bar_height},
					spaces::window::align_left,
					spaces::window::align_bottom);
			}

			{ // Draw hotbar items.
				static auto const hotbar_slot_texture = umake<texture>("resources/textures/ui/hud/hotbar-slot.png");

				int const hotbar_width = hotbar_slot_texture->width() * _hotbar_size +
					hotbar_interslot_gap * (_hotbar_size - 1);
				int const hotbar_x_start = (the_window().width() - hotbar_width) / 2;

				for (int i = 0; i < _hotbar_size; ++i) {
					int x = hotbar_x_start + (hotbar_slot_width + hotbar_interslot_gap) * i;
					int y = screen_bottom - hotbar_bottom_gap;
					hotbar_slot_texture->draw(
						spaces::window::point{x, y}, spaces::window::align_left, spaces::window::align_bottom);
					if (std::optional<id<item>> o_item_id = _hotbar[i]) {
						item const& item = the_game().items.cref(*o_item_id);
						item_animator item_animator{spaces::window::point{x + hotbar_slot_h_padding,
							y - hotbar_slot_v_padding - 55}}; //! @todo Remove magic number or add alignment back
															  //! in. 55 is the height of item textures.
						item.accept(item_animator);
					}
				}
			}

			// Draw the inventory if it's open.
			if (_inv_open) {
				static constexpr float inv_width_pct = 0.7f;
				static constexpr float inv_height_pct = 0.7f;

				// Update the inventory layout if the window was resized.
				if (im.window_resized()) {
					// Calculate number of visible inventory rows and columns.
					_inv_row_count = std::max(1l, lround(inv_height_pct * the_window().getSize().y / item_icon_size.y));
					_inv_column_count = std::max(1l, lround(inv_width_pct * the_window().getSize().x / item_icon_size.x));

					_inv_layout = spaces::window::box{the_window().getSize() / 2,
						spaces::window::vector{_inv_column_count * item_icon_size.x, _inv_row_count * item_icon_size.y},
						{spaces::window::align_center, spaces::window::align_middle}};
				}

				the_renderer().draw_box(_inv_layout, 1, sf::Color::Black, sf::Color{128, 128, 128});

				{ // Draw selection.
					auto const mouse_pos = sf::Mouse::getPosition();
					int const row = (mouse_pos.y - _inv_layout.top) / item_icon_size.y;
					int const column = (x_mouse - _inv_layout.left) / item_icon_size.x;
					if (_inv_layout.top <= y_mouse && row < _inv_row_count && _inv_layout.left <= x_mouse &&
						column < _inv_column_count) {
						the_renderer().draw_box(
							spaces::window::box{spaces::window::point{_inv_layout.left + item_icon_size.x * column,
													_inv_layout.top + item_icon_size.y * row},
								item_icon_size},
							1,
							sf::Color::Black,
							sf::Color{192, 192, 192});
					}
				}

				// Draw item icons.
				for (size_t i = 0; i < _displayed_items.size(); ++i) {
					int row = i / _inv_column_count;
					int column = i % _inv_column_count;
					item_animator texturer{spaces::window::point{
						_inv_layout.left + column * item_icon_size.x, _inv_layout.top + row * item_icon_size.y}};
					_displayed_items[i].get().accept(texturer);
				}
			}
		}
	}

	void hud::update_displayed_items(ent player_id) {
		_displayed_items = {player_being.inventory.item_ids.begin(), player_being.inventory.item_ids.end()};
		//! @todo Support item sorting (alphabetical or custom).
		//! @todo Cache the displayed items (only need to regenerate list if a turn has been taken).
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
					result.emplace_back("Jab", [quarterstaff] { quarterstaff->jav(); });
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
}
