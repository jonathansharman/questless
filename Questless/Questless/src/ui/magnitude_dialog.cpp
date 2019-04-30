//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "magnitude_dialog.hpp"

namespace ql {
	magnitude_dialog::magnitude_dialog(sf::Window const& window,
		fonts const& fonts,
		sf::String const& title,
		sf::String const& prompt,
		double default_value,
		std::optional<double> min,
		std::optional<double> max,
		std::function<void(std::optional<double>)> cont)
		: dialog{window, fonts}
		, _magnitude{default_value}
		, _min{min}
		, _max{max}
		, _cont{std::move(cont)}
		, _title{make_title(title)}
		, _prompt{make_prompt(prompt)}
		, _selector{make_selector(std::to_string(_magnitude))} //
	{}

	void magnitude_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_cont(std::nullopt);
			close();
			return;
		}

		double old_magnitude = _magnitude;

		int const ups = im.press_count(sf::Keyboard::Up);
		int const downs = im.press_count(sf::Keyboard::Down);
		_magnitude += (im.shift() ? 10.0 : 1.0) * (ups - downs + im.scroll());

		// Clamp magnitude, if bounds are set.
		if (_min) { _magnitude = std::max(*_min, _magnitude); }
		if (_max) { _magnitude = std::min(*_max, _magnitude); }

		// Update selector texture if value changed.
		if (old_magnitude != _magnitude) { _selector = make_selector(_window, std::to_string(_magnitude)); }

		// Choose value.
		if (im.pressed({sf::Mouse::Left, sf::Keyboard::Return, sf::Keyboard::Space})) {
			_cont(_magnitude);
			close();
			return;
		}

		return state::open;
	}

	void magnitude_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);
		draw_selector(_selector);
	}
}
