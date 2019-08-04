//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "widget.hpp"

#include <SFML/Graphics/Text.hpp>

namespace ql {
	//! Displays some text.
	struct label : widget {
		auto get_size() const -> view::vector final;

		auto update(sec) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		//! Sets this label's text to @p text.
		auto set_text(sf::Text text) -> void;

	private:
		sf::Text _text;
		view::point _position;
		view::vector _size;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
