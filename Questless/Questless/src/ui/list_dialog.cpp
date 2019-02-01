//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "list_dialog.hpp"

namespace ql {
	list_dialog(sf::Window const& window,
		fonts const& fonts,
		sf::Vector2f origin,
		sf::String title,
		std::vector<sf::String> option_texts,
		std::function<void(std::optional<int>)> cont)
		: dialog{window}
		, _cont{std::move(cont)}
		, _title{make_title(window, title, sf::Color::Black)} //
	{
		//! @todo Replace with precondition.
		assert(!option_texts.empty());

		sf::Vector2f const padding{10, 10};

		_bg.setPosition(origin);
		_bg.setOutlineThickness(1);
		_bg.setOutlineColor(sf::Color::Black);
		_bg.setFillColor(sf::Color{255, 192, 153});

		float bg_width = _title.getLocalBounds().width;

		// Create options.
		for (auto const& option_text : option_texts) {
			sf::Text option{option_text, font, 20};
			option.setFillColor(sf::Color::Black);
			option.setOutlineColor(sf::Color::Black);
			option.setOutlineThickness(2);
			// Update background to fit this option's text.
			bg_width = std::max(bg_width, option.getLocalBounds().width);
			// Move new option into options list.
			_options.push_back(std::move(option));
		}

		// Highlight first option.
		_options.front().setFillColor(sf::Color::White);

		// Add width padding to background and set size.
		bg_width += 2 * padding.x;
		_bg.setSize({bg_width, _title_height + static_cast<int>(_options.size()) * _option_height + 2 * padding.y});

		// Confine bounds to window.
		auto bg_bounds = _bg.getGlobalBounds();
		if (bg_bounds.left + bg_bounds.width > _window.getSize().x) { _bg.move(-bg_bounds.width, 0); }
		if (bg_bounds.top + bg_bounds.height > _window.getSize().y) { _bg.move(0, -bg_bounds.height); }

		// Position contents relative to background.
		_title.setPosition(_bg.getPosition() + padding);
		for (std::size_t i = 0; i < _options.size(); ++i) {
			_options[i].setPosition(
				_bg.getPosition() + padding + sf::Vector2f{0, _title_height + static_cast<float>(i) * _option_height});
		}
	}

	state update(input_manager& im) final {
		if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) { return _cont(std::nullopt); }

		int const option_count = static_cast<int>(_options.size());

		if (!_options.empty()) {
			int const old_selection = _selection;

			_selection -= im.press_count(sf::Keyboard::Up);
			_selection += im.press_count(sf::Keyboard::Down);
			_selection += im.scroll();
			_selection = _selection % option_count;
			_selection = _selection < 0 ? _selection + option_count : _selection;

			int const end = std::min(10, option_count);
			for (int i = 0; i < end; ++i) {
				if (im.pressed(input_manager::index_to_num_key(i))) {
					if (_selection == i) {
						return _cont(_selection);
					} else {
						_selection = i;
						break;
					}
				}
			}

			if (old_selection != _selection) {
				// Selection changed. Update colors.
				_options[old_selection].setFillColor(sf::Color::Black);
				_options[_selection].setFillColor(sf::Color::White);
			}

			if (im.pressed({sf::Mouse::Left, sf::Keyboard::Return, sf::Keyboard::Space})) { return _cont(_selection); }
		}

		return state::open;
	}

	void list_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Draw background.
		target.draw(_bg);
		// Draw title.
		target.draw(_title);
		// Draw options.
		for (auto const& option : _options) {
			target.draw(option);
		}
	}
}
