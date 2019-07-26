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
	//! Retrieves region tile coordinates from the player.
	struct tile_dialog : dialog<region_tile::point> {
		tile_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::String title,
			sf::String prompt,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::point)> predicate);

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		std::optional<region_tile::point> _origin;
		std::function<bool(region_tile::point)> _predicate;

		label _title;
		label _prompt;
	};
}
