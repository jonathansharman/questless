//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <optional>

namespace ql {
	namespace rsrc {
		struct item;
		struct spell;
	}
	struct animation;

	//! Allows interaction with an item.
	struct item_widget : widget {
		//! Item widgets have a fixed size.
		static constexpr view::vector size{view::px{55.0f}, view::px{55.0f}};

		//! Invoked when this widget is clicked, passing the item's ID, if any, and the mouse click coordinates.
		std::function<void(std::optional<id>, view::point)> on_click;

		item_widget(rsrc::item const& item_resources, rsrc::spell const& spell_resources);

		~item_widget();

		//! The ID of the item this widget currently interfaces with, if any.
		auto get_o_item_id() const -> std::optional<id>;

		//! Sets this widget's item ID to @p item_id.
		auto set_o_item_id(std::optional<id> o_item_id) -> void;

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

	private:
		rsrc::item const& _item_resources;
		rsrc::spell const& _spell_resources;

		std::optional<id> _o_item_id;
		uptr<animation> _ani;

		view::point _position;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
