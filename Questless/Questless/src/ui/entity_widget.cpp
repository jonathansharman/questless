//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entity_widget.hpp"

#include "animation/animation.hpp"

namespace ql {
	auto entity_widget::get_size() const -> view::vector {
		//! @todo This.
		return {};
	}

	auto entity_widget::update(sec elapsed_time) -> void {
		_ani->update(elapsed_time);
	}

	auto entity_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		target.draw(*_ani, states);
	}

	auto entity_widget::set_position(view::point position) -> void {
		_ani->setPosition(to_sfml(position));
	}

	auto entity_widget::get_position() const -> view::point {
		return view::point_from_sfml(_ani->getPosition());
	}
}
