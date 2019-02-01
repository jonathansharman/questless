//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "media/spaces/window.hpp"

#include <SFML/Window.hpp>

#include <optional>
#include <set>
#include <stdexcept>
#include <variant>
#include <vector>

namespace ql {
	//! A keyboard key or a mouse button.
	using input = std::variant<sf::Keyboard::Key, sf::Mouse::Button>;

	//! Handles mouse and keyboard input as well as other events.
	class input_manager {
	public:
		//! Represents the press or release of a keyboard key or mouse button.
		struct input_event {
			input input;
			bool pressed;

			constexpr bool operator ==(input_event const& other) const {
				return this->input == other.input && this->pressed == other.pressed;
			}
			constexpr bool operator !=(input_event const& other) const {
				return !(*this == other);
			}
		};

		//! The window whose events this input manager polls.
		sf::Window& window;

		//! @param window The window whose events this input manager polls.
		input_manager(sf::Window& window) : window{window} {}

		//! The number key corresponding to the given index, with the number keys ordered 1 to 9 and then 0.
		static sf::Keyboard::Key index_to_num_key(size_t index);

		//! Updates the input state and handles events.
		void update();

		//! Whether the window was resized during this frame.
		bool window_resized() const { return _window_resized; }

		//! Whether a quit event has been received.
		bool quit() const { return _quit; }

		//! Whether @p input is currently up.
		bool up(input input) const;

		//! Whether @p input is currently down.
		bool down(input input) const;

		//! Whether any of @p inputs are currently up.
		bool up(std::set<input> inputs) const;

		//! Whether any of @p inputs are currently down.
		bool down(std::set<input> inputs) const;

		//! Whether @p input was pressed/clicked during this frame.
		bool pressed(input input) const;

		//! Whether @p input was released during this frame.
		bool released(input input) const;

		//! Whether any of @p inputs were pressed/clicked during this frame.
		bool pressed(std::set<input> inputs) const;

		//! Whether any of @p inputs were released during this frame.
		bool released(std::set<input> inputs) const;

		//! The number of times the given key was pressed during this frame.
		int press_count(input input) const;

		//! The number of times the given key was released during this frame.
		int release_count(input input) const;

		//! The number of times any of @p inputs were pressed during this frame.
		bool press_count(std::set<input> inputs) const;

		//! The number of times any of @p inputs were released during this frame.
		bool release_count(std::set<input> inputs) const;

		//! Whether any key has been pressed this frame.
		//! @param include_mouse_buttons Whether to count mouse button presses.
		bool any_pressed(bool include_mouse_buttons = true) const;

		//! Whether any key has been released this frame.
		//! @param include_mouse_buttons Whether to count mouse button releases.
		bool any_released(bool include_mouse_buttons = true) const;

		//! The number of times any key has been pressed this frame.
		//! @param include_mouse_buttons Whether to count mouse button presses.
		int any_press_count(bool include_mouse_buttons = true) const;

		//! The number of times any key has been released this frame.
		//! @param include_mouse_buttons Whether to count mouse button releases.
		int any_release_count(bool include_mouse_buttons = true) const;

		//! Whether either shift key is currently held.
		bool shift() const;

		//! Whether either control key is currently held.
		bool ctrl() const;

		//! Whether either alt key is currently held.
		bool alt() const;

		//! The offset the mouse has moved since the last frame.
		sf::Vector2i mouse_movement() const { return _mouse_movement; }

		//! The number of ticks the mouse wheel has been scrolled up since the last call to update. Negative value indicates scroll down.
		int scroll() const { return _scroll; }

		//! Sets the position of the mouse cursor in the window.
		//! @param position The position to which the cursor is moved.
		void move_mouse(sf::Vector2i const& position);
	private:
		std::vector<input_event> _input_event_buffer{};
		int _scroll = 0;
		sf::Vector2i _mouse_movement{0, 0};
		bool _window_resized = false;
		bool _quit = false;
	};
}
