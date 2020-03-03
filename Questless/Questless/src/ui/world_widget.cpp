//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "world_widget.hpp"

#include "animation/particles/arrow_particle.hpp"
#include "animation/particles/blood_particle.hpp"
#include "animation/particles/green_magic_particle.hpp"
#include "animation/particles/text_particle.hpp"
#include "animation/particles/yellow_magic_particle.hpp"
#include "animation/still_image.hpp"

#include "damage/damage.hpp"
#include "effects/effect.hpp"
#include "entities/beings/body_part.hpp"
#include "entities/beings/world_view.hpp"
#include "entities/entity.hpp"
#include "rsrc/fonts.hpp"
#include "utility/unreachable.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/region.hpp"
#include "world/tile.hpp"

#include <range/v3/action/remove_if.hpp>

namespace ql {
	using namespace vecx;
	using namespace vecx::literals;

	using namespace view::literals;

	world_widget::world_widget(reg& reg, rsrc::world_widget const& resources)
		: _reg{&reg}
		, _rsrc{resources}
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

	auto world_widget::get_size() const -> view::vector {
		return _size;
	}

	auto world_widget::update(sec elapsed_time) -> void {
		// Update tile widgets.
		for (auto& id_and_widget : _tile_widgets) {
			id_and_widget.second.update(elapsed_time);
		}

		// Update entity widgets.
		for (auto& id_and_widget : _entity_widgets) {
			id_and_widget.second.update(elapsed_time);
		}

		// Update effect animations.
		for (auto& ani : _effect_animations) {
			ani->update(elapsed_time);
		}
		// Remove stopped animations.
		ranges::actions::remove_if(_effect_animations, [](auto& ani) { return ani->stopped(); });

		{ // Camera controls.
			constexpr auto pan_rate = 10.0_px;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
				set_position(_position + view::vector{0.0_px, -pan_rate});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
				set_position(_position + view::vector{pan_rate, 0.0_px});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
				set_position(_position + view::vector{0.0_px, pan_rate});
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
				set_position(_position + view::vector{-pan_rate, 0.0_px});
			}
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

	auto world_widget::on_key_press(sf::Event::KeyEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto world_widget::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Button::Middle) {
			_o_drag_start = view::point_from_mouse_button_event(event);
			return event_handled::yes;
		} else {
			return event_handled::no;
		}
	}

	auto world_widget::on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Button::Middle) {
			_o_drag_start = std::nullopt;
			return event_handled::yes;
		} else {
			return event_handled::no;
		}
	}

	auto world_widget::on_mouse_move(view::point mouse_position) -> void {
		if (_o_drag_start) {
			// Pan camera.
			set_position(_position + (mouse_position - *_o_drag_start));
			_o_drag_start = mouse_position;
		}
	}

	auto world_widget::set_highlight_predicate(std::function<bool(tile_hex::point)> predicate) -> void {
		_highlight_predicate = std::move(predicate);
	}

	auto world_widget::clear_highlight_predicate() -> void {
		_highlight_predicate = std::nullopt;
	}

	auto world_widget::render_terrain(world_view const& view) -> void {
		_tile_widgets.clear();
		for (auto const& tv : view.tile_views) {
			auto& tile_widget = _tile_widgets.try_emplace(tv.id, *_reg, _rsrc.tile, tv).first->second;
			tile_widget.on_parent_resize(_size);
			tile_widget.set_position(tv.position);
		}
	}

	auto world_widget::render_entities(world_view const& view) -> void {
		using ev = world_view::entity_view;

		_entity_widgets.clear();
		// Create a list of entity IDs sorted by y-coordinate.
		std::vector<ev> sorted_entity_views = view.entity_views;
		std::sort(sorted_entity_views.begin(), sorted_entity_views.end(), [](ev const& ev1, ev const& ev2) {
			return ev1.position[1] < ev2.position[1];
		});
		// Render sorted entities.
		for (auto const& ev : sorted_entity_views) {
			auto& entity_widget = _entity_widgets.try_emplace(ev.id, *_reg, _rsrc.entity, _rsrc.particle, ev).first->second;
			entity_widget.on_parent_resize(_size);
			entity_widget.set_position(ev.position);
		}
	}

	auto world_widget::render_effect(effects::effect const& effect) -> void {
		match(
			effect.value,
			[&](effects::arrow_attack const& e) {
				view::point source = to_view_space(e.origin);
				view::point target = to_view_space(e.target);
				_effect_animations.push_back(umake<arrow_particle>(_rsrc.particle, source, target));
				_arrow_sound.play();
			},
			[&](effects::injury const& e) {
				body_part const* const target_part = _reg->try_get<body_part>(e.target_part_id);
				// Assume vitality = 100 if being no longer exists to check.
				auto const target_vitality = target_part ? target_part->stats.a.vitality.cur : 100_hp;
				//! @todo Pass along the vitality in the event object if it's needed here (to avoid having to make
				//! up a number).

				view::point const position = to_view_space(e.origin);

				dmg::group const& damage = e.damage;

				for (auto const& part : damage.parts) {
					static constexpr sec text_duration = 2.0_s;

					auto const& font = _rsrc.fonts.firamono;

					auto spawn_blood = [&](int const damage) {
						constexpr int scaling_factor = 20;
						int const n = damage * scaling_factor / target_vitality.value;
						for (int i = 0; i < n; ++i) {
							_effect_animations.push_back(umake<blood_particle>(_rsrc.particle));
						}
					};

					auto render_slash_or_pierce = [&](int const amount) {
						spawn_blood(amount);
						_effect_animations.push_back(umake<text_particle>(
							text_duration, font, std::to_string(amount), sf::Color::White, sf::Color::Black, 1.0f));
						_effect_animations.back()->setPosition(view::to_sfml(position));
						_pierce_sound.play();
					};

					auto render_cleave_or_bludgeon = [&](int const amount) {
						spawn_blood(amount);
						_effect_animations.push_back(umake<text_particle>(
							text_duration, font, std::to_string(amount), sf::Color::White, sf::Color::Black, 1.0f));
						_effect_animations.back()->setPosition(view::to_sfml(position));
						_hit_sound.play();
					};

					match(
						part,
						[&](dmg::slash const& slash) { render_slash_or_pierce(slash.value); },
						[&](dmg::pierce const& pierce) { render_slash_or_pierce(pierce.value); },
						[&](dmg::cleave const& cleave) { render_cleave_or_bludgeon(cleave.value); },
						[&](dmg::bludgeon const& bludgeon) { render_cleave_or_bludgeon(bludgeon.value); },
						[&](dmg::scorch const& scorch) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(scorch.value), sf::Color{255, 128, 0}, sf::Color::Black, 1.0f));
							_effect_animations.back()->setPosition(view::to_sfml(position));
						},
						[&](dmg::freeze const& freeze) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(freeze.value), sf::Color::Cyan, sf::Color::Black, 1.0f));
							_effect_animations.back()->setPosition(view::to_sfml(position));
						},
						[&](dmg::shock const& shock) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(shock.value), sf::Color::Yellow, sf::Color::Black, 1.0f));
							_effect_animations.back()->setPosition(view::to_sfml(position));
						},
						[&](dmg::poison const& poison) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(poison.value), sf::Color{128, 0, 192}, sf::Color::Black, 1.0f));
							_effect_animations.back()->setPosition(view::to_sfml(position));
						},
						[&](dmg::rot const& rot) {
							_effect_animations.push_back(umake<text_particle>(
								text_duration, font, std::to_string(rot.value), sf::Color{96, 96, 96}, sf::Color::White, 1.0f));
							_effect_animations.back()->setPosition(view::to_sfml(position));
						});
				}
			},
			[&](effects::lightning_bolt const& e) {
				view::point position = to_view_space(e.origin);
				for (int i = 0; i < 35; ++i) {
					auto p = umake<yellow_magic_particle>(_rsrc.particle);
					p->setPosition(to_sfml(position));
					_effect_animations.push_back(std::move(p));
				}
				_shock_sound.play();
			},
			[&](effects::telescope const& e) {
				view::point position = to_view_space(e.origin);
				for (int i = 0; i < 50; ++i) {
					auto particle = umake<green_magic_particle>(_rsrc.particle);
					particle->setPosition(to_sfml(position));
					_effect_animations.push_back(std::move(particle));
				}
				_telescope_sound.play();
			});
	}

	auto world_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		// Adjust states transform to account for position.
		states.transform.translate(view::to_sfml(_position));

		// Draw tiles.
		for (auto const& id_and_widget : _tile_widgets) {
			target.draw(id_and_widget.second, states);
		}
		// Draw entities.
		for (auto const& id_and_widget : _entity_widgets) {
			target.draw(id_and_widget.second, states);
		}
		// Draw effects.
		for (auto const& animation : _effect_animations) {
			target.draw(*animation, states);
		}
		{ // Draw axes.
			tile_hex::point origin{0_pace, 0_pace};
			sf::VertexArray q_array(sf::Lines);
			q_array.append(sf::Vertex(view::to_sfml(world_layout.to_view_space(origin)), sf::Color::Red));
			q_array.append(sf::Vertex(
				view::to_sfml(world_layout.to_view_space(origin + tile_hex::vector{3_pace, 0_pace})), sf::Color::Red));
			target.draw(q_array, states);
			sf::VertexArray r_array(sf::Lines);
			r_array.append(sf::Vertex(view::to_sfml(world_layout.to_view_space(origin)), sf::Color::Green));
			r_array.append(sf::Vertex(
				view::to_sfml(world_layout.to_view_space(origin + tile_hex::vector{0_pace, 3_pace})), sf::Color::Green));
			target.draw(r_array, states);
		}
	}
}
