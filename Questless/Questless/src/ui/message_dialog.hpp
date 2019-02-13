//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "dialog.hpp"

namespace ql {
	//! Displays a message to the player.
	struct message_dialog : dialog {
		message_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			std::string const& title,
			std::string const& prompt,
			std::function<void()> cont)
			: dialog{window, fonts}
			, _cont{std::move(cont)}
			, _title{make_title(title)}
			, _prompt{make_prompt(prompt)} //
		{}

		state update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		continuation<> _cont;

		sf::Text _title;
		sf::Text _prompt;
	};
}
