//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"
#include "view_space.hpp"

namespace ql {
	//! A UI element.
	struct widget : public sf::Drawable {
		//! Used to indicate whether an event has been handled and should no longer be propagated to other widgets.
		enum class [[nodiscard]] event_handled{yes, no};

		virtual ~widget() = default;

		//! The size of this widget's bounding box.
		virtual auto get_size() const -> view::vector = 0;

		//! Updates the widget. To be called once per frame.
		//! @param elapsed_time The amount of time since the last update.
		virtual auto update(sec elapsed_time) -> void = 0;

		//! Sets the absolute position in the window of this widget's top-left corner to @p position.
		virtual auto set_position(view::point position) -> void = 0;

		//! The absolute position in the window of this widget's top-left corner.
		virtual auto get_position() const -> view::point = 0;

		//! The bounding box around this widget in the window.
		auto get_bounding_box() const -> view::box;

		// Event Handlers

		//! Called when the parent widget is resized. A container widget should always call this initially to inform its
		//! children of its initial size.
		//! @param parent_size The new size of the parent widget.
		virtual auto on_parent_resize(view::vector parent_size) -> void;

		//! Called when a keyboard key is pressed down.
		//! @param event The key event that occurred.
		virtual auto on_key_press(sf::Event::KeyEvent const& event) -> event_handled;

		//! Called when a keyboard key is released.
		//! @param event The key event that occurred.
		virtual auto on_key_release(sf::Event::KeyEvent const& event) -> event_handled;

		//! Called when text is entered.
		//! @param unicode The Unicode character that was entered.
		virtual auto on_text_entry(sf::Uint32 unicode) -> event_handled;

		//! Called when a mouse button is pressed.
		//! @param event The mouse button event that occurred.
		virtual auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled;

		//! Called when a mouse button is released.
		//! @param event The mouse button event that occurred.
		virtual auto on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled;

		//! Called when the mouse moves.
		//! @param mouse_position The new position of the mouse.
		virtual auto on_mouse_move(view::point mouse_position) -> void;

		//! Called when the mouse wheel is scrolled.
		//! @param event The mouse wheel scroll event that occurred.
		virtual auto on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled;

		//! Called when the user requests to quit the program.
		virtual auto on_request_quit() -> event_handled;
	};
}
