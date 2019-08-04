//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "label.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace ql {
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

	auto label::set_text(sf::Text text) -> void {
		_text = std::move(text);
		_size = view::vector_from_sfml_rect(_text.getLocalBounds());
	}
}
