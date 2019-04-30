//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <optional>
#include <string>

#include "dialog.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! Retrieves region tile coordinates from the player.
	struct tile_dialog : dialog {
		tile_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::String title,
			sf::String prompt,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::point)> predicate,
			std::function<void(std::optional<region_tile::point>)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		std::optional<region_tile::point> _origin;
		std::function<bool(region_tile::point)> _predicate;
		continuation<std::optional<region_tile::point>> _cont;

		sf::Text _title;
		sf::Text _prompt;
	};
}
