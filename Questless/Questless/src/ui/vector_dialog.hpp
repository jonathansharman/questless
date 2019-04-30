//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include "world/coordinates.hpp"

#include <functional>
#include <optional>
#include <string>

namespace ql {
	//! Retrieves a region tile vector from the player.
	struct vector_dialog : dialog {
		vector_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::String const& title,
			sf::String const& prompt,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::vector)> predicate,
			std::function<void(std::optional<region_tile::vector>)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		std::optional<region_tile::point> _origin;
		std::function<bool(region_tile::vector)> _predicate;
		continuation<std::optional<region_tile::vector>> _cont;

		sf::Text _title;
		sf::Text _prompt;
	};
}
