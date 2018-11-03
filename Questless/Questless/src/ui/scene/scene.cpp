//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "scene.hpp"

#include "sdl/resources.hpp"

#include <sstream>
#include <thread>

using namespace sdl;
using namespace units;

namespace ql::scene {
	scene::scene() : _start_time{clock::now()} {}

	update_result scene::update(sec& accrued_duration, clock::time_point& last_update_time) {
		using namespace sdl;

		the_input().update();

		if (the_input().quit() || the_input().alt() && the_input().presses(SDLK_F4)) {
			return update_result::game_over;
		}

		if (the_input().window_resized()) {
			// Reset the viewport and viewport GLSL uniforms.
			glViewport(0, 0, static_cast<GLsizei>(the_window().width()), static_cast<GLsizei>(the_window().height()));
			{ // dflt_program
				dflt_program().use();
				GLint const viewport_size_uniform = dflt_program().get_uniform_handle("viewport_size");
				glUniform2f(viewport_size_uniform, static_cast<float>(the_window().width()), static_cast<float>(the_window().height()));
			}
			{ // solid_program
				solid_program().use();
				GLint const viewport_size_uniform = solid_program().get_uniform_handle("viewport_size");
				glUniform2f(viewport_size_uniform, static_cast<float>(the_window().width()), static_cast<float>(the_window().height()));
			}
		}

		if (subupdate() == update_result::game_over) { return update_result::game_over; }

		// Timekeeping

		auto frame_duration = to_sec_quantity(clock::now() - last_update_time);
		last_update_time = clock::now();
		accrued_duration += frame_duration;

		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(to_chrono_seconds(frame_duration - accrued_duration)));

		accrued_duration -= frame_duration;
		// Cap accrued time.
		accrued_duration = std::min(accrued_duration, 1.0_s);

		// Update FPS.

		if (frame_duration != 0.0_s) {
			_fps_buffer.push_back(1.0 / frame_duration);
		}
		constexpr std::size_t max_fps_buffer_size = 25;
		if (_fps_buffer.size() > max_fps_buffer_size) {
			_fps_buffer.pop_front();
		}

		return update_result::continue_game;
	}

	void scene::draw() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		subdraw();

		// Draw FPS counter.

		auto fps_buffer_sum = std::reduce(_fps_buffer.begin(), _fps_buffer.end(), 0.0_hz, std::plus<per_sec>{});
		std::ostringstream oss_fps;
		oss_fps.setf(std::ios::fixed);
		oss_fps.precision(2);
		oss_fps << fps_buffer_sum.value / _fps_buffer.size();
		texture txt_fps = _fnt_20pt->render(oss_fps.str().c_str(), colors::white());
		txt_fps.draw(window_space::point(the_window().width() - 1, the_window().height() - 1), texture_space::align_right, texture_space::align_bottom);

		// Swap buffers to update the screen.
		SDL_GL_SwapWindow(the_window().sdl_ptr());
	}
}
