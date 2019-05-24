//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "splash.hpp"

#include "main_menu.hpp"

#include "ui/input_manager.hpp"
#include "utility/random.hpp"

#include <algorithm>

namespace ql::scene {
	using namespace view::literals;

	namespace {
		constexpr sec fade_out_duration = 2.0_s;
		constexpr sec fade_in_duration = 2.0_s;
		constexpr sec duration = fade_out_duration + fade_in_duration;
	}

	splash::splash(sf::Window& window, rsrc::fonts const& fonts) : scene{window, fonts} {
		static constexpr int flame_count = 20;
		for (int i = 0; i < flame_count; ++i) {
			view::px const x = uniform(0.0_px, view::px{_window.getSize().x - 1});
			view::px const y{(i + 1) * _window.getSize().y / flame_count};
			_flame_positions.push_back({x, y});
		}
	}

	update_result splash::scene_subupdate(sec elapsed_time, input_manager& im) {
		// Play the splash sound effect on the first frame of the splash screen.
		if (!_sound_played) {
			_sound_played = true;
			_rsrc.sfx.flame.play();
		}

		// Move splash flames.
		for (auto& position : _flame_positions) {
			constexpr auto splash_flames_vy = -2800.0_px / 1.0_s;
			position[1] += splash_flames_vy * elapsed_time;
			if (position[1] < 0.0_px) {
				position[1] += view::px{_window.getSize().y + _rsrc.txtr.flame.getSize().y};
				position[0] = uniform(0.0_px, view::px{_window.getSize().x - 1});
			}
		}

		if (im.any_press_count() || to_sec(clock::now() - start_time()) >= duration) {
			_rsrc.sfx.flame.stop();
			return switch_scene{umake<main_menu>()};
		};
	}

	void splash::scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Get intensity, used for fading in and out.
		auto const intensity = to_uint8([&]() -> double {
			if (to_sec(clock::now() - start_time()) < fade_in_duration) {
				auto time_fading_in = to_sec(clock::now() - start_time());
				return time_fading_in / fade_in_duration;
			} else {
				auto time_fading_out = (to_sec(clock::now() - start_time()) - fade_in_duration);
				return 1.0 - time_fading_out / fade_out_duration;
			}
		}());

		// Create logo sprite.
		sf::Sprite logo_sprite{_rsrc.txtr.logo};
		{ // Set logo position.
			constexpr auto max_jiggle = 3.0_px;
			view::vector const logo_jiggle{uniform(-max_jiggle, max_jiggle), uniform(-max_jiggle, max_jiggle)};
			auto const logo_position = view::point{} + view::vector_from_sfml(_window.getSize()) / 2.0f + logo_jiggle;
			logo_sprite.setPosition(view::to_sfml(logo_position));
		}
		{ // Set logo origin to center.
			auto const logo_size = _rsrc.txtr.logo.getSize();
			logo_sprite.setOrigin(logo_size.x / 2, logo_size.y / 2);
		}
		{ // Set logo color.
			logo_sprite.setColor(sf::Color(intensity, intensity, intensity, 1.0));
		}
		// Draw logo.
		target.draw(logo_sprite, states);

		// Create flame sprite.
		sf::Sprite flame_sprite{_rsrc.txtr.flame};
		{ // Set flame origin to bottom-center.
			auto const flame_size = _rsrc.txtr.flame.getSize();
			flame_sprite.setOrigin(flame_size.x / 2, flame_size.y);
		}
		{ // Set flame color.
			flame_sprite.setColor(sf::Color(intensity, intensity, intensity, 1.0));
		}
		for (auto position : _flame_positions) {
			// Position and draw each flame.
			flame_sprite.setPosition(to_sfml(position));
			target.draw(flame_sprite, states);
		}
	}
}
