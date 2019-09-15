//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "view_space.hpp"

#include "utility/reference.hpp"

#include "rsrc/splash.hpp"

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! The splash screen.
	struct splash : widget {
		//! @param root A reference to the root UI element of the game, used to change scenes when the splash screen ends.
		splash(uptr<widget>& root, rsrc::fonts const& fonts);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector) -> void final;

		auto on_key_press(sf::Event::KeyEvent const&) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const&) -> event_handled final;

		auto on_request_quit() -> event_handled final;

	private:
		clock::time_point _start_time = clock::now();

		uptr<widget>& _root;
		rsrc::fonts const& _fonts;

		rsrc::splash _rsrc;
		sf::Shader _fade_shader;
		view::point _position;
		view::vector _size;

		bool _sound_played = false;
		sf::Sound _flame_sound;

		std::vector<vecx::vector<cancel::unitless<float>, 2>> _flame_positions{};

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		auto end_scene() -> void;
	};
}
