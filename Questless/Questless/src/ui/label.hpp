//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "widget.hpp"

#include <SFML/Graphics/Text.hpp>

namespace ql {
	struct label : widget {
		sf::Text text;

		view::vector get_local_offset() const final;

		view::vector get_size() const final;

		void update(sec, std::vector<sf::Event>&) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
