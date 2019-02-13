//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "dialog.hpp"

#include "rsrc/fonts.hpp"

#include "media/spaces/window.hpp"

namespace ql {
	sf::Text dialog::make_title(sf::String const& title, sf::Color fill_color, sf::Color outline_color) {
		sf::Text text{title, _fonts.dumbledor1, 32};
		text.setFillColor(fill_color);
		text.setOutlineColor(outline_color);
		text.setOutlineThickness(1);
		text.setOrigin({text.getLocalBounds().width / 2, 0});
		text.setPosition({_window.getSize().x / 2, 0});
		return text;
	}

	sf::Text dialog::make_prompt(sf::String const& prompt, sf::Color fill_color, sf::Color outline_color) {
		sf::Text text{prompt, _fonts.dumbledor1, 20};
		text.setFillColor(fill_color);
		text.setOutlineColor(outline_color);
		text.setOutlineThickness(1);
		text.setOrigin({text.getLocalBounds().width / 2, 0});
		text.setPosition({_window.getSize().x / 2, _prompt_top});
		return text;
	}

	sf::Text dialog::make_selector(sf::String const& selector, sf::Color fill_color, sf::Color outline_color) {
		sf::Text text{selector, _fonts.dumbledor1, 32};
		text.setFillColor(fill_color);
		text.setOutlineColor(outline_color);
		text.setOutlineThickness(1);
		text.setOrigin({text.getLocalBounds().width / 2, 0});
		text.setPosition({_window.getSize().x / 2, _selector_top});
		return text;
	}
}
