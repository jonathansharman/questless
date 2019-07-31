//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	//! Holds any number of elements.
	struct panel : widget {
		//! The widgets contained in this panel.
		std::vector<widget*> children;

		panel(widget& parent);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
