//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"
#include "view_space.hpp"

namespace ql {
	//! A UI element.
	struct widget : public sf::Drawable {
		enum class event_handled { yes, no };

		//! The offset of this widget's position from its parent's.
		view::vector local_offset{};

		widget(widget* parent) : _parent{parent} {}

		virtual ~widget() = default;

		widget* parent() const {
			return _parent;
		}

		//! The size of this widget's bounding box.
		virtual view::vector get_size() const = 0;

		//! Updates the widget. To be called once per frame.
		//! @param elapsed_time The amount of time since the last update.
		virtual void update(sec elapsed_time) = 0;

		//! This widget's absolute position in the window.
		view::point get_position() const {
			return (_parent ? _parent->get_position() : view::point{}) + local_offset;
		}

		//! The bounding box around this widget in the window.
		auto get_bounding_box() const {
			return view::box{get_position(), get_size()};
		}

		// Event Handlers

		virtual event_handled on_parent_resize(view::vector) {
			return event_handled::no;
		};
		virtual event_handled on_key_press(sf::Event::KeyEvent const&) {
			return event_handled::no;
		}
		virtual event_handled on_key_release(sf::Event::KeyEvent const&) {
			return event_handled::no;
		}
		virtual event_handled on_text_entry(sf::Uint32 unicode) {
			return event_handled::no;
		}
		virtual event_handled on_mouse_press(sf::Event::MouseButtonEvent const&) {
			return event_handled::no;
		}
		virtual event_handled on_mouse_release(sf::Event::MouseButtonEvent const&) {
			return event_handled::no;
		}
		virtual event_handled on_mouse_move(view::point) {
			return event_handled::no;
		}
		virtual event_handled on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const&) {
			return event_handled::no;
		}

	private:
		widget* _parent;
	};
}
