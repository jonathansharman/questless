//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/qte/incant.h"

#include "game.h"
#include "items/magic/gatestone.h"
#include "magic/all_spells.h"

using namespace sdl;
using namespace units;

namespace ql::qte
{
	incant::incant(gatestone& gatestone, std::function<void(uptr<magic::spell>)> cont)
		: _gatestone{gatestone}
		, _cont{std::move(cont)}
	{
		load_textures();
	}

	dialog::state incant::update()
	{
		_elapsed_time += game::frame_duration;

		_tick = false;

		// Get the position of the metronome.
		side side = sin(time_factor * _elapsed_time.count()) >= 0.0 ? side::right : side::left;
		if (_side != side) {
			// Metronome ticked. Read input.

			_side = side;
			_tick = true;

			static auto metronome_sound_handle = the_sound_manager().add("resources/sounds/menu/hover.wav"); //! @todo This is a placeholder.
			the_sound_manager()[metronome_sound_handle].play();
			
			bool left = the_input().down(SDLK_LEFT);
			bool right = the_input().down(SDLK_RIGHT);
			bool up = the_input().down(SDLK_UP);
			bool down = the_input().down(SDLK_DOWN);
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

				uptr<magic::spell> spell = nullptr;
				if (_notes.size() > 0) {
					if (_notes.front() == note::left) {
						spell = std::make_unique<magic::heal>();
					} else if (_notes.front() == note::right) {
						spell = std::make_unique<magic::shock>();
					} else if (_notes.front() == note::up) {
						spell = std::make_unique<magic::teleport>();
					} else if (_notes.front() == note::down) {
						spell = std::make_unique<magic::eagle_eye>();
					}
				}
				return _cont(std::move(spell));
			}
		}

		return state::open;
	}

	void incant::draw() const
	{
		static constexpr int metronome_width = 10;

		int x_center = the_window().x_center();
		int window_width = the_window().width();
		int window_height = the_window().height();

		int x = static_cast<int>(x_center - metronome_width / 2 + sin(time_factor * _elapsed_time.count()) * window_width / 2);
		the_renderer().draw_box
			( screen_space::box
				{ screen_space::point{x, 0}
				, screen_space::vector{metronome_width, window_height}
				}
			, _tick ? colors::red() : colors::white()
			);

		draw_title(*_txt_title);
		draw_prompt(*_txt_prompt);
	}

	void incant::load_textures()
	{
		_txt_title = make_title("Incant a spell!");
		_txt_prompt = make_prompt("Use the arrow keys to control the pitch, and type the magic words.");
	}
}
