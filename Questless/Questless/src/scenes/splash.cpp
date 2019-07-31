//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "splash.hpp"

#include "main_menu.hpp"

#include "utility/random.hpp"

#include <algorithm>

namespace ql::scenes {
	using namespace view::literals;

	namespace {
		constexpr sec fade_out_duration = 2.0_s;
		constexpr sec fade_in_duration = 2.0_s;
		constexpr sec duration = fade_out_duration + fade_in_duration;
	}

	splash_widget::splash_widget(widget& parent) : widget{parent} {
		static constexpr int flame_count = 20;
		auto const window_size = parent.get_size();
		for (int i = 0; i < flame_count; ++i) {
			_flame_positions.emplace_back(cancel::unitless<float>{uniform(0.0f, 1.0f)},
				cancel::unitless<float>{static_cast<float>(i + 1) / flame_count});
		}
	}

	view::vector splash_widget::get_size() const {
		return parent()->get_size();
	}

	void splash_widget::update(sec elapsed_time, std::vector<sf::Event>& events) {
		// Play the splash sound effect on the first frame of the splash screen.
		if (!_sound_played) {
			_sound_played = true;
			_rsrc.sfx.flame.play();
		}

		// Move splash flames.
		auto const window_size = parent()->get_size();
		for (auto& position : _flame_positions) {
			constexpr auto flame_speed = 2800.0_px / 1.0_s;
			position[1] += flame_speed / window_size[1] * elapsed_time;
			if (position[1] > 1.0f) {
				position[0] = {uniform(0.0f, 1.0f)};
				position[1] = {0.0f};
			}
		}

		// End scene if a key or button is pressed.
		for (auto event : events) {
			switch (event.type) {
				case sf::Event::KeyPressed:
					_skip = true;
					break;
				case sf::Event::MouseButtonPressed:
					_skip = true;
					break;
				default:
					break;
			}
		}
		if (_skip) { _rsrc.sfx.flame.stop(); }
	}

	void splash_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Get the size of the parent widget.
		auto const parent_size = parent()->get_size();
		// Create logo sprite.
		sf::Sprite logo_sprite{_rsrc.txtr.logo};
		{ // Set logo position.
			constexpr auto max_jiggle = 3.0_px;
			view::vector const logo_jiggle{uniform(-max_jiggle, max_jiggle), uniform(-max_jiggle, max_jiggle)};
			auto const logo_position = view::point{} + parent_size / 2.0f + logo_jiggle;
			logo_sprite.setPosition(view::to_sfml(logo_position));
		}
		{ // Set logo origin to center.
			auto const logo_size = _rsrc.txtr.logo.getSize();
			logo_sprite.setOrigin(logo_size.x / 2.0f, logo_size.y / 2.0f);
		}
		// Draw logo.
		target.draw(logo_sprite, states);

		{ // Draw flames.
			sf::Sprite flame_sprite{_rsrc.txtr.flame};
			auto const flame_size = _rsrc.txtr.flame.getSize();
			for (auto position : _flame_positions) {
				// Set origin such that flames just go off-screen at position = 0 and position = 1.
				flame_sprite.setOrigin(flame_size.x / 2.0f, position[1].value * flame_size.y);
				// Set position based on the parent widget size, and then draw.
				flame_sprite.setPosition(to_sfml(vecx::component_wise_product(parent_size, position)));
				target.draw(flame_sprite, states);
			}
		}
	}

	splash::splash(view::vector window_size, rsrc::fonts const& fonts)
		: scene{fonts}
		, _root{_splash_widget, window_size}
		, _splash_widget{_root} //
	{
		_fade_shader.loadFromFile("resources/shaders/fade.frag", sf::Shader::Type::Fragment);
	}

	update_result splash::scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) {
		// Update UI.
		_root.update(elapsed_time, events);

		auto const time_in_state = get_time_in_state();

		// End scene if user skipped or if time has elapsed.
		if (_splash_widget.skip() || time_in_state >= duration) {
			return switch_scene{umake<main_menu>(_root.get_size(), fonts)};
		}

		// Set fade-in/fade-out intensity.
		auto const intensity = time_in_state < fade_in_duration //
			? time_in_state / fade_in_duration
			: 1.0f - (time_in_state - fade_in_duration) / fade_out_duration;
		_fade_shader.setUniform("intensity", intensity);

		return continue_scene{};
	}

	void splash::scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.shader = &_fade_shader;
		target.draw(_root, states);
	}
}
