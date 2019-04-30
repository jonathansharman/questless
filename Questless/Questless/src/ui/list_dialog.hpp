//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include <string>
#include <vector>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! Retrieves the player's choice from a list of options.
	struct list_dialog : dialog {
		list_dialog(sf::Window const& window,
			rsrc::fonts const& fonts,
			sf::Vector2f origin,
			sf::String title,
			std::vector<sf::String> option_texts,
			std::function<void(std::optional<int>)> cont);

		void update(sec /*elapsed_time*/, input_manager& im) final {
			if (im.pressed({sf::Keyboard::Backspace, sf::Keyboard::Escape})) {
				_cont(std::nullopt);
				close();
				return;
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
							_cont(_selection);
							close();
							return;
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

				if (im.pressed({sf::Mouse::Left, sf::Keyboard::Return, sf::Keyboard::Space})) {
					_cont(_selection);
					close();
					return;
				}
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final {
			// Draw background.
			target.draw(_bg, states);
			// Draw title.
			target.draw(_title, states);
			// Draw options.
			for (auto const& option : _options) {
				target.draw(option, states);
			}
		}

	private:
		static constexpr float _title_height = 40;
		static constexpr float _option_height = 20;

		int _selection = 0;
		continuation<std::optional<int>> _cont;

		sf::RectangleShape _bg;
		sf::Text _title;
		std::vector<sf::Text> _options;
	};
}
