//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include <SFML/Graphics/Text.hpp>

namespace ql {
	//! Displays text.
	struct label : widget {
		label(sf::String const& text, sf::Font const& font, unsigned size, sf::Color fill_color = sf::Color::Black);

		auto get_size() const -> view::vector final;

		auto update(sec) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto set_text(sf::String const& text) -> void;

		auto set_font(sf::Font const& font) -> void;

		auto set_font_size(unsigned font_size) -> void;

		auto set_fill_color(sf::Color const& fill_color) -> void;

		auto set_outline_color(sf::Color const& fill_color) -> void;

		auto set_outline_thickness(float thickness) -> void;

	private:
		sf::Text _text;
		view::point _position;
		view::vector _size;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		auto update_size() -> void;
	};
}
