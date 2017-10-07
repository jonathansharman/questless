//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "Game.h"

#include <sstream>
#include <thread>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "animation/EntityAnimator.h"
#include "world/coordinates.h"
#include "world/LightSource.h"

//! @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/Agent.h"
#include "agents/Player.h"
#include "items/magic/Gatestone.h"
#include "entities/beings/goblinoids/Goblin.h"
#include "entities/beings/Human.h"
#include "items/Scroll.h"
#include "items/weapons/Arrow.h"
#include "items/weapons/Bow.h"
#include "items/weapons/Quarterstaff.h"
#include "items/weapons/Quiver.h"
#include "sdl/ShaderProgram.h"
#include "spell/all-spells.h"

using std::move;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::string;
using std::function;

using namespace std::chrono;

using namespace sdl;
using namespace units;

namespace questless
{
	Id<Being> Game::get_being_id(uptr<Being> const& being) { return being->id; }
	Id<Object> Game::get_object_id(uptr<Object> const& object) { return object->id; }
	Id<Item> Game::get_item_id(uptr<Item> const& item) { return item->id; }
	Id<LightSource> Game::get_light_source_id(uptr<LightSource> const& light_source) { return light_source->id; }

	Game::Game(bool fullscreen)
		: _player_being_id{std::nullopt}
		, _splash_sound_played{false}
		, _state{State::splash}
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
			window(make_unique<Window>("Questless", "resources/textures/icon.png", true));
		} else {
			window(make_unique<Window>("Questless", "resources/textures/icon.png", false, _dflt_window_width, _dflt_window_height, true, true, true, false));
		}

		{ // Initialize OpenGL.
			// Create OpenGL context.
			SDL_GLContext gl_context = SDL_GL_CreateContext(window().sdl_ptr()); //! @todo Destroy context.
			if (!gl_context) {
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
			dflt_program(std::make_unique<ShaderProgram>
				( contents_of_file("resources/shaders/dflt.vert").c_str()
				, contents_of_file("resources/shaders/dflt.frag").c_str()
				));
			solid_program(std::make_unique<ShaderProgram>
				( contents_of_file("resources/shaders/solid.vert").c_str()
				, contents_of_file("resources/shaders/solid.frag").c_str()
				));
			
			// Set clear color.
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			{ // Set default program uniforms.
				dflt_program().use();

				GLint viewport_size_uniform = dflt_program().get_uniform_handle("viewport_size");
				int window_width = window().width();
				int window_height = window().height();
				glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

				GLint flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
				glUniform1i(flip_y_uniform, GL_FALSE);
			}
			{ // Set solid program uniforms.
				solid_program().use();

				GLint viewport_size_uniform = solid_program().get_uniform_handle("viewport_size");
				int window_width = window().width();
				int window_height = window().height();
				glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

				GLint flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
				glUniform1i(flip_y_uniform, GL_FALSE);
			}

			// Set blend function.
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		// Initialize renderer.
		renderer(make_unique<Renderer>(window(), window().width(), window().height()));

		_camera = make_unique<Camera>(GameSpace::Point{0.0, 0.0});

		if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
			throw std::runtime_error("Failed to initialize IMG. SDL Error: " + string{SDL_GetError()});
		}

		// Initialize and load fonts.

		if (TTF_Init()) {
			throw std::runtime_error("Failed to initialize TTF. SDL Error: " + string{SDL_GetError()});
		}

		_fnt_20pt = make_unique<Font>("resources/fonts/firamono.ttf", 20, SDL_BLENDMODE_BLEND);

		// Initialize sound.

	    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)) {
			throw std::runtime_error("Failed to initialize Mix. SDL Error: " + string{SDL_GetError()});
	    }

		// Initialize and load menu resources.

		_hud = make_unique<HUD>();

		// Load textures and graphics.

		load_textures();

		// Load sounds.

		_sfx_splash = make_unique<Sound>("resources/sounds/splash.wav");

		// Initialize game state.

		for (int i = 0; i < _splash_flames_count; ++i) {
			_splash_flame_positions.emplace_back(uniform(0, window().width() - 1), (i + 1) * window().height() / _splash_flames_count);
		}

		_time_last_state_change = clock::now();
		_state = State::splash;
	}

	Game::~Game()
	{
		// Need to delete all SDL-related variables prior to calling the Close/Quit functions.

		// Delete sounds.

		// Delete textures.

		// Delete fonts.

		_fnt_20pt = nullptr;

		// Delete renderer.

		renderer(nullptr);

		// Delete window.

		window(nullptr);

		// Quit SDL components.

		Mix_CloseAudio();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Game::load_textures()
	{
		_txt_test1 = make_unique<Texture>("resources/textures/test1.png");
		_txt_test2 = make_unique<Texture>("resources/textures/test2.png");
		_txt_test3 = make_unique<Texture>("resources/textures/test3.png");
		_txt_test_even = make_unique<Texture>("resources/textures/test-even.png");

		_txt_splash_logo = make_unique<Texture>("resources/textures/splash/logo.png");

		_txt_splash_flame = make_unique<Texture>("resources/textures/splash/flame.png");

		_txt_hex_highlight = make_unique<Texture>("resources/textures/terrain/tile.png");

		_txt_hex_circle = make_unique<Texture>("resources/textures/ui/hex_circle.png");
	}

	Complete Game::add_dialog(uptr<Dialog> dialog)
	{
		_dialogs.push_back(move(dialog));
		return Complete{};
	}

	void Game::query_player_choice(function<void(PlayerActionDialog::Choice)> cont)
	{
		_player_action_dialog = make_unique<PlayerActionDialog>(*_hud, move(cont));
	}

	void Game::add_effect(sptr<Effect> const& effect)
	{
		int range = effect->range();
		auto origin = effect->origin();

		RegionTile::Point min_tile_coords{origin.q - range, origin.r - range};
		RegionTile::Point max_tile_coords{origin.q + range, origin.r + range};

		RegionSection::Point min_section_coords = Section::region_section_coords(min_tile_coords);
		RegionSection::Point max_section_coords = Section::region_section_coords(max_tile_coords);

		for (int r = min_section_coords.r; r <= max_section_coords.r; ++r) {
			for (int q = min_section_coords.q; q <= max_section_coords.q; ++q) {
				RegionSection::Point section_coords{q, r};
				Section* section = _region->section(section_coords);
				if (section) {
					for (Being& being : section->beings) {
						being.agent().perceive(effect);
					}
				}
			}
		}
	}

	// Main Game Logic

	void Game::run()
	{
		GameSeconds accrued_time = GameSeconds::zero();
		clock::time_point last_update_time = clock::now();
		while (update(accrued_time, last_update_time) == UpdateResult::continue_game) {
			render();
		}
		if (_region) {
			_region->save("Slot1");
		}
	}

	Game::UpdateResult Game::update(GameSeconds& accrued_time, clock::time_point& last_update_time)
	{
		input().update();

		if (input().quit() || input().alt() && input().presses(SDLK_F4)) {
			return UpdateResult::game_over;
		}

		//! @todo Sometimes resizing the window causes a crash.
		if (input().window_restored()) {
			//! @todo Save previous window size and restore it here.
		}
		if (input().window_resized()) {
			window().recreate();
			renderer(make_unique<Renderer>(window(), window().width(), window().height()));
		}

		{ // Perform state-specific updates.
			UpdateResult result = UpdateResult::game_over;
			switch (_state) {
				case State::splash:
					result = update_splash();
					break;
				case State::menu:
					result = update_menu();
					break;
				case State::playing:
					result = update_playing();
					break;
				default:
					break;
			}
			if (result == UpdateResult::game_over) return UpdateResult::game_over;
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

		return UpdateResult::continue_game;
	}

	void Game::render()
	{
		////////////
		// Render //
		////////////

		switch (_state) {
			case State::splash:
				glClear(GL_COLOR_BUFFER_BIT);
				render_splash();
				break;
			case State::menu:
				glClear(GL_COLOR_BUFFER_BIT);
				render_menu();
				break;
			case State::playing:
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
		Texture txt_fps = _fnt_20pt->render(oss_fps.str().c_str(), colors::white());
		txt_fps.draw(ScreenSpace::Point(window().width() - 1, window().height() - 1), TextureSpace::align_right, TextureSpace::align_bottom);

		// Swap buffers to update the screen.
		SDL_GL_SwapWindow(window().sdl_ptr());
	}

	Game::UpdateResult Game::update_splash()
	{
		if (!_splash_sound_played) {
			_splash_sound_played = true;
			_sfx_splash->play();
		}

		for (ScreenSpace::Point& position : _splash_flame_positions) {
			position.y() += lround(_splash_flames_vy * frame_duration);
			if (position.y() < 0) {
				position.y() += window().height() + _txt_splash_flame->height();
				position.x() = uniform(0, window().width() - 1);
			}
		}

		if (input().any_presses() || clock::now() - _time_last_state_change >= _splash_duration) {
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
			_state = State::menu;
		}
		return UpdateResult::continue_game;
	}
	
	void Game::render_splash()
	{
		float intensity;
		if (clock::now() - _time_last_state_change < _splash_fade_in_duration) {
			auto ms_fading_in = duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count();
			intensity = static_cast<float>(ms_fading_in / _splash_fade_in_duration.count());
		} else {
			auto ms_fading_out = duration_cast<GameSeconds>(clock::now() - _time_last_state_change - _splash_fade_in_duration).count();
			intensity = static_cast<float>(1.0 - ms_fading_out / _splash_fade_out_duration.count());
		}

		ScreenSpace::Point logo_position = window().center() + ScreenSpace::Vector{uniform(-_splash_logo_jiggle, _splash_logo_jiggle), uniform(-_splash_logo_jiggle, _splash_logo_jiggle)};
		_txt_splash_logo->draw(logo_position, TextureSpace::align_center, TextureSpace::align_middle, colors::ColorFactor{intensity, intensity, intensity, 1.0f});

		for (ScreenSpace::Point position : _splash_flame_positions) {
			_txt_splash_flame->draw(position, TextureSpace::align_center, TextureSpace::align_bottom, colors::ColorFactor{intensity, intensity, intensity, 1.0f});
		}
	}

	Game::UpdateResult Game::update_menu()
	{
		_main_menu.update();
		for (auto const& option : _main_menu.poll_selections()) {
			if (option.first == "Questless") {
				if (option.second == "Continue" || option.second == "Begin Anew") {
					_region = make_unique<Region>("Region1");
					//_region = make_unique<Region>("Slot1", "Region1");

					{ // Spawn the player's being.
						auto player_being = make_unique<Human>(Agent::make<Player>);
						_player = dynamic_cast<Player*>(&player_being->agent());
						_player_being_id = player_being->id;
						player_being->inventory.add(items.add(make_unique<Scroll>(make_unique<spell::Shock>())).id);
						player_being->inventory.add(items.add(make_unique<Scroll>(make_unique<spell::Heal>())).id);
						player_being->inventory.add(items.add(make_unique<Scroll>(make_unique<spell::Teleport>())).id);
						player_being->inventory.add(items.add(make_unique<Scroll>(make_unique<spell::EagleEye>())).id);
						player_being->inventory.add(items.add(make_unique<Quarterstaff>()).id);
						player_being->inventory.add(items.add(make_unique<Bow>()).id);
						{
							Inventory inventory;
							constexpr int arrow_count = 20;
							for (int i = 0; i < arrow_count; ++i) {
								inventory.add(items.add(make_unique<Arrow>()).id);
							}
							player_being->inventory.add(items.add(make_unique<Quiver>(std::move(inventory))).id);
						}
						player_being->inventory.add(items.add(make_unique<Gatestone>(100.0, 100.0, 0.0, spell::Color::green)).id);
						_region->spawn_player(move(player_being));
					}
					// Pass the player's being ID to the HUD.
					_hud->set_player_being_id(*_player_being_id);
					// Update the player's initial world view.
					_player->update_world_view();
					// Initialize the world renderer.
					_world_renderer = make_unique<WorldRenderer>(_player->world_view());
					// Set the camera position relative to the player's being.
					_camera->position(GameSpace::Point{Layout::dflt().to_world(beings.cref(*_player_being_id).coords)});

					_time_last_state_change = clock::now();
					_state = State::playing;
				} else if (option.second  == "Quit") {
					return UpdateResult::game_over;
				}
			}
		}
		return UpdateResult::continue_game;
	}

	void Game::render_menu()
	{
		_main_menu.draw(window().center(), ScreenSpace::align_center, ScreenSpace::align_middle);
	}

	void Game::render_playing()
	{
		_world_renderer->draw_terrain();

		if (input().pressed(MouseButton::right)) {
			_point_clicked_rounded = Layout::dflt().to_world(_camera->tile_hovered());
		}
		_camera->draw(*_txt_hex_highlight, Layout::dflt().to_world(_camera->tile_hovered()), Origin{TextureSpace::Vector::zero()}, colors::white_factor(0.5f));
		_camera->draw(*_txt_hex_circle, _point_clicked_rounded);

		_world_renderer->draw_entities();
		_world_renderer->draw_effects();

		// Draw HUD.
		_hud->draw();

		// Draw dialogs.
		for (auto const& dialog : _dialogs) {
			dialog->draw();
		}

		{
			std::ostringstream ss_cam_coords;
			ss_cam_coords.setf(std::ios::fixed);
			ss_cam_coords.precision(2);
			ss_cam_coords << "Cam: ((" << _camera->position().x() << ", " << _camera->position().y() << "), ";
			ss_cam_coords << _camera->angle().count() << ", " << _camera->zoom() << ")";
			Texture txt_cam_coords = _fnt_20pt->render(ss_cam_coords.str().c_str(), colors::white());
			txt_cam_coords.draw(ScreenSpace::Point{0, 0});
		}
		{
			auto cam_hex_coords = Layout::dflt().to_hex_coords<RegionTile::Point>(_camera->position());
			std::ostringstream ss_cam_hex_coords;
			ss_cam_hex_coords << "Cam hex: (" << cam_hex_coords.q << ", " << cam_hex_coords.r << ")";
			Texture txt_cam_hex_coords = _fnt_20pt->render(ss_cam_hex_coords.str().c_str(), colors::white());
			txt_cam_hex_coords.draw(ScreenSpace::Point{0, 25});
		}
		{
			std::ostringstream ss_time;
			double const time_of_day = _region->time_of_day();
			std::string time_name;
			switch (_region->period_of_day()) {
				case Region::PeriodOfDay::morning:
					time_name = "Morning";
					break;
				case Region::PeriodOfDay::afternoon:
					time_name = "Afternoon";
					break;
				case Region::PeriodOfDay::dusk:
					time_name = "Dusk";
					break;
				case Region::PeriodOfDay::evening:
					time_name = "Evening";
					break;
				case Region::PeriodOfDay::night:
					time_name = "Night";
					break;
				case Region::PeriodOfDay::dawn:
					time_name = "Dawn";
					break;
			}
			ss_time << "Time: " << _region->time() << " (" << time_of_day << ", " << time_name << ')';
			Texture txt_turn = _fnt_20pt->render(ss_time.str().c_str(), colors::white());
			txt_turn.draw(ScreenSpace::Point{0, 50});
		}

		// Draw q- and r-axes.
		RegionTile::Point origin{0, 0};
		RegionTile::Point q_axis{5, 0};
		RegionTile::Point r_axis{0, 5};
		camera().draw_lines({Layout::dflt().to_world(origin), Layout::dflt().to_world(q_axis)}, colors::green());
		camera().draw_lines({Layout::dflt().to_world(origin), Layout::dflt().to_world(r_axis)}, colors::red());

		_txt_test1->draw(ScreenSpace::Point{0, 0});
		renderer().draw_box(ScreenSpace::Box{ScreenSpace::Point{0, 0}, ScreenSpace::Vector{15, 15}}, 1, colors::blue(), colors::clear());

		//! @todo Uncomment to test polygon rendering after adding support for non-convex polygons.
#if 0
		{ // Draw hexagon.
			std::vector<ViewSpace::Point> vertices;
			for (int i = 0; i < 6; ++i) {
				auto const angle = constants::tau / 6 * i;
				auto const offset = ViewSpace::Vector{static_cast<float>(cos(angle) * 50), static_cast<float>(sin(angle) * 50)};
				vertices.push_back(ViewSpace::Point{50.0f, 50.0f} + offset);
			}
			float border_width = abs(static_cast<float>(10 * sin(constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4)));
			renderer().draw_polygon(vertices, border_width, colors::white(), colors::orange());

			// Reverse hexagon and draw again. (Testing winding-order detection.)
			auto reverse_vertices = std::vector<ViewSpace::Point>(vertices.rbegin(), vertices.rend());
			for (auto& v : reverse_vertices) { v.y() += 100.0f; }
			renderer().draw_polygon(reverse_vertices, border_width, colors::white(), colors::clear());
		}
		{ // Draw right triangles. (Different border widths and winding orders.)
			renderer().draw_polygon
				(
					{ ViewSpace::Point{100.0f, 0.0f}
					, ViewSpace::Point{200.0f, 0.0f}
					, ViewSpace::Point{100.0f, 100.0f}
					}
				, 1.0f
				, colors::red()
				, colors::cyan()
				);
			renderer().draw_polygon
				(
					{ ViewSpace::Point{100.0f, 100.0f}
					, ViewSpace::Point{100.0f, 200.0f}
					, ViewSpace::Point{200.0f, 100.0f}
					}
				, 5.0f
				, colors::cyan()
				, colors::red()
				);
		}
		{ // Draw non-convex polygon.
			float border_width = abs(static_cast<float>(20 * sin(constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4)));
			renderer().draw_polygon
				(
					{ ViewSpace::Point{200.0f, 0.0f}
					, ViewSpace::Point{300.0f, 0.0f}
					, ViewSpace::Point{250.0f, 50.0f}
					, ViewSpace::Point{300.0f, 100.0f}
					, ViewSpace::Point{200.0f, 100.0f}
					}
				, border_width
				, colors::lime()
				, colors::magenta()
				);
		}
		{ // Draw a star outline.
			ViewSpace::Point const center{350.0f, 50.0f};
			std::vector<ViewSpace::Point> star_vertices;
			auto start_angle = 0;// constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4;
			for (int i = 0; i < 10; ++i) {
				float const radius = i & 1 ? 20.0f : 50.0f;
				auto const angle = constants::tau / 10 * i + start_angle;
				auto const offset = ViewSpace::Vector{static_cast<float>(cos(angle) * radius), static_cast<float>(sin(angle) * radius)};
				star_vertices.push_back(center + offset);
			}
			float border_width = abs(static_cast<float>(8 * sin(constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4)));
			//renderer().draw_polygon(star_vertices, colors::lime(), Fill::outline);
			//renderer().draw_polygon(star_vertices, colors::lime(), Fill::solid);
			renderer().draw_polygon(star_vertices, border_width, colors::red(0.5f), colors::blue(0.5f));
		}
#endif
	}

	Game::UpdateResult Game::update_playing()
	{
		// Update camera.
		_camera->update();

		// Update HUD.
		_hud->update();

		// Update dialogs.
		if (!_dialogs.empty()) {
			if (_dialogs.front()->update() == Dialog::State::closed) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else if (_player_action_dialog) {
			if (_player_action_dialog->update() == Dialog::State::closed) {
				_player_action_dialog = nullptr;
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish acting yet.
			while (Being* next_ready_being = _region->next_ready_being()) {
				if (next_ready_being->id == _player_being_id) {
					// Update the player view before the player acts.
					update_player_view();
				}
				next_ready_being->act();
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
			if (input().down(MouseButton::middle)) {
				ScreenSpace::Vector mouse_shift = input().last_mouse_position() - input().mouse_position();
				auto pan = GameSpace::Vector{static_cast<double>(mouse_shift.x()), static_cast<double>(-mouse_shift.y())} / _camera->zoom();
				pan.rotate(_camera->angle());
				_camera->pan(pan);
			}
			double const pan_amount = 10.0;
			if (input().down(SDLK_KP_8)) {
				_camera->pan(GameSpace::Vector{0.0, pan_amount} / _camera->zoom());
			}
			if (input().down(SDLK_KP_4)) {
				_camera->pan(GameSpace::Vector{-pan_amount, 0.0} / _camera->zoom());
			}
			if (input().down(SDLK_KP_2)) {
				_camera->pan(GameSpace::Vector{0.0, -pan_amount} / _camera->zoom());
			}
			if (input().down(SDLK_KP_6)) {
				_camera->pan(GameSpace::Vector{pan_amount, 0.0} / _camera->zoom());
			}

			// Scale camera.
			_camera->zoom_factor(1 + input().scroll() / 10.0f);

			// Rotate camera.
			GameSpace::Radians angle = _camera->angle();
			if (input().down(SDLK_KP_9)) {
				_camera->rotate((-GameSpace::Radians::circle() / 6.0 - angle) / 20.0);
			} else if (input().down(SDLK_KP_7)) {
				_camera->rotate((GameSpace::Radians::circle() / 6.0 - angle) / 20.0);
			}
		}

		// Camera follow.
		static bool follow = true;
		if (input().presses(SDLK_BACKSPACE) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			if (Being* player = beings.ptr(*_player_being_id)) {
				constexpr double recoil = 0.2;

				GameSpace::Vector to_player = Layout::dflt().to_world(player->coords) - _camera->position();
				_camera->position(_camera->position() + recoil * to_player);

				double to_zoom_1 = 1.0 - _camera->zoom();
				_camera->zoom(_camera->zoom() + recoil * to_zoom_1);
				if (abs(_camera->zoom() - 1.0) < 0.001) {
					_camera->zoom(1.0);
				}

				// Only snap camera angle back if the player isn't actively rotating it.
				if (!input().down(SDLK_KP_9) && !input().down(SDLK_KP_7)) {
					_camera->angle(_camera->angle() * (1 - recoil));
				}
			}
		}
		return UpdateResult::continue_game;
	}

	void Game::update_player_view()
	{
		//! @todo Do something reasonable with the player view when the player dies.
		Being* player_being = beings.ptr(*_player_being_id);
		if (player_being && !player_being->dead) {
			// Update the player's world view.
			_player->update_world_view();
			// Update the world renderer's world view.
			_world_renderer->update_view(_player->world_view(), _player->poll_perceived_effects());
		}
	}
}
