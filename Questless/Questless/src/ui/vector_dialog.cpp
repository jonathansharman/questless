//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "vector_dialog.hpp"

#include "game.hpp"

using namespace media;

namespace ql {
	vector_dialog::vector_dialog(sf::Window& window,
		rsrc::fonts const& fonts,
		sf::String const& title,
		sf::String const& prompt,
		std::optional<region_tile::point> origin,
		std::function<bool(region_tile::vector)> predicate,
		std::function<void(std::optional<region_tile::vector>)> cont)
		: dialog{window, fonts}
		, _origin{std::move(origin)}
		, _predicate{std::move(predicate)}
		, _cont{std::move(cont)}
		, _title{make_title{window, font, title}}
		, _prompt{make_prompt{window, font, prompt}} {}

	void vector_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_cont(std::nullopt);
			close();
			return;
		}

		std::optional<region_tile::vector> result;

		if (im.pressed(sf::Keyboard::E)) {
			result = region_tile::vector::unit(region_tile::direction::one);
		} else if (im.pressed(sf::Keyboard::W)) {
			result = region_tile::vector::unit(region_tile::direction::two);
		} else if (im.pressed(sf::Keyboard::Q)) {
			result = region_tile::vector::unit(region_tile::direction::three);
		} else if (im.pressed(sf::Keyboard::A)) {
			result = region_tile::vector::unit(region_tile::direction::four);
		} else if (im.pressed(sf::Keyboard::S)) {
			result = region_tile::vector::unit(region_tile::direction::five);
		} else if (im.pressed(sf::Keyboard::D)) {
			result = region_tile::vector::unit(region_tile::direction::six);
		}
		if (result && _predicate(*result)) {
			cont(*result);
			close();
			return;
		}

		if (_origin && im.pressed(sf::Mouse::Left)) {
			result = the_game().camera().tile_hovered() - *_origin;
			if (_predicate(*result)) {
				cont(*result);
				close();
				return;
			}
		}

		return state::open;
	}

	void vector_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);

		// Highlight valid tiles.
		//! @todo This.
	}
}
