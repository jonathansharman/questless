//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include <SFML/Window.hpp>

#include <string>
#include <vector>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! Retrieves the player's choice from a list of options.
	struct list_dialog : dialog<std::function<void()>> {
		//! @param options The list of pairs of option names and their callbacks. Must be non-empty.
		list_dialog(rsrc::fonts const& fonts, sf::String title, std::vector<std::tuple<sf::String, std::function<void()>>> options);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_key_press(sf::Event::KeyEvent const& event) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled final;

	private:
		static constexpr float _title_height = 40;
		static constexpr float _option_height = 20;

		view::point _position;

		int _selection = 0;

		sf::RectangleShape _bg;
		sf::Text _title;
		std::vector<std::tuple<sf::Text, std::function<void()>>> _options;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Changes the selection to @p selection.
		auto select(int selection) -> void;

		//! Chooses the currently selected option.
		auto choose() -> void;
	};
}
