//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	auto widget::get_bounding_box() const -> view::box {
		return {get_position(), get_size()};
	}

	auto widget::on_parent_resize(view::vector) -> void {}

	auto widget::on_key_press(sf::Event::KeyEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_key_release(sf::Event::KeyEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_text_entry(sf::Uint32) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_mouse_press(sf::Event::MouseButtonEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_mouse_release(sf::Event::MouseButtonEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_mouse_move(view::point) -> void {}

	auto widget::on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const&) -> event_handled {
		return event_handled::no;
	}

	auto widget::on_request_quit() -> event_handled {
		return event_handled::no;
	}
}
