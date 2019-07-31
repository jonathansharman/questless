//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "label.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace ql {
	view::vector label::get_size() const {
		return view::vector_from_sfml_rect(text.getLocalBounds());
	}

	void label::update(sec, std::vector<sf::Event>&) {
		text.setPosition(to_sfml(get_position()));
	}

	void label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(text, states);
	}
}
