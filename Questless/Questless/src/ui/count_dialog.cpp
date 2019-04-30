//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/count_dialog.hpp"

namespace ql {
	count_dialog::count_dialog(sf::Window const& window,
		sf::Font const& font,
		sf::String const& title,
		sf::String const& prompt,
		int default_value,
		std::optional<int> min,
		std::optional<int> max,
		std::function<void(std::optional<int>)> cont)
		: dialog{window}
		, _count{default_value}
		, _min{min}
		, _max{max}
		, _cont{std::move(cont)}
		, _title{make_title(window, font, title)}
		, _prompt{make_prompt(window, font, prompt)}
		, _selector{make_selector(window, font, std::to_string(_count))} //
	{}

	void count_dialog::update(sec elapsed_time, input_manager& im) {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
			_cont(std::nullopt);
			close();
			return;
		}

		int old_count = _count;

		_count += (im.shift() ? 10 : 1) * (im.pressed(sf::Keyboard::Up) - im.pressed(sf::Keyboard::Down) + im.scroll());
		_count = (_min && _count < *_min) ? *_min : _count;
		_count = (_max && _count > *_max) ? *_max : _count;

		// Update selector texture if value changed.
		if (old_count != _count) { _selector.text.setString(std::to_string(_count)); }

		if (im.pressed({sf::Mouse::Left, sf::Keyboard::Return, sf::Keyboard::Space})) {
			_cont(_count);
			close();
			return;
		}
		return state::open;
	}

	void count_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);
		draw_selector(_selector);
	}
}
