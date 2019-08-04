//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "panel.hpp"

namespace ql {
	panel::panel() {}

	view::vector panel::get_size() const {
		return _size;
	}

	void panel::update(sec elapsed_time) {
		for (auto child_and_offet : children) {
			std::get<widget*>(child_and_offet)->update(elapsed_time);
		}
	}

	void panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto child_and_offet : children) {
			target.draw(*std::get<widget*>(child_and_offet), states);
		}
	}

	auto panel::set_position(view::point position) -> void {
		_position = position;
		for (auto [child, offset] : children) {
			child->set_position(_position + offset);
		}
	}

	auto panel::get_position() const -> view::point {
		return _position;
	}

	auto panel::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
		for (auto child_and_offet : children) {
			std::get<widget*>(child_and_offet)->on_parent_resize(_size);
		}
	}
}
