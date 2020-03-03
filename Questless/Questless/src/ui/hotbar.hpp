//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "item_widget.hpp"
#include "widget.hpp"

#include "reg.hpp"

#include <optional>

namespace ql {
	//! Holds items for ready use.
	struct hotbar : widget {
		hotbar(reg& reg, rsrc::item const& item_resources, rsrc::spell const& spell_resources);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_key_press(sf::Event::KeyEvent const&) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_move(view::point mouse_position) -> void final;

		//! Sets the item ID of the @p idx item widget to @p o_item_id.
		auto set_item(size_t idx, std::optional<id> o_item_id) -> void;

		//! Copies @p handler to the @p on_click handler for each of this hotbar's item widgets.
		auto set_on_click(std::function<void(std::optional<id>, view::point)> handler) -> void;

	private:
		rsrc::item const& _item_resources;
		rsrc::spell const& _spell_resources;

		view::point _position;
		view::point _mouse_position;

		std::array<item_widget, 10> _item_widgets;
		size_t _most_recent_idx = 0;

		sf::Texture _slot_texture;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		auto click(size_t idx) -> void;
	};
}
