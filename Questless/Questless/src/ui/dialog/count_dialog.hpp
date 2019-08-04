//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include <optional>

namespace ql {
	//! Retrieves a count from the player.
	struct count_dialog : dialog<int> {
		count_dialog( //
			sf::Font const& font,
			sf::String const& title,
			sf::String const& prompt,
			int default_value,
			std::optional<int> min,
			std::optional<int> max);

		void update(sec elapsed_time) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		int _count;
		std::optional<int> _min;
		std::optional<int> _max;

		label _title;
		label _prompt;
		label _selector;
	};
}
