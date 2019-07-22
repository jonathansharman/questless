//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

namespace ql {
	//! Displays a message to the player.
	struct message_dialog : dialog<void> {
		message_dialog(sf::Window const& window, rsrc::fonts const& fonts, std::string const& title, std::string const& prompt)
			: dialog{window, fonts}
			, _title{make_title(title)}
			, _prompt{make_prompt(prompt)} //
		{}

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		label _title;
		label _prompt;
	};
}
