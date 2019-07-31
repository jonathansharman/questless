//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "panel.hpp"

namespace ql {
	panel::panel(widget& parent) : widget{&parent} {}

	view::vector panel::get_size() const {
		return parent()->get_size();
	}

	void panel::update(sec elapsed_time, std::vector<sf::Event>& events) {
		for (auto child : children) {
			child->update(elapsed_time, events);
		}
	}

	void panel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto child : children) {
			target.draw(*child, states);
		}
	}
}
