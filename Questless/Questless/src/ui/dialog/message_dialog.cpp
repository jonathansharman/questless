//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "message_dialog.hpp"

namespace ql {
	void message_dialog::update(sec elapsed_time, std::vector<sf::Event>& events) {
		if (im.any_pressed()) {
			_cont();
			close();
		}
	}

	void message_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);
	}
}
