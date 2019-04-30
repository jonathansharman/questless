//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <optional>
#include <string>

#include "dialog.hpp"

namespace ql {
	//! Retrieves a floating-point value from the player.
	struct magnitude_dialog : dialog {
		magnitude_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::String const& title,
			sf::String const& prompt,
			double default_value,
			std::optional<double> min,
			std::optional<double> max,
			std::function<void(std::optional<double>)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		double _magnitude;
		std::optional<double> _min;
		std::optional<double> _max;
		continuation<std::optional<double>> _cont;

		sf::Text _title;
		sf::Text _prompt;
		sf::Text _selector;
	};
}
