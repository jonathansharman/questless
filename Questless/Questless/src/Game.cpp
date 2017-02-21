/**
* @file    Questless.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implemention for the Questless Game class.
*/

#include <sstream>
using std::ostringstream;

#include "Game.h"
#include "animation/EntityAnimator.h"
#include "world/coordinates.h"

/// @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/Agent.h"
#include "agents/Player.h"
#include "entities/beings/Human.h"
#include "entities/beings/goblinoids/Goblin.h"
#include "items/Scroll.h"
#include "spell/LightningBolt.h"
#include "spell/Heal.h"
#include "spell/Teleport.h"
#include "items/weapons/Quarterstaff.h"

using std::move;
using std::unique_ptr;
using std::make_unique;
using std::dynamic_pointer_cast;
using std::string;
using std::function;

using namespace std::chrono;

using namespace sdl;
using namespace units;

namespace questless
{
	Game::Game(bool fullscreen)
		: _game_over{false}
		, _splash_sound_played{false}
		, _state{State::splash}
		, _time{0.0}
		, _mnu_main{480, 640}
	{
		// Initialize video.

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
			throw std::runtime_error("Failed to initialize SDL. SDL Error: " + string{SDL_GetError()});
		}
		if (fullscreen) {
			_window = make_unique<Window>("Questless", "resources/textures/icon.png", true);
		} else {
			_window = make_unique<Window>("Questless", "resources/textures/icon.png", false, _dflt_window_width, _dflt_window_height, true, true, true, false);
		}

		renderer(make_unique<Renderer>(*_window, _window->width(), _window->height()));

		_camera = make_unique<Camera>(*_window, GamePoint{0, 0});

		/// @todo Make render quality a game setting.
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

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

		_hud = make_unique<HUDController>(*this, *_window);

		// Load textures and graphics.

		load_textures();

		// Load sounds.

		_sfx_splash = make_unique<Sound>("resources/sounds/splash.wav");

		// Initialize game state.

		for (int i = 0; i < _splash_flames_count; ++i) {
			_splash_flame_positions.emplace_back(uniform(0, _window->width() - 1), (i + 1) * _window->height() / _splash_flames_count);
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

		_window = nullptr;

		// Quit SDL components.

		Mix_CloseAudio();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Game::load_textures()
	{
		_txt_test = make_unique<Texture>("resources/textures/test.png", renderer(), SDL_BLENDMODE_BLEND);
		_txt_test2 = make_unique<Texture>("resources/textures/test2.png", renderer(), SDL_BLENDMODE_BLEND);
		_txt_test3 = make_unique<Texture>("resources/textures/test3.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_splash_logo = make_unique<Texture>("resources/textures/splash/logo.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_splash_flame = make_unique<Texture>("resources/textures/splash/flame.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_hex_highlight = make_unique<Texture>("resources/textures/terrain/tile.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_hex_circle = make_unique<Texture>("resources/textures/ui/hex_circle.png", renderer(), SDL_BLENDMODE_BLEND);
	}

	Action::Complete Game::add_dialog(Dialog::ptr dialog)
	{
		_dialogs.push_back(move(dialog));
		return Action::Complete{};
	}

	void Game::query_player_choice(function<void(PlayerActionDialog::Choice)> cont)
	{
		_player_action_dialog = make_unique<PlayerActionDialog>(*_hud, move(cont));
	}

	void Game::add_effect(Effect::ptr const& effect)
	{
		int range = effect->range();
		auto origin = effect->origin();

		RegionTileCoords min_tile_coords{origin.q - range, origin.r - range};
		RegionTileCoords max_tile_coords{origin.q + range, origin.r + range};

		RegionSectionCoords min_section_coords = _region->containing_section_coords(min_tile_coords);
		RegionSectionCoords max_section_coords = _region->containing_section_coords(max_tile_coords);

		for (int r = min_section_coords.r; r <= max_section_coords.r; ++r) {
			for (int q = min_section_coords.q; q <= max_section_coords.q; ++q) {
				RegionSectionCoords section_coords{q, r};
				Section* section = _region->section(section_coords);
				if (section) {
					for (Being& being : section->beings()) {
						being.agent().perceive(effect);
					}
				}
			}
		}
	}

	// Main Game Logic

	void Game::run()
	{
		game_loop();
		if (_region != nullptr) {
			_region->save("Slot1");
		}
	}

	void Game::game_loop()
	{
		GameSeconds accrued_time = GameSeconds::zero();
		clock::time_point last_update_time = clock::now();
		for (;;) {
			// Update

			_input.update();

			if (_input.quit() || _input.alt() && _input.presses(SDLK_F4)) {
				_game_over = true;
				return;
			}

			/// @todo Sometimes resizing the window causes a crash.
			if (_input.window_restored()) {
				/// @todo Save previous window size and restore it here.
			}
			if (_input.window_resized()) {
				_window->recreate();
				renderer(make_unique<Renderer>(*_window, _window->width(), _window->height()));
			}

			switch (_state) {
				case State::splash:
					update_splash();
					break;
				case State::menu:
					update_menu();
					break;
				case State::playing:
					update_playing();
					break;
				default:
					break;
			}

			if (_game_over) {
				break;
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

			// Render

			switch (_state) {
				case State::splash:
					renderer().clear(_splash_clear_color);
					render_splash();
					break;
				case State::menu:
					renderer().clear(_menu_clear_color);
					render_menu();
					break;
				case State::playing:
					renderer().clear(_playing_clear_color);
					render_playing();
					break;
			}

			// Draw FPS counter.

			double fps_buffer_sum = 0.0;
			for (double fps : _fps_buffer) {
				fps_buffer_sum += fps;
			}
			ostringstream oss_fps;
			oss_fps.setf(std::ios::fixed);
			oss_fps.precision(2);
			oss_fps << fps_buffer_sum / _fps_buffer.size();
			Texture txt_fps = _fnt_20pt->render(oss_fps.str(), renderer(), Color::white());
			txt_fps.draw(ScreenPoint(_window->width() - 1, _window->height() - 1), HAlign::right, VAlign::bottom);

			// Present rendering.

			renderer().present();
		}
	}

	void Game::update_splash()
	{
		if (!_splash_sound_played) {
			_splash_sound_played = true;
			_sfx_splash->play();
		}

		for (ScreenPoint& position : _splash_flame_positions) {
			position.y += lround(_splash_flames_vy * frame_duration);
			if (position.y < 0) {
				position.y += _window->height() + _txt_splash_flame->height();
				position.x = uniform(0, _window->width() - 1);
			}
		}

		if (_input.any_presses() || clock::now() - _time_last_state_change >= _splash_duration) {
			// End splash.

			_splash_flame_positions.clear();
			_txt_splash_logo = nullptr;
			_txt_splash_flame = nullptr;
			_sfx_splash->stop();

			// Initialize menu.

			// Add pages.
			_mnu_main.add_page("Questless");
			_mnu_main.add_page("Settings");
			// Add options.
			_mnu_main.add_option("Questless", "Continue");
			_mnu_main.add_option("Questless", "Begin Anew");
			_mnu_main.add_option("Questless", "Settings", "Settings");
			_mnu_main.add_option("Questless", "Quit");
			_mnu_main.add_option("Settings", "Save", "Questless");
			_mnu_main.add_option("Settings", "Cancel", "Questless");

			_time_last_state_change = clock::now();
			_state = State::menu;
		}
	}
	
	void Game::render_splash()
	{
		if (clock::now() - _time_last_state_change < _splash_fade_in_duration) {
			auto ms_fading_in = duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count();
			uint8_t intensity = percentage_to_byte(static_cast<double>(ms_fading_in / _splash_fade_in_duration.count()));
			_txt_splash_logo->color(Color{intensity, intensity, intensity});
			_txt_splash_flame->color(Color{intensity, intensity, intensity});
		} else {
			auto ms_fading_out = duration_cast<GameSeconds>(clock::now() - _time_last_state_change - _splash_fade_in_duration).count();
			uint8_t intensity = percentage_to_byte(1 - static_cast<double>(ms_fading_out / _splash_fade_out_duration.count()));
			_txt_splash_logo->color(Color{intensity, intensity, intensity});
			_txt_splash_flame->color(Color{intensity, intensity, intensity});
		}

		ScreenPoint logo_position = _window->center() + ScreenVector{uniform(-_splash_logo_jiggle, _splash_logo_jiggle), uniform(-_splash_logo_jiggle, _splash_logo_jiggle)};
		_txt_splash_logo->draw(logo_position, HAlign::center, VAlign::middle);

		for (ScreenPoint position : _splash_flame_positions) {
			_txt_splash_flame->draw(position, HAlign::center, VAlign::bottom);
		}
	}

	void Game::update_menu()
	{
		_mnu_main.update(_input);
		for (auto const& option : _mnu_main.poll_selections()) {
			if (option.first == "Questless") {
				if (option.second == "Continue" || option.second == "Begin Anew") {
					_region = make_unique<Region>(*this, "Region1");
					//_region = make_unique<Region>(*this, "Slot1", "Region1");

					{ // Spawn the player's being.
						Being::ptr player_being = make_unique<Human>(*this, Agent::make<Player>, BeingId::next());
						_player = dynamic_cast<Player*>(&player_being->agent());
						_player_being_id = player_being->id();
						player_being->give_item(make_unique<Scroll>(make_unique<spell::LightningBolt>()));
						player_being->give_item(make_unique<Scroll>(make_unique<spell::Heal>()));
						player_being->give_item(make_unique<Scroll>(make_unique<spell::Teleport>()));
						player_being->give_item(make_unique<Quarterstaff>());
						_region->spawn_player(move(player_being));
					}
					// Pass the player's being ID to the HUD.
					_hud->player_id(_player_being_id);
					// Update the player's initial world view.
					_player->update_world_view();
					// Initialize the world renderer.
					_world_renderer = make_unique<WorldRenderer>(_player->world_view());
					// Set the camera position relative to the player's being.
					_camera->position(GamePoint{Layout::dflt().to_world(being(_player_being_id)->coords)});

					_time_last_state_change = clock::now();
					_state = State::playing;
				} else if (option.second  == "Quit") {
					_game_over = true;
					return;
				}
			}
		}
	}

	void Game::render_menu()
	{
		_mnu_main.draw(_window->center(), HAlign::center, VAlign::middle);
	}

	void Game::render_playing()
	{
		//{
		//	constexpr int width = 20;
		//	constexpr int count = 18;
		//	for (int i = 0; i < count; ++i) {
		//		renderer().draw_rect(ScreenRect{_window->center().x - width * i, _window->center().y - width * i, 2 * width * i, 2 * width * i}, Color::white(), false);
		//	}
		//}

		_world_renderer->draw_terrain(*_camera);

		if (_input.pressed(MouseButton::right)) {
			_point_clicked_rounded = Layout::dflt().to_world(_camera->tile_hovered());
		}
		_camera->draw(*_txt_hex_highlight, Layout::dflt().to_world(_camera->tile_hovered()), Origin{boost::none}, Color::white(128));
		_camera->draw(*_txt_hex_circle, _point_clicked_rounded);

		_world_renderer->draw_objects(*this, *_camera);
		_world_renderer->draw_beings(*this, *_camera);
		_world_renderer->draw_effects(*this, *_camera);

		// Draw HUD.
		_hud->draw();

		// Draw dialogs.
		for (auto const& dialog : _dialogs) {
			dialog->draw(*_window);
		}

		{
			ostringstream ss_cam_coords;
			ss_cam_coords.setf(std::ios::fixed);
			ss_cam_coords.precision(2);
			ss_cam_coords << "Cam: ((" << _camera->position().x << ", " << _camera->position().y << "), ";
			ss_cam_coords << _camera->angle().count() << ", " << _camera->zoom() << ")";
			Texture txt_cam_coords = _fnt_20pt->render(ss_cam_coords.str(), renderer(), Color::white());
			txt_cam_coords.draw(ScreenPoint{0, 0});
		}
		{
			auto cam_hex_coords = Layout::dflt().to_hex_coords<RegionTileCoords>(_camera->position());
			ostringstream ss_cam_hex_coords;
			ss_cam_hex_coords << "Cam hex: (" << cam_hex_coords.q << ", " << cam_hex_coords.r << ")";
			Texture txt_cam_hex_coords = _fnt_20pt->render(ss_cam_hex_coords.str(), renderer(), Color::white());
			txt_cam_hex_coords.draw(ScreenPoint{0, 25});
		}
		{
			ostringstream ss_time;
			ss_time << "Time: " << _time;
			Texture txt_turn = _fnt_20pt->render(ss_time.str(), renderer(), Color::white());
			txt_turn.draw(ScreenPoint{0, 50});
		}

		// Draw q- and r-axes.
		RegionTileCoords origin{0, 0};
		RegionTileCoords q_axis{5, 0};
		RegionTileCoords r_axis{0, 5};
		camera().draw_lines({Layout::dflt().to_world(origin), Layout::dflt().to_world(q_axis)}, Color::green());
		camera().draw_lines({Layout::dflt().to_world(origin), Layout::dflt().to_world(r_axis)}, Color::red());
	}

	void Game::update_playing()
	{
		// Update camera.
		_camera->update(_input);

		// Update HUD.
		_hud->update(_input);

		// Update dialogs.
		if (!_dialogs.empty()) {
			if (_dialogs.front()->update(_input)) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else if (_player_action_dialog) {
			if (_player_action_dialog->update(_input)) {
				_player_action_dialog = nullptr;
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish acting yet.
			while (Being* next_ready_being = _region->next_ready_being()) {
				next_ready_being->act();

				if (_player_action_dialog || !_dialogs.empty()) {
					// Awaiting player input to complete current action. Stop taking turns, and start at the next agent once this action is complete.
					update_player_view();
					break;
				}
			}
			if (!_player_action_dialog && _dialogs.empty()) {
				// Advance the time.
				_time += 1.0;
				// Update the region.
				_region->update();
			}
		}

		// Update world renderer.
		_world_renderer->update();

		// Disable camera controls during dialogs (except player action dialog).
		if (_dialogs.empty()) {
			// Pan camera.
			if (_input.down(MouseButton::middle)) {
				ScreenVector mouse_shift = _input.last_mouse_position() - _input.mouse_position();
				auto pan = GameVector{static_cast<double>(mouse_shift.x), static_cast<double>(-mouse_shift.y)} / _camera->zoom();
				pan.rotate(_camera->angle());
				_camera->pan(pan);
			}
			double const pan_amount = 10.0;
			if (_input.down(SDLK_KP_8)) {
				_camera->pan(GameVector{0.0, pan_amount} / _camera->zoom());
			}
			if (_input.down(SDLK_KP_4)) {
				_camera->pan(GameVector{-pan_amount, 0.0} / _camera->zoom());
			}
			if (_input.down(SDLK_KP_2)) {
				_camera->pan(GameVector{0.0, -pan_amount} / _camera->zoom());
			}
			if (_input.down(SDLK_KP_6)) {
				_camera->pan(GameVector{pan_amount, 0.0} / _camera->zoom());
			}

			// Scale camera.
			_camera->zoom_factor(1 + _input.scroll() / 10.0f);

			// Rotate camera.
			GameRadians angle = _camera->angle();
			if (_input.down(SDLK_KP_9)) {
				_camera->rotate((-GameRadians::circle() / 6.0 - angle) / 20.0);
			} else if (_input.down(SDLK_KP_7)) {
				_camera->rotate((GameRadians::circle() / 6.0 - angle) / 20.0);
			}
		}

		// Camera follow.
		static bool follow = true;
		if (_input.presses(SDLK_BACKSPACE) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			if (Being* player = being(_player_being_id)) {
				constexpr double acceleration = 0.2;

				GameVector to_player = Layout::dflt().to_world(player->coords) - _camera->position();
				_camera->position(_camera->position() + acceleration * to_player);

				double to_zoom_1 = 1.0 - _camera->zoom();
				_camera->zoom(_camera->zoom() + acceleration * to_zoom_1);
				if (abs(_camera->zoom() - 1.0) < 0.001) {
					_camera->zoom(1.0);
				}

				// Only snap camera angle back if the player isn't actively rotating it.
				if (!_input.down(SDLK_KP_9) && !_input.down(SDLK_KP_7)) {
					_camera->angle(_camera->angle() * (1 - acceleration));
				}
			}
		}
	}

	Being* Game::_being(BeingId id) const
	{
		auto it = _beings.find(id);
		if (it == _beings.end()) {
			// Being no longer exists.
			return nullptr;
		} else {
			if (Being* being = std::get<Being*>(it->second)) {
				// Being is already loaded.
				return being;
			}
			// Load being from its coordinates.
			GlobalCoords coords = std::get<GlobalCoords>(it->second);
			Region& region = *_region; /// @todo Load region based on the region name in the coords (coords.region).
			for (Being& being : region.section(coords.section)->beings()) {
				if (being.id() == id) {
					return &being;
				}
			}
			throw std::logic_error{"Being lookup failed."};
		}
	}

	Object* Game::_object(ObjectId id) const
	{
		auto it = _objects.find(id);
		if (it == _objects.end()) {
			// Object no longer exists.
			return nullptr;
		} else {
			if (Object* object = std::get<Object*>(it->second)) {
				// Object is already loaded.
				return object;
			}
			// Load object from its coordinates.
			GlobalCoords coords = std::get<GlobalCoords>(it->second);
			Region& region = *_region; /// @todo Load region based on the region name in the coords (coords.region).
			for (Object& object : region.section(coords.section)->objects()) {
				if (object.id() == id) {
					return &object;
				}
			}
			throw std::logic_error{"Object lookup failed."};
		}
	}

	void Game::update_player_view()
	{
		/// @todo Do something nice when the player dies.
		if (Being* player_being = being(_player_being_id)) {
			// Update the player's world view.
			_player->update_world_view();
			// Update the world renderer's world view.
			_world_renderer->update_view(_player->world_view(), _player->poll_perceived_effects());
		}
	}
}
