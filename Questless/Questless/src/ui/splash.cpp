//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "splash.hpp"

#include "main_menu.hpp"

#include "utility/random.hpp"

#include <algorithm>

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr sec fade_out_duration = 2.0_s;
		constexpr sec fade_in_duration = 2.0_s;
		constexpr sec duration = fade_out_duration + fade_in_duration;
	}

	splash::splash(uptr<widget>& root, rsrc::fonts const& fonts)
		: _root{root}
		, _fonts{fonts}
		, _flame_sound{_rsrc.sfx.flame} //
	{
		_fade_shader.loadFromFile("resources/shaders/fade.frag", sf::Shader::Type::Fragment);
		_fade_shader.setUniform("texture", sf::Shader::CurrentTexture);

		constexpr int flame_count = 20;
		for (int i = 0; i < flame_count; ++i) {
			_flame_positions.emplace_back(cancel::unitless<float>{uniform(0.0f, 1.0f)},
				cancel::unitless<float>{static_cast<float>(i + 1) / flame_count});
		}
	}

	auto splash::get_size() const -> view::vector {
		return _size;
	}

	auto splash::update(sec elapsed_time) -> void {
		static int n_updates = 0;
		++n_updates;

		// Play the splash sound effect on the first frame of the splash screen.
		if (!_sound_played) {
			_sound_played = true;
			_flame_sound.play();
		}

		// Get total time spent in the splash screen.
		sec const time_in_splash = to_sec(clock::now() - _start_time);

		// End scene if time has elapsed.
		if (time_in_splash >= duration) {
			end_scene();
			return;
		}

		// Move splash flames.
		for (auto& position : _flame_positions) {
			constexpr auto flame_speed = 1.6_hz;
			position[1] -= flame_speed * elapsed_time;
			if (position[1] < 0.0f) {
				position[0] = cancel::unitless<float>{uniform(0.0f, 1.0f)};
				position[1] += cancel::unitless<float>{1.0f};
			}
		}

		// Set fade-in/fade-out intensity.
		auto const intensity = time_in_splash < fade_in_duration //
			? time_in_splash / fade_in_duration
			: 1.0f - (time_in_splash - fade_in_duration) / fade_out_duration;
		_fade_shader.setUniform("intensity", intensity);
	}

	auto splash::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		// Activate the fade shader.
		states.shader = &_fade_shader;

		{ // Draw logo.
			sf::Sprite logo_sprite{_rsrc.txtr.logo};
			// Set logo position.
			constexpr auto max_jiggle = 3.0_px;
			view::vector const logo_jiggle{uniform(-max_jiggle, max_jiggle), uniform(-max_jiggle, max_jiggle)};
			auto const logo_position = get_position() + _size / 2.0f + logo_jiggle;
			logo_sprite.setPosition(view::to_sfml(logo_position));
			// Set logo origin to center.
			auto const logo_size = _rsrc.txtr.logo.getSize();
			logo_sprite.setOrigin(logo_size.x / 2.0f, logo_size.y / 2.0f);
			target.draw(logo_sprite, states);
		}

		{ // Draw flames.
			sf::Sprite flame_sprite{_rsrc.txtr.flame};
			auto const flame_size = _rsrc.txtr.flame.getSize();
			for (auto position : _flame_positions) {
				// Set origin such that flames just go off-screen at position = 0 and position = 1.
				flame_sprite.setOrigin(flame_size.x / 2.0f, (1.0f - position[1].value) * flame_size.y);
				// Set position based on current size and draw.
				flame_sprite.setPosition(to_sfml(vecx::component_wise_product(_size, position)));
				target.draw(flame_sprite, states);
			}
		}
	}

	auto splash::set_position(view::point position) -> void {
		_position = position;
	}

	//! This widget's absolute position in the window.
	auto splash::get_position() const -> view::point {
		return _position;
	}

	auto splash::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
	}

	auto splash::on_key_press(sf::Event::KeyEvent const&) -> event_handled {
		// End scene if any key is pressed.
		end_scene();
		return event_handled::yes;
	}

	auto splash::on_mouse_press(sf::Event::MouseButtonEvent const&) -> event_handled {
		// End scene if any mouse button is pressed.
		end_scene();
		return event_handled::yes;
	}

	auto splash::end_scene() -> void {
		_flame_sound.stop();
		auto main_menu = umake<ql::main_menu>(_root, _fonts);
		// Initialize size and position.
		main_menu->on_parent_resize(_size);
		main_menu->set_position(_position);
		// Switch to main menu.
		_root = std::move(main_menu);
	}
}
