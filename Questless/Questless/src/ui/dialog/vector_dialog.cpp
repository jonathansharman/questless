//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "vector_dialog.hpp"

namespace ql {
	vector_dialog::vector_dialog( //
		widget* parent,
		rsrc::fonts const& fonts,
		sf::String const& title,
		sf::String const& prompt,
		std::optional<region_tile::point> origin,
		std::function<bool(region_tile::vector)> predicate)
		: dialog{parent, fonts}
		, _origin{std::move(origin)}
		, _predicate{std::move(predicate)}
		, _title{make_title{window, font, title}}
		, _prompt{make_prompt{window, font, prompt}} {}

	void vector_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_promise.set_value(std::nullopt);
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
			_promise.set_value(result);
			return;
		}

		if (_origin && im.pressed(sf::Mouse::Left)) {
			result = the_game().camera().tile_hovered() - *_origin;
			if (_predicate(*result)) {
				_promise.set_value(result);
				return;
			}
		}

		return state::open;
	}

	void vector_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title, states);
		target.draw(_prompt, states);
	}
}
