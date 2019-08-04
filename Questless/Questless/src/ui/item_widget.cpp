//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "item_widget.hpp"

#include "animation/animation.hpp"

namespace ql {
	using namespace view::literals;

	auto item_widget::get_size() const -> view::vector {
		return size;
	}

	auto item_widget::update(sec elapsed_time) -> void {
		_ani->update(elapsed_time);
	}

	auto item_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(*_ani, states);
	}

	auto item_widget::set_position(view::point position) -> void {
		_ani->setPosition(to_sfml(position));
	}

	auto item_widget::get_position() const -> view::point {
		return view::point_from_sfml(_ani->getPosition());
	}

	auto item_widget::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (o_item_id && get_bounding_box().contains(view::point_from_mouse_button_event(event))) {
			on_click(o_item_id);
			return event_handled::yes;
		}
		return event_handled::no;
	}
}
