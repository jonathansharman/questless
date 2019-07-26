//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

namespace ql {
	//! A widget that serves only to contain other widgets.
	struct layout : widget {
		//! Convenience constructor for creating a layout with no parent that covers an entire window.
		layout(sf::RenderWindow const& window)
			: widget{nullptr}, local_offset{}, size{view::vector_from_sfml(window.getSize())} {}

		layout(widget* parent, view::vector local_offset, view::vector size)
			: widget{parent}, local_offset{local_offset}, size{size} {}

		view::vector get_local_offset() const final {
			return local_offset;
		}

		view::vector get_size() const final {
			return size;
		}

		void update(sec, std::vector<sf::Event>&) final {}

		void draw(sf::RenderTarget&, sf::RenderStates states) const final {}

	private:
		view::vector local_offset;
		view::vector size;
	};
}
