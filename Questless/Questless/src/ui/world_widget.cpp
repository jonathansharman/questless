//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world_widget.hpp"

#include "animation/animators.hpp"
#include "animation/particles/arrow_particle.hpp"
#include "animation/particles/blood_particle.hpp"
#include "animation/particles/green_magic_particle.hpp"
#include "animation/particles/text_particle.hpp"
#include "animation/particles/yellow_magic_particle.hpp"
#include "animation/still_image.hpp"

#include "damage/damage.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/world_view.hpp"
#include "entities/entity.hpp"
#include "rsrc/fonts.hpp"
#include "utility/unreachable.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/region.hpp"
#include "world/tile.hpp"

namespace ql {
	using namespace vecx;
	using namespace vecx::literals;

	using namespace view::literals;

	world_widget::world_widget(rsrc::world_widget const& resources)
		: _rsrc{resources}
		, _arrow_sound{_rsrc.sfx.arrow}
		, _hit_sound{_rsrc.sfx.hit}
		, _pierce_sound{_rsrc.sfx.pierce}
		, _shock_sound{_rsrc.sfx.shock}
		, _telescope_sound{_rsrc.sfx.telescope} //
	{}

	void world_widget::render_view(world_view const& view) {
		render_terrain(view);
		render_entities(view);
	}

	view::vector world_widget::get_size() const {
		return _size;
	}

	void world_widget::update(sec elapsed_time) {
		// Update tile widgets.
		for (auto& id_and_widget : _tile_widgets) {
			id_and_widget.second.update(elapsed_time);
		}

		// Update entity widgets.
		for (auto& id_and_widget : _entity_widgets) {
			id_and_widget.second.update(elapsed_time);
		}

		// Update effect animations.
		for (std::size_t i = 0; i < _effect_animations.size();) {
			_effect_animations[i]->update(elapsed_time);
			if (_effect_animations[i]->stopped()) {
				_effect_animations.erase(_effect_animations.begin() + i);
			} else {
				++i;
			}
		}

		{ // Camera controls.
			constexpr auto pan_rate = 10.0_px;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) { _pan += view::vector{0.0_px, pan_rate} / _zoom; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) { _pan += view::vector{-pan_rate, 0.0_px} / _zoom; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) { _pan += view::vector{0.0_px, -pan_rate} / _zoom; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) { _pan += view::vector{pan_rate, 0.0_px} / _zoom; }

			// Apply transforms to view.
			_view.setViewport({0.0f, 0.0f, 1.0f, 1.0f});
			_view.setCenter(to_sfml(_pan));
			_view.zoom(_zoom);
		}
	}

	void world_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_terrain(target, states);
		draw_entities(target, states);
		draw_effects(target, states);
	}

	void world_widget::draw_terrain(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& id_and_widget : _tile_widgets) {
			target.draw(id_and_widget.second, states);
		}
	}

	void world_widget::draw_entities(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& id_and_widget : _entity_widgets) {
			target.draw(id_and_widget.second, states);
		}
	}

	void world_widget::draw_effects(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& animation : _effect_animations) {
			target.draw(*animation, states);
		}
	}

	auto world_widget::set_position(view::point position) -> void {
		_position = position;
	}

	auto world_widget::get_position() const -> view::point {
		return _position;
	}

	auto world_widget::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
	}

	auto world_widget::on_key_press(sf::Event::KeyEvent const&) -> event_handled {}

	auto world_widget::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Button::Middle) { _o_drag_start = view::point_from_mouse_button_event(event); }
	}

	auto world_widget::on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Button::Middle) { _o_drag_start = std::nullopt; }
	}

	auto world_widget::on_mouse_move(view::point mouse_position) -> void {
		if (_o_drag_start) {
			// Pan camera.
			_pan += mouse_position - *_o_drag_start;
			_o_drag_start = mouse_position;
		}
	}

	auto world_widget::on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled {
		// Zoom camera.
		_zoom += event.y / 10.0f;
	}

	void world_widget::set_highlight_predicate(std::function<bool(region_tile::point)> predicate) {
		_highlight_predicate = std::move(predicate);
	}

	void world_widget::clear_highlight_predicate() {
		_highlight_predicate = std::nullopt;
	}

	void world_widget::render_terrain(world_view const& view) {
		_tile_widgets.clear();
		for (auto const& section_view : view.section_views) {
			auto const& region = reg.get<ql::region>(view.center.region_id);
			section const* section = region.section_at(section_view.coords);
			if (!section) continue;
			for (span q = 0_span; q < section_diameter; ++q) {
				for (span r = 0_span; r < section_diameter; ++r) {
					section_tile::point const section_tile_coords{q, r};

					perception tile_perception =
						section_view.tile_perceptions[section_tile_coords.q.value][section_tile_coords.r.value];

					if (tile_perception > 0_perception) {
						//! @todo Use tile_game_point or remove these lines if no longer needed.
						region_tile::point const region_tile_coords = section->region_tile_coords(section_tile_coords);
						world::point const tile_game_point = to_world(region_tile_coords);

						// Get the current tile.
						id tile_id = section->tile_id_at(section_tile_coords);

						auto& tile_widget = _tile_widgets[tile_id];
						tile_widget.o_tile_id = tile_id;
						tile_widget.render();
						tile_widget.on_parent_resize(_size);
						tile_widget.set_position(tile_game_point);

						//! @todo Use some kind of shader to indicate perception level.
					}
				}
			}
		}
	}

	void world_widget::render_entities(world_view const& view) {
		// Draw from the sorted view.
		for (auto const& entity_view : sorted_entity_views) {
			// Attempt to load the entity.
			if (entity const* entity = get_entity_cptr(entity_view.id)) {
				auto& entity_animation = [&]() -> animation& {
					// Search for the entity's animation in the cache.
					auto it = _entity_animations.find(entity_view.id);
					// If it's there, use it. Otherwise, create and cache the animation.
					if (it != _entity_animations.end()) {
						return *(it->second);
					} else {
						auto result = _entity_animations.insert({entity_view.id, animate(_entity_resources, *entity)});
						return *result.first->second;
					}
				}();

				auto const perception_above_minimum = entity_view.perception.value() - perception::minimum_level;
				auto const perception_range = perception::maximum_level - perception::minimum_level;
				sf::Uint8 const intensity = to_uint8(perception_above_minimum / perception_range);

				switch (get_category(entity_view.perception)) {
					case perception_category::none:
						break;
					case perception::category::low:
						_entity_resources.ani.unknown.setPosition(to_sfml(to_world(entity->location().coords)));
						_entity_resources.ani.unknown.set_color({intensity, intensity, intensity});
						break;
					case perception::category::medium:
					case perception::category::high:
					case perception::category::full: {
						entity_animation.setPosition(to_sfml(to_world(entity->location().coords)));
						//! @todo: Incorporate color.
						// entity_animation.setColor(sf::Color{intensity, intensity, intensity});
						break;
					}
					default:
						UNREACHABLE;
				}
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_entity_animations.erase(entity_view.id);
			}
		}
	}

	void world_widget::render_effect(effects::effect const& effect) {
		match(
			effect.value,
			[&](effects::arrow_attack const& e) {
				world::point source = to_world(e.origin);
				world::point target = to_world(e.target);
				_effect_animations.push_back(umake<arrow_particle>(source, target));
				_arrow_sound.play();
			},
			[&](effects::injury const& e) {
				being const* const target_being = the_game().beings.cptr(e.target_being_id);
				body_part const* const target_part = target_being ? target_being->body.find_part(e.target_part_id) : nullptr;
				// Assume vitality = 100 if being no longer exists to check.
				auto const target_vitality = target_part ? target_part->stats.vitality.value() : 100.0_hp;
				//! @todo Pass along the vitality in the event object if it's needed here (to avoid having to make
				//! up a number).

				world::point const position = to_world(e.origin);

				dmg::group const& damage = e.damage;

				for (auto const& part : damage.parts) {
					static constexpr sec text_duration = 2.0_s;

					auto const& font = _rsrc.fonts.firamono;

					auto spawn_blood = [&](double const damage) {
						constexpr double scaling_factor = 20.0;
						int const n = static_cast<int>(damage / target_vitality.value * scaling_factor);
						for (int i = 0; i < n; ++i) {
							_effect_animations.push_back(umake<blood_particle>());
						}
					};

					auto render_slash_or_pierce = [&](double const amount) {
						spawn_blood(amount);
						_effect_animations.push_back(
							umake<text_particle>(text_duration, font, std::to_string(lround(amount)), sf::Color::White));
						_effect_animations.back()->setPosition(to_sfml(position));
						_resources.sfx.pierce.play();
					};

					auto render_cleave_or_bludgeon = [&](double const amount) {
						spawn_blood(amount);
						_effect_animations.push_back(
							umake<text_particle>(text_duration, font, std::to_string(lround(amount)), sf::Color::White));
						_effect_animations.back()->setPosition(to_sfml(position));
						_resources.sfx.hit.play();
					};

					match(
						part,
						[&](dmg::slash const& slash) { render_slash_or_pierce(slash.value); },
						[&](dmg::pierce const& pierce) { render_slash_or_pierce(pierce.value); },
						[&](dmg::cleave const& cleave) { render_cleave_or_bludgeon(cleave.value); },
						[&](dmg::bludgeon const& bludgeon) { render_cleave_or_bludgeon(bludgeon.value); },
						[&](dmg::scorch const& burn) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std ::to_string(lround(burn.value)), sf::Color{255, 128, 0}));
							_effect_animations.back()->setPosition(to_sfml(position));
						},
						[&](dmg::freeze const& freeze) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(lround(freeze.value)), sf::Color::Cyan));
							_effect_animations.back()->setPosition(to_sfml(position));
						},
						[&](dmg::poison const& poison) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(lround(poison.value)), sf::Color{128, 0, 192}));
							_effect_animations.back()->setPosition(to_sfml(position));
						},
						[&](dmg::shock const& shock) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(lround(shock.value)), sf::Color::Yellow));
							_effect_animations.back()->setPosition(to_sfml(position));
						});
				}
			},
			[&](effects::lightning_bolt const& e) {
				world::point position = to_world(e.origin);
				for (int i = 0; i < 35; ++i) {
					auto p = umake<yellow_magic_particle>();
					p->setPosition(to_sfml(position));
					_effect_animations.push_back(std::move(p));
				}
				_shock_sound.play();
			},
			[&](effects::telescope const& e) {
				world::point position = to_world(e.origin);
				for (int i = 0; i < 50; ++i) {
					auto particle = umake<green_magic_particle>(_rsrc.particle);
					particle->setPosition(to_sfml(position));
					_effect_animations.push_back(std::move(particle));
				}
				_telescope_sound.play();
			});
	}
}
