//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include <future>
#include <optional>

namespace ql {
	//! Retrieves a floating-point value from the player.
	struct magnitude_dialog : dialog<double> {
		magnitude_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::String const& title,
			sf::String const& prompt,
			double default_value,
			std::optional<double> min,
			std::optional<double> max);

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

	private:
		double _magnitude;
		std::optional<double> _min;
		std::optional<double> _max;

		sf::Text _title;
		sf::Text _prompt;
		sf::Text _selector;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
