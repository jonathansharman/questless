//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"
#include "world/coordinates.hpp"

#include <optional>

namespace ql {
	//! Retrieves a direction from the player.
	struct direction_dialog : dialog {
		direction_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			std::string title,
			std::string prompt,
			std::function<void(std::optional<region_tile::direction>)> cont)
			: dialog{window, fonts}
			, _cont{std::move(cont)}
			, _title{make_title(title)}
			, _prompt{make_prompt(prompt)} //
		{}

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		continuation<std::optional<region_tile::direction>> _cont;

		label _title;
		label _prompt;
	};
}
