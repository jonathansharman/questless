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
	struct list_dialog : dialog<std::tuple<sf::String, std::function<void()>>> {
		list_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::Vector2f origin,
			sf::String title,
			std::vector<std::tuple<sf::String, std::function<void()>>> options);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		static constexpr float _title_height = 40;
		static constexpr float _option_height = 20;

		int _selection = 0;

		sf::RectangleShape _bg;
		sf::Text _title;
		std::vector<std::tuple<sf::Text, std::function<void()>>> _options;
	};
}
