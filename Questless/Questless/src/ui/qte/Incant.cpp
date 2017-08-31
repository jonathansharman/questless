//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/Incant.h"

#include "Game.h"
#include "items/magic/Gatestone.h"
#include "spell/all-spells.h"

using namespace sdl;
using namespace units;

namespace questless::qte
{
	Incant::Incant(Gatestone& gatestone, std::function<void(uptr<spell::Spell>)> cont)
		: _gatestone{gatestone}
		, _cont{std::move(cont)}
	{
		load_textures();
	}

	Dialog::State Incant::update()
	{
		_elapsed_time += Game::frame_duration;

		_tick = false;

		// Get the position of the metronome.
		Side side = sin(time_factor * _elapsed_time.count()) >= 0.0 ? Side::right : Side::left;
		if (_side != side) {
			// Metronome ticked. Read input.

			_side = side;
			_tick = true;

			static auto metronome_sound_handle = sound_manager().add("resources/sounds/menu/hover.wav"); //! @todo This is a placeholder.
			sound_manager()[metronome_sound_handle].play();
			
			bool left = input().down(SDLK_LEFT);
			bool right = input().down(SDLK_RIGHT);
			bool up = input().down(SDLK_UP);
			bool down = input().down(SDLK_DOWN);
			if (left ^ right ^ up ^ down) {
				_begun = true;
				if (left) {
					_notes.push_back(Note::left);
				} else if (right) {
					_notes.push_back(Note::right);
				} else if (up) {
					_notes.push_back(Note::up);
				} else if (down) {
					_notes.push_back(Note::down);
				}
			} else if (_begun) {
				// Done with incantation. Interpret results.

				uptr<spell::Spell> spell = nullptr;
				if (_notes.size() > 0) {
					if (_notes.front() == Note::left) {
						spell = std::make_unique<spell::Heal>();
					} else if (_notes.front() == Note::right) {
						spell = std::make_unique<spell::LightningBolt>();
					} else if (_notes.front() == Note::up) {
						spell = std::make_unique<spell::Teleport>();
					} else if (_notes.front() == Note::down) {
						spell = std::make_unique<spell::EagleEye>();
					}
				}
				return _cont(std::move(spell));
			}
		}

		return State::open;
	}

	void Incant::draw() const
	{
		static constexpr int metronome_width = 10;

		int x_center = window().center().x();
		int window_width = window().width();
		int window_height = window().height();

		int x = static_cast<int>(x_center - metronome_width / 2 + sin(time_factor * _elapsed_time.count()) * window_width / 2);
		renderer().draw_box
			( ScreenSpace::Box
				{ ScreenSpace::Point{x, 0}
				, ScreenSpace::Vector{metronome_width, window_height}
				}
			, _tick ? colors::red() : colors::white()
			, Fill::solid
			);

		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void Incant::load_textures()
	{
		_txt_title = make_title("Incant a spell!");
		_txt_prompt = make_prompt("Use the arrow keys to control the pitch, and type the magic words.");
	}
}
