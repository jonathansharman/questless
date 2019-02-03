//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/incant.hpp"

#include "game.hpp"
#include "items/magic/gatestone.hpp"
#include "magic/all_spells.hpp"

using namespace media;

namespace ql::qte {
	incant::incant(gatestone& gatestone, std::function<void(std::optional<magic::spell> const&)> cont)
		: _gatestone{gatestone}, _cont{std::move(cont)} {
		_title = make_title("Incant a spell!");
		_prompt = make_prompt("Use the arrow keys to control the pitch, and type the magic words.");
		_metronome.setOrigin({0, 5});
	}

	incant::~incant() {}

	dialog::state incant::update(input_manager& im) {
		_metronome.setSize({10, _window.getSize().y});

		_elapsed_time += target_frame_duration;

		// Set the position of the metronome.
		_metronome.setPosition({0, _window.getSize().x / 2 * (1 + sin(time_factor * _elapsed_time.value))});

		side const side = sin(time_factor * _elapsed_time.value) >= 0.0 ? side::right : side::left;
		if (_side != side) {
			// Metronome ticked. Read input.

			_side = side;
			_metronome.setFillColor(sf::Color::Red);

			static auto metronome_sound_buffer_handle = the_sound_buffer_manager().add(
				"resources/sounds/menu/hover.wav"); //! @todo This is a placeholder.
			static auto metronome_sound_handle = the_sound_manager().add(
				the_sound_buffer_manager()[metronome_sound_buffer_handle]);
			the_sound_manager()[metronome_sound_handle].play();

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
				return _cont([&]() -> std::optional<magic::spell> {
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
			}
		} else {
			_metronome.setFillColor(sf::Color::White);
		}

		return state::open;
	}

	void incant::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		draw_title(_title);
		draw_prompt(_prompt);
		the_window().draw(_metronome);
	}
}
