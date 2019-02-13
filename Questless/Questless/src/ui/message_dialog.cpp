//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "message_dialog.hpp"

namespace ql {
	dialog::state message_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.any_pressed()) { return _cont(); }
		return state::open;
	}

	void message_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);
	}
}
