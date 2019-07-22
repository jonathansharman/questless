//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "widget.hpp"

#include <SFML/Graphics/Text.hpp>

namespace ql {
	struct label : widget {
		sf::Text text;

		sf::FloatRect get_bounding_box() const final;

		void update(sec, input_manager&) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
