//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "list_dialog.hpp"

#include "rsrc/fonts.hpp"

namespace ql {
	list_dialog::list_dialog( //
		widget& parent,
		rsrc::fonts const& fonts,
		sf::Vector2f origin,
		sf::String title,
		std::vector<std::tuple<sf::String, std::function<void()>>> options)
		: dialog{parent}
		, _title{make_title(window, title, sf::Color::Black)} //
	{
		sf::Vector2f const padding{10, 10};

		_bg.setPosition(origin);
		_bg.setOutlineThickness(1);
		_bg.setOutlineColor(sf::Color::Black);
		_bg.setFillColor(sf::Color{255, 192, 153});

		float bg_width = _title.getLocalBounds().width;

		// Create options.
		for (auto& [string, callback] : options) {
			sf::Text option_text{string, font, 20};
			option_text.setFillColor(sf::Color::Black);
			option_text.setOutlineColor(sf::Color::Black);
			option_text.setOutlineThickness(2);
			// Update background to fit this option's text.
			bg_width = std::max(bg_width, option_text.getLocalBounds().width);
			// Move new option into options list.
			_options.emplace_back(std::move(option_text), std::move(callback));
		}

		// Highlight first option.
		std::get<0>(_options.front()).setFillColor(sf::Color::White);

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
			std::get<0>(_options[i])
				.setPosition(
					_bg.getPosition() + padding + sf::Vector2f{0, _title_height + static_cast<float>(i) * _option_height});
		}
	}

	void list_dialog::update(sec /*elapsed_time*/, std::vector<sf::Event>& events) {
		for (auto event : events) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Backspace || sf::Keyboard::Escape) {
					_promise.set_value(std::nullopt);
					return;
				}
			}
		}

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
						close();
						std::get<1>(_options(_selection))();
						return;
					} else {
						_selection = i;
						break;
					}
				}
			}

			if (old_selection != _selection) {
				// Selection changed. Update colors.
				std::get<0>(_options[old_selection]).setFillColor(sf::Color::Black);
				std::get<0>(_options[_selection]).setFillColor(sf::Color::White);
			}

			if (im.pressed({sf::Mouse::Left, sf::Keyboard::Return, sf::Keyboard::Space})) {
				close();
				std::get<1>(_options(_selection))();
				return;
			}
		}
	}

	void list_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Draw background.
		target.draw(_bg);
		// Draw title.
		target.draw(_title);
		// Draw options.
		for (auto const& option : _options) {
			target.draw(option, states);
		}
	}
}
