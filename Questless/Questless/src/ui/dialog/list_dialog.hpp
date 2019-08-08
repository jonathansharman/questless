//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include "ui/label.hpp"

#include <vector>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! Retrieves the player's choice from a list of options.
	struct list_dialog : dialog<std::function<void()>> {
		//! @param options The list of pairs of option names and their callbacks. Must be non-empty.
		list_dialog( //
			widget* parent,
			rsrc::fonts const& fonts,
			sf::String const& title,
			std::vector<std::tuple<sf::String, std::function<void()>>> options);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_key_press(sf::Event::KeyEvent const& event) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled final;

	private:
		widget* _parent;

		view::point _position;
		view::vector _size;
		view::vector _parent_size;

		int _selection = 0;

		sf::RectangleShape _bg;
		label _title;
		std::vector<std::tuple<label, std::function<void()>>> _options;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Changes the selection to @p selection.
		auto select(int selection) -> void;

		//! Chooses the currently selected option.
		auto choose() -> void;

		//! Repositions the list dialog so that it fits inside its parent.
		auto confine_to_parent() -> void;
	};
}
