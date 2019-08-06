//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "label.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace ql {
	label::label(sf::String const& text, sf::Font const& font, unsigned size, sf::Color fill_color = sf::Color::Black) {
		_text.setString(text);
		_text.setFont(font);
		_text.setCharacterSize(size);
		_text.setFillColor(fill_color);
	}

	view::vector label::get_size() const {
		return _size;
	}

	auto label::update(sec) -> void {
		_text.setPosition(to_sfml(get_position()));
	}

	auto label::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(_text, states);
	}

	auto label::set_position(view::point position) -> void {
		_position = position;
	}

	auto label::get_position() const -> view::point {
		return _position;
	}

	auto label::set_text(sf::String const& text) -> void {
		_text.setString(text);
		update_size();
	}

	auto label::set_font(sf::Font const& font) -> void {
		_text.setFont(font);
		update_size();
	}

	auto label::set_font_size(unsigned font_size) -> void {
		_text.setCharacterSize(font_size);
		update_size();
	}

	auto label::set_fill_color(sf::Color const& fill_color) -> void {
		_text.setFillColor(fill_color);
	}

	auto label::set_outline_color(sf::Color const& outline_color) -> void {
		_text.setOutlineColor(outline_color);
	}

	auto label::set_outline_thickness(float thickness) -> void {
		_text.setOutlineThickness(thickness);
	}

	auto label::update_size() -> void {
		_size = view::vector_from_sfml_rect(_text.getLocalBounds());
	}
}
