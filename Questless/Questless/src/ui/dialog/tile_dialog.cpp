//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "tile_dialog.hpp"

namespace ql {
	tile_dialog::tile_dialog(sf::Window const& window,
		fonts const& fonts,
		sf::String const& title,
		sf::String const& prompt,
		std::optional<region_tile::point> origin,
		std::function<bool(region_tile::point)> predicate)
		: dialog{window, fonts}
		, _origin{std::move(origin)}
		, _predicate{std::move(predicate)}
		, _cont{std::move(cont)}
		, _title{make_title(title)}
		, _prompt{make_prompt(prompt)} {}

	void tile_dialog::update(sec elapsed_time, std::vector<sf::Event>& events) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			the_game().world_renderer().clear_highlight_predicate();
			_cont(std::nullopt);
			close();
			return;
		}

		std::optional<region_tile::point> result;

		if (_origin) {
			if (im.pressed(sf::Keyboard::E)) {
				result = _origin->neighbor(region_tile::direction::one);
			} else if (im.pressed(sf::Keyboard::W)) {
				result = _origin->neighbor(region_tile::direction::two);
			} else if (im.pressed(sf::Keyboard::Q)) {
				result = _origin->neighbor(region_tile::direction::three);
			} else if (im.pressed(sf::Keyboard::A)) {
				result = _origin->neighbor(region_tile::direction::four);
			} else if (im.pressed(sf::Keyboard::S)) {
				result = _origin->neighbor(region_tile::direction::five);
			} else if (im.pressed(sf::Keyboard::D)) {
				result = _origin->neighbor(region_tile::direction::six);
			}
			if (result && _predicate(*result)) {
				_promise.set_value(result);
				return;
			}
		}

		if (im.pressed(sf::Mouse::Left)) {
			result = the_game().camera().tile_hovered();
			if (_predicate(*result)) {
				the_game().world_renderer().clear_highlight_predicate();
				_promise.set_value(result);
				return;
			}
		}
	}

	void tile_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title, states);
		target.draw(_prompt, states);

		// Highlight valid tiles.
		//! @todo This.
	}
}
