//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "incant.hpp"

#include "items/magic/gatestone.hpp"

namespace ql::qte {
	incant::incant(sf::Window const& window, rsrc::fonts const& fonts, gatestone& gatestone)
		: dialog{window, fonts}
		, _gatestone{gatestone} //
	{
		_title = make_title("Incant a spell!");
		_prompt = make_prompt("Use the arrow keys to control the pitch, and type the magic words.");
		_metronome.setOrigin({0, 5});
	}

	void incant::update(sec elapsed_time, std::vector<sf::Event>& events) {
		_metronome.setSize({10, _window.getSize().y});

		_total_elapsed_time += elapsed_time;

		// Set the position of the metronome.
		_metronome.setPosition({0, _window.getSize().x / 2 * (1 + sin(time_factor * _total_elapsed_time.value))});

		side const side = sin(time_factor * _total_elapsed_time.value) >= 0.0 ? side::right : side::left;
		if (_side != side) {
			// Metronome ticked. Read input.

			_side = side;
			_metronome.setFillColor(sf::Color::Red);

			bool left = im.down(sf::Keyboard::Left);
			bool right = im.down(sf::Keyboard::Right);
			bool up = im.down(sf::Keyboard::Up);
			bool down = im.down(sf::Keyboard::Down);
			if (left ^ right ^ up ^ down) {
				_begun = true;
				if (left) {
					_notes.push_back(note::left);
				} else if (right) {
					_notes.push_back(note::right);
				} else if (up) {
					_notes.push_back(note::up);
				} else if (down) {
					_notes.push_back(note::down);
				}
			} else if (_begun) {
				// Done with incantation. Interpret results.
				_promise.set_value([&]() -> std::optional<magic::spell> {
					if (_notes.empty()) return std::nullopt;
					if (_notes.front() == note::left) {
						return magic::heal{};
					} else if (_notes.front() == note::right) {
						return magic::shock{};
					} else if (_notes.front() == note::up) {
						return magic::teleport{};
					} else if (_notes.front() == note::down) {
						return magic::telescope{};
					}
				}());
				return;
			}
		} else {
			_metronome.setFillColor(sf::Color::White);
		}
	}

	void incant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(_title);
		target.draw(_prompt);
		target.draw(_metronome, states);
	}
}
