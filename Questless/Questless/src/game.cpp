//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "game.hpp"

#include "animation/entity_animator.hpp"
#include "entities/objects/object.hpp"
#include "utility/io.hpp"
#include "utility/random.hpp"
#include "world/coordinates.hpp"
#include "world/light_source.hpp"
//! @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/agent.hpp"
#include "agents/player.hpp"
#include "items/magic/gatestone.hpp"
#include "entities/beings/goblinoids/goblin.hpp"
#include "entities/beings/human.hpp"
#include "items/scroll.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/all_spells.hpp"

#include "sdl/shader_program.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <sstream>
#include <thread>

using std::move;
using std::dynamic_pointer_cast;
using std::string;
using std::function;

using namespace std::chrono;

using namespace sdl;
using namespace units;

namespace ql {
	id<being> game::get_being_id(uptr<being> const& being) { return being->id; }
	id<object> game::get_object_id(uptr<object> const& object) { return object->id; }
	id<item> game::get_item_id(uptr<item> const& item) { return item->id; }
	id<light_source> game::get_light_source_id(uptr<light_source> const& light_source) { return light_source->id; }

	game::game(bool fullscreen)
		: _player_being_id{std::nullopt}
		, _splash_sound_played{false}
		, _state{state::splash}
		, _main_menu{480, 640}
	{
		// Set OpenGL version.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Initialize SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
			throw std::runtime_error("Failed to initialize SDL. SDL Error: " + string{SDL_GetError()});
		}

		// Initialize window.
		if (fullscreen) {
			set_the_window(umake<window>("Questless", "resources/textures/icon.png", true));
		} else {
			set_the_window(umake<window>("Questless", "resources/textures/icon.png", false, _dflt_window_width, _dflt_window_height, true, true, true, false));
		}

		// Initialize OpenGL.
		initialize_opengl();

		// Initialize renderer.
		set_the_renderer(umake<renderer>(the_window(), the_window().width(), the_window().height()));

		_camera = umake<ql::camera>(game_space::point{0.0, 0.0});

		if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
			throw std::runtime_error("Failed to initialize IMG. SDL Error: " + string{SDL_GetError()});
		}

		// Initialize and load fonts.

		if (TTF_Init()) {
			throw std::runtime_error("Failed to initialize TTF. SDL Error: " + string{SDL_GetError()});
		}

		_fnt_20pt = umake<font>("resources/fonts/firamono.ttf", 20, SDL_BLENDMODE_BLEND);

		// Initialize sound.

	    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)) {
			throw std::runtime_error("Failed to initialize Mix. SDL Error: " + string{SDL_GetError()});
	    }

		// Load textures and graphics.

		load_textures();

		// Load sounds.

		_sfx_splash = umake<sound>("resources/sounds/splash.wav");

		// Initialize game state.

		for (int i = 0; i < _splash_flames_count; ++i) {
			_splash_flame_positions.emplace_back(uniform(0, the_window().width() - 1), (i + 1) * the_window().height() / _splash_flames_count);
		}

		_time_last_state_change = clock::now();
		_state = state::splash;
	}

	game::~game() {
		// Need to delete all SDL-related variables prior to calling the Close/Quit functions.

		// Delete sounds.

		// Delete textures.

		// Delete fonts.

		_fnt_20pt = nullptr;

		// Delete renderer.

		set_the_renderer(nullptr);

		// Delete window.

		set_the_window(nullptr);

		// Quit SDL components.

		Mix_CloseAudio();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void game::load_textures() {
		_txt_test1 = umake<texture>("resources/textures/test1.png");
		_txt_test2 = umake<texture>("resources/textures/test2.png");
		_txt_test3 = umake<texture>("resources/textures/test3.png");
		_txt_test_even = umake<texture>("resources/textures/test-even.png");

		_txt_splash_logo = umake<texture>("resources/textures/splash/logo.png");

		_txt_splash_flame = umake<texture>("resources/textures/splash/flame.png");

		_txt_hex_highlight = umake<texture>("resources/textures/terrain/tile.png");

		_txt_hex_circle = umake<texture>("resources/textures/ui/hex_circle.png");
	}

	void game::initialize_opengl() {
		// Create OpenGL context.
		opengl_context() = SDL_GL_CreateContext(the_window().sdl_ptr()); //! @todo Destroy context?
		if (!opengl_context()) {
			throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + string{SDL_GetError()});
		}

		// Initialize GLEW.
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW.");
		}

		// Use late swap tearing.
		if (SDL_GL_SetSwapInterval(-1)) {
			// Fall back to VSync.
			if (SDL_GL_SetSwapInterval(1)) {
				throw std::runtime_error("Failed to set swap interval. SDL Error: " + string{SDL_GetError()});
			}
		}

		// Generate GLSL programs.
		set_dflt_program(umake<shader_program>
			( contents_of_file("resources/shaders/dflt.vert").c_str()
			, contents_of_file("resources/shaders/dflt.frag").c_str()
			));
		set_solid_program(umake<shader_program>
			( contents_of_file("resources/shaders/solid.vert").c_str()
			, contents_of_file("resources/shaders/solid.frag").c_str()
			));

		// Set clear color.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		{ // Set default program uniforms.
			dflt_program().use();

			GLint const viewport_size_uniform = dflt_program().get_uniform_handle("viewport_size");
			int const window_width = the_window().width();
			int const window_height = the_window().height();
			glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

			GLint const flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
			glUniform1i(flip_y_uniform, GL_FALSE);
		}
		{ // Set solid program uniforms.
			solid_program().use();

			GLint const viewport_size_uniform = solid_program().get_uniform_handle("viewport_size");
			int const window_width = the_window().width();
			int const window_height = the_window().height();
			glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

			GLint const flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
			glUniform1i(flip_y_uniform, GL_FALSE);
		}

		// Set blend function.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	complete game::add_dialog(uptr<dialog> dialog) {
		_dialogs.push_back(move(dialog));
		return complete{};
	}

	complete game::query_player_choice(function<void(player_action_dialog::choice)> cont) {
		_player_action_dialog = umake<player_action_dialog>(*_hud, move(cont));
		return complete{};
	}

	// Main Game Logic

	void game::run() {
		game_space::seconds accrued_time = game_space::seconds::zero();
		clock::time_point last_update_time = clock::now();
		while (update(accrued_time, last_update_time) == update_result::continue_game) {
			render();
		}
		if (_region) {
			_region->save("Slot1");
		}
	}

	game::update_result game::update(game_space::seconds& accrued_time, clock::time_point& last_update_time) {
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

		{ // Perform state-specific updates.
			update_result result = update_result::game_over;
			switch (_state) {
				case state::splash:
					result = update_splash();
					break;
				case state::menu:
					result = update_menu();
					break;
				case state::playing:
					result = update_playing();
					break;
				default:
					break;
			}
			if (result == update_result::game_over) return update_result::game_over;
		}

		// Timekeeping

		auto frame_time = clock::now() - last_update_time;
		last_update_time = clock::now();
		accrued_time += frame_time;

		std::this_thread::sleep_for(duration_cast<milliseconds>(frame_duration - accrued_time));

		accrued_time -= frame_duration;
		if (accrued_time > _max_accrued_update_time) {
			accrued_time = _max_accrued_update_time;
		}

		// Update FPS.

		auto frame_ms = duration_cast<milliseconds>(frame_time).count();
		if (frame_ms != 0) {
			_fps_buffer.push_back(1000.0 / frame_ms);
		}
		if (_fps_buffer.size() > _max_fps_buffer_size) {
			_fps_buffer.pop_front();
		}

		return update_result::continue_game;
	}

	void game::render() {
		////////////
		// Render //
		////////////

		switch (_state) {
			case state::splash:
				glClear(GL_COLOR_BUFFER_BIT);
				render_splash();
				break;
			case state::menu:
				glClear(GL_COLOR_BUFFER_BIT);
				render_menu();
				break;
			case state::playing:
				glClear(GL_COLOR_BUFFER_BIT);
				render_playing();
				break;
		}

		// Draw FPS counter.

		double fps_buffer_sum = 0.0;
		for (double fps : _fps_buffer) {
			fps_buffer_sum += fps;
		}
		std::ostringstream oss_fps;
		oss_fps.setf(std::ios::fixed);
		oss_fps.precision(2);
		oss_fps << fps_buffer_sum / _fps_buffer.size();
		texture txt_fps = _fnt_20pt->render(oss_fps.str().c_str(), colors::white());
		txt_fps.draw(window_space::point(the_window().width() - 1, the_window().height() - 1), texture_space::align_right, texture_space::align_bottom);

		// Swap buffers to update the screen.
		SDL_GL_SwapWindow(the_window().sdl_ptr());
	}

	game::update_result game::update_splash() {
		if (!_splash_sound_played) {
			_splash_sound_played = true;
			_sfx_splash->play();
		}

		for (window_space::point& position : _splash_flame_positions) {
			position.y() += lround(_splash_flames_vy * frame_duration);
			if (position.y() < 0) {
				position.y() += the_window().height() + _txt_splash_flame->height();
				position.x() = uniform(0, the_window().width() - 1);
			}
		}

		if (the_input().any_presses() || clock::now() - _time_last_state_change >= _splash_duration) {
			// End splash.

			_splash_flame_positions.clear();
			_txt_splash_logo = nullptr;
			_txt_splash_flame = nullptr;
			_sfx_splash->stop();

			// Initialize menu.

			// Add pages.
			_main_menu.add_page("Questless");
			_main_menu.add_page("Settings");
			// Add options.
			_main_menu.add_option("Questless", "Continue");
			_main_menu.add_option("Questless", "Begin Anew");
			_main_menu.add_option("Questless", "Settings", "Settings");
			_main_menu.add_option("Questless", "Quit");
			_main_menu.add_option("Settings", "Save", "Questless");
			_main_menu.add_option("Settings", "Cancel", "Questless");

			_time_last_state_change = clock::now();
			_state = state::menu;
		}
		return update_result::continue_game;
	}
	
	void game::render_splash() {
		float intensity;
		if (clock::now() - _time_last_state_change < _splash_fade_in_duration) {
			auto ms_fading_in = duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count();
			intensity = static_cast<float>(ms_fading_in / _splash_fade_in_duration.count());
		} else {
			auto ms_fading_out = duration_cast<game_space::seconds>(clock::now() - _time_last_state_change - _splash_fade_in_duration).count();
			intensity = static_cast<float>(1.0 - ms_fading_out / _splash_fade_out_duration.count());
		}

		window_space::point logo_position = the_window().window_center() + window_space::vector{uniform(-_splash_logo_jiggle, _splash_logo_jiggle), uniform(-_splash_logo_jiggle, _splash_logo_jiggle)};
		_txt_splash_logo->draw(logo_position, texture_space::align_center, texture_space::align_middle, colors::color_vector{intensity, intensity, intensity, 1.0f});

		for (window_space::point position : _splash_flame_positions) {
			_txt_splash_flame->draw(position, texture_space::align_center, texture_space::align_bottom, colors::color_vector{intensity, intensity, intensity, 1.0f});
		}
	}

	game::update_result game::update_menu() {
		_main_menu.update();
		for (auto const& option : _main_menu.poll_selections()) {
			if (option.first == "Questless") {
				if (option.second == "Continue" || option.second == "Begin Anew") {
					_region = umake<ql::region>("Region1");
					//_region = umake<region>("Slot1", "Region1");

					{ // Spawn the player's being.
						auto player_being = umake<human>(agent::make<player>);
						_player = dynamic_cast<player*>(&player_being->agent());
						_player_being_id = player_being->id;
						player_being->inventory.add(items.add(umake<scroll>(umake<magic::shock>())).id);
						player_being->inventory.add(items.add(umake<scroll>(umake<magic::heal>())).id);
						player_being->inventory.add(items.add(umake<scroll>(umake<magic::teleport>())).id);
						player_being->inventory.add(items.add(umake<scroll>(umake<magic::eagle_eye>())).id);
						player_being->inventory.add(items.add(umake<quarterstaff>()).id);
						player_being->inventory.add(items.add(umake<bow>()).id);
						{
							inventory inventory;
							constexpr int arrow_count = 20;
							for (int i = 0; i < arrow_count; ++i) {
								inventory.add(items.add(umake<arrow>()).id);
							}
							player_being->inventory.add(items.add(umake<quiver>(std::move(inventory))).id);
						}
						player_being->inventory.add(items.add(umake<gatestone>(100.0, 100.0, 0.0, magic::color::green)).id);
						_region->spawn_player(move(player_being));
					}
					// Create the HUD and pass the player's being ID to it.
					_hud = umake<ql::hud>();
					_hud->set_player_being_id(*_player_being_id);
					// Update the player's initial world view.
					_player->update_world_view();
					// Initialize the world renderer.
					_world_renderer = umake<ql::world_renderer>(_player->world_view());
					// Set the camera position relative to the player's being.
					_camera->position(game_space::point{to_world(beings.cref(*_player_being_id).coords)});

					_time_last_state_change = clock::now();
					_state = state::playing;
				} else if (option.second  == "Quit") {
					return update_result::game_over;
				}
			}
		}
		return update_result::continue_game;
	}

	void game::render_menu() {
		_main_menu.draw(the_window().window_center(), window_space::align_center, window_space::align_middle);
	}

	void game::render_playing() {
		_world_renderer->draw_terrain();

		if (the_input().pressed(mouse_button::right)) {
			_point_clicked_rounded = to_world(_camera->tile_hovered());
		}
		_camera->draw(*_txt_hex_highlight, to_world(_camera->tile_hovered()), origin{texture_space::vector::zero()}, colors::white_vector(0.5f));
		_camera->draw(*_txt_hex_circle, _point_clicked_rounded);

		_world_renderer->draw_entities();
		_world_renderer->draw_effects();

		// Draw HUD.
		_hud->draw();

		// Draw dialogs.
		for (auto const& dialog : _dialogs) {
			dialog->draw();
		}

		{ // Draw camera position.
			std::ostringstream ss_cam_coords;
			ss_cam_coords.setf(std::ios::fixed);
			ss_cam_coords.precision(2);
			ss_cam_coords << "Cam: ((" << _camera->position().x() << ", " << _camera->position().y() << "), ";
			ss_cam_coords << _camera->angle().count() << ", " << _camera->zoom() << ")";
			texture txt_cam_coords = _fnt_20pt->render(ss_cam_coords.str().c_str(), colors::white());
			txt_cam_coords.draw(window_space::point{0, 0});
		}
		{ // Draw camera hex position.
			auto cam_hex_coords = to_region_tile(_camera->position());
			std::ostringstream ss_cam_hex_coords;
			ss_cam_hex_coords << "Cam hex: (" << cam_hex_coords.q << ", " << cam_hex_coords.r << ")";
			texture txt_cam_hex_coords = _fnt_20pt->render(ss_cam_hex_coords.str().c_str(), colors::white());
			txt_cam_hex_coords.draw(window_space::point{0, 25});
		}
		{ // Draw time.
			std::ostringstream ss_time;
			double const time_of_day = _region->time_of_day();
			std::string time_name;
			switch (_region->period_of_day()) {
				case period_of_day::morning:
					time_name = "Morning";
					break;
				case period_of_day::afternoon:
					time_name = "Afternoon";
					break;
				case period_of_day::dusk:
					time_name = "Dusk";
					break;
				case period_of_day::evening:
					time_name = "Evening";
					break;
				case period_of_day::night:
					time_name = "Night";
					break;
				case period_of_day::dawn:
					time_name = "Dawn";
					break;
			}
			ss_time << "Time: " << _region->time() << " (" << time_of_day << ", " << time_name << ')';
			texture txt_turn = _fnt_20pt->render(ss_time.str().c_str(), colors::white());
			txt_turn.draw(window_space::point{0, 50});
		}

		{ // Draw q- and r-axes.
			region_tile::point origin{0, 0};
			region_tile::point q_axis{5, 0};
			region_tile::point r_axis{0, 5};
			camera().draw_lines({to_world(origin), to_world(q_axis)}, colors::green());
			camera().draw_lines({to_world(origin), to_world(r_axis)}, colors::red());
		}

		_txt_test1->draw(window_space::point{0, 0});
		the_renderer().draw_box(window_space::box{window_space::point{0, 0}, window_space::vector{15, 15}}, 1, colors::blue(), colors::clear());

		//! @todo Enable to test polygon rendering after adding support for non-convex polygons.
#if 0
		{ // Draw hexagon.
			view_space::polygon hexagon;
			for (int i = 0; i < 6; ++i) {
				auto const angle = constants::tau / 6 * i;
				auto const offset = view_space::vector{static_cast<float>(cos(angle) * 50), static_cast<float>(sin(angle) * 50)};
				hexagon.push_back(view_space::point{50.0f, 50.0f} + offset);
			}
			bool const hovering = view_space::contains(hexagon, to_view_space(the_input().mouse_position()));
			float border_width = abs(static_cast<float>(10 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			the_renderer().draw_polygon(hexagon, border_width, colors::white(), hovering ? colors::teal() : colors::orange());

			// Reverse hexagon and draw again. (Testing winding-order detection.)
			auto reverse_polygon = view_space::polygon(hexagon.rbegin(), hexagon.rend());
			for (auto& v : reverse_polygon) { v.y() += 100.0f; }
			the_renderer().draw_polygon(reverse_polygon, border_width, colors::white(), colors::clear());
		}
		{ // Draw right triangles. (Different border widths and winding orders.)
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{100.0f, 0.0f}
					, view_space::point{200.0f, 0.0f}
					, view_space::point{100.0f, 100.0f}
					}
				, 1.0f
				, colors::red()
				, colors::cyan()
				);
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{100.0f, 100.0f}
					, view_space::point{100.0f, 200.0f}
					, view_space::point{200.0f, 100.0f}
					}
				, 5.0f
				, colors::cyan()
				, colors::red()
				);
		}
		{ // Draw non-convex polygon.
			float border_width = abs(static_cast<float>(20 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{200.0f, 0.0f}
					, view_space::point{300.0f, 0.0f}
					, view_space::point{250.0f, 50.0f}
					, view_space::point{300.0f, 100.0f}
					, view_space::point{200.0f, 100.0f}
					}
				, border_width
				, colors::lime()
				, colors::magenta()
				);
		}
		{ // Draw a star outline.
			view_space::point const center{350.0f, 50.0f};
			view_space::polygon star;
			auto start_angle = 0;// constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4;
			for (int i = 0; i < 10; ++i) {
				float const radius = i & 1 ? 20.0f : 50.0f;
				auto const angle = constants::tau / 10 * i + start_angle;
				auto const offset = view_space::vector{static_cast<float>(cos(angle) * radius), static_cast<float>(sin(angle) * radius)};
				star.push_back(center + offset);
			}
			float border_width = abs(static_cast<float>(8 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			//the_renderer().draw_polygon(star_vertices, colors::lime(), Fill::outline);
			//the_renderer().draw_polygon(star_vertices, colors::lime(), Fill::solid);
			the_renderer().draw_polygon(star, border_width, colors::red(0.5f), colors::blue(0.5f));
		}
#endif
	}

	game::update_result game::update_playing() {
		// Update camera.
		_camera->update();

		// Update HUD.
		_hud->update();

		// Update dialogs.
		if (!_dialogs.empty()) {
			if (_dialogs.front()->update() == dialog::state::closed) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else if (_player_action_dialog) {
			if (_player_action_dialog->update() == dialog::state::closed) {
				_player_action_dialog = nullptr;
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish acting yet.
			while (being* next_ready_being = _region->next_ready_being()) {
				if (next_ready_being->id == _player_being_id) {
					// Update the player view before the player acts.
					update_player_view();
				}
				[[maybe_unused]] auto complete = next_ready_being->act();
				if (next_ready_being->id == _player_being_id) {
					// Update the player view after the player acts.
					update_player_view();
				}

				if (_player_action_dialog || !_dialogs.empty()) {
					// Awaiting player input to complete current action. Stop taking turns, and start at the next agent once this action is complete.
					break;
				}
			}
			if (!_player_action_dialog && _dialogs.empty()) {
				// Update the region.
				_region->update();
				// Update the player view after each time unit passes.
				update_player_view();
			}
		}

		// Update world renderer.
		_world_renderer->update();

		// Disable camera controls during dialogs (except player action dialog).
		if (_dialogs.empty()) {
			// Pan camera.
			if (the_input().down(mouse_button::middle)) {
				window_space::vector mouse_shift = the_input().last_mouse_position() - the_input().mouse_position();
				auto pan = game_space::vector{static_cast<double>(mouse_shift.x()), static_cast<double>(-mouse_shift.y())} / _camera->zoom();
				pan.rotate(_camera->angle());
				_camera->pan(pan);
			}
			double const pan_amount = 10.0;
			if (the_input().down(SDLK_KP_8)) {
				_camera->pan(game_space::vector{0.0, pan_amount} / _camera->zoom());
			}
			if (the_input().down(SDLK_KP_4)) {
				_camera->pan(game_space::vector{-pan_amount, 0.0} / _camera->zoom());
			}
			if (the_input().down(SDLK_KP_2)) {
				_camera->pan(game_space::vector{0.0, -pan_amount} / _camera->zoom());
			}
			if (the_input().down(SDLK_KP_6)) {
				_camera->pan(game_space::vector{pan_amount, 0.0} / _camera->zoom());
			}

			// Scale camera.
			_camera->zoom_factor(1 + the_input().scroll() / 10.0f);

			// Rotate camera.
			game_space::radians angle = _camera->angle();
			if (the_input().down(SDLK_KP_9)) {
				_camera->rotate((-game_space::radians::circle() / 6.0 - angle) / 20.0);
			} else if (the_input().down(SDLK_KP_7)) {
				_camera->rotate((game_space::radians::circle() / 6.0 - angle) / 20.0);
			}
		}

		// Camera follow.
		static bool follow = true;
		if (the_input().presses(SDLK_BACKSPACE) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			if (being* player = beings.ptr(*_player_being_id)) {
				constexpr double recoil = 0.2;

				game_space::vector to_player = to_world(player->coords) - _camera->position();
				_camera->position(_camera->position() + recoil * to_player);

				double to_zoom_1 = 1.0 - _camera->zoom();
				_camera->zoom(_camera->zoom() + recoil * to_zoom_1);
				if (abs(_camera->zoom() - 1.0) < 0.001) {
					_camera->zoom(1.0);
				}

				// Only snap camera angle back if the player isn't actively rotating it.
				if (!the_input().down(SDLK_KP_9) && !the_input().down(SDLK_KP_7)) {
					_camera->angle(_camera->angle() * (1 - recoil));
				}
			}
		}
		return update_result::continue_game;
	}

	void game::update_player_view() {
		//! @todo Do something reasonable with the player view when the player dies.
		being const* player_being = beings.cptr(*_player_being_id);
		if (player_being && player_being->mortality != ql::mortality::dead) {
			// Update the player's world view.
			_player->update_world_view();
			// Update the world renderer's world view.
			_world_renderer->update_view(_player->world_view(), _player->poll_perceived_effects());
		}
	}
}
