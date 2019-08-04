//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <optional>

namespace ql {
	struct animation;

	//! Allows interaction with an item.
	struct item_widget : widget {
		//! Item widgets have a fixed size.
		static constexpr view::vector size{view::px{55.0f}, view::px{55.0f}};

		//! The ID of the item this widget currently interfaces with, if any.
		std::optional<id> o_item_id;

		//! Invoked when this widget is clicked, passing the item's ID, if any.
		std::function<void(std::optional<id>)> on_click;

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

	private:
		uptr<animation> _ani;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
