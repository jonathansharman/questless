//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "utility/reference.hpp"

namespace ql {
	struct animation;

	//! Allows interaction with a being in the world.
	struct being_widget : widget {
		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		uptr<animation> _ani;
	};
}
