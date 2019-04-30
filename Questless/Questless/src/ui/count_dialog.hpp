//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <string>

#include "dialog.hpp"

namespace ql {
	//! Retrieves a count from the player.
	struct count_dialog : dialog {
		count_dialog(sf::Window const& window,
			sf::Font const& font,
			sf::String const& title,
			sf::String const& prompt,
			int default_value,
			std::optional<int> min,
			std::optional<int> max,
			std::function<void(std::optional<int>)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		int _count;
		std::optional<int> _min;
		std::optional<int> _max;
		continuation<std::optional<int>> _cont;

		label _title;
		label _prompt;
		label _selector;
	};
}
