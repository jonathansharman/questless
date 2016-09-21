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
#include "ui/MessageDialog.h"
#include "ui/CountDialog.h"
#include "ui/MagnitudeDialog.h"
#include "ui/TileDialog.h"
#include "animation/EntityAnimator.h"
#include "animation/particles/WhiteMagic.h"
#include "animation/particles/BlackMagic.h"
#include "animation/particles/GreenMagic.h"
#include "animation/particles/RedMagic.h"
#include "animation/particles/BlueMagic.h"
#include "animation/particles/YellowMagic.h"

/// @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/Agent.h"
#include "agents/Player.h"
#include "entities/beings/Human.h"
#include "entities/beings/goblinoids/Goblin.h"
#include "items/Scroll.h"
#include "spells/LightningBoltSpell.h"
#include "spells/HealSpell.h"
#include "spells/TeleportSpell.h"
#include "items/weapons/Quarterstaff.h"

using std::move;
using std::unique_ptr;
using std::make_unique;
using std::make_shared;
using std::dynamic_pointer_cast;
using std::function;

using namespace std::chrono;

using namespace sdl;

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
			throw std::runtime_error("Failed to initialize SDL.");
		}
		if (fullscreen) {
			_window = Window::make("Questless", "resources/textures/icon.png", true);
		} else {
			_window = Window::make("Questless", "resources/textures/icon.png", false, dflt_window_width, dflt_window_height, true, true, true);
		}

		renderer(make_unique<Renderer>(*_window, _window->width(), _window->height()));

		_camera = make_unique<Camera>(*_window, PointF{0, 0});

		/// @todo Make render quality a game setting.
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

		if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
			throw std::runtime_error("Failed to initialize IMG.");
		}

		// Initialize and load fonts.

		if (TTF_Init()) {
			throw std::runtime_error("Failed to initialize TTF. SDL Error: " + string{SDL_GetError()});
		}

		_fnt_20pt = Font::make("resources/fonts/firamono.ttf", 20, SDL_BLENDMODE_BLEND);

		// Initialize sound.

	    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)) {
			throw std::runtime_error("Failed to initialize Mix.");
	    }

		// Initialize and load menu resources.

		_hud = make_unique<HUDController>(*this, *_window);

		// Load textures and graphics.

		load_textures();

		Handle<Texture> test_ss_handle = texture_manager().add([] { return Texture::make("resources/textures/test_animation.png", renderer(), SDL_BLENDMODE_BLEND); });
		_ani_test = make_unique<AnimationCollection>(test_ss_handle, 3, 1);

		// Load sounds.

		_sfx_splash = Sound::make("resources/sounds/splash.wav");

		// Initialize game state.

		for (unsigned i = 0; i < splash_flames_count; ++i) {
			_splash_flame_positions.emplace_back(uniform(0, _window->width() - 1), (i + 1) * _window->height() / splash_flames_count);
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
		_txt_test = Texture::make("resources/textures/test.png", renderer(), SDL_BLENDMODE_BLEND);
		_txt_test2 = Texture::make("resources/textures/test2.png", renderer(), SDL_BLENDMODE_BLEND);
		_txt_test3 = Texture::make("resources/textures/test3.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_splash_logo = Texture::make("resources/textures/splash/logo.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_splash_flame = Texture::make("resources/textures/splash/flame.png", renderer(), SDL_BLENDMODE_BLEND);

		_txt_hex_highlight = Texture::make("resources/textures/terrain/tile.png", renderer(), SDL_BLENDMODE_BLEND);
		_txt_hex_highlight->alpha(128);

		_txt_hex_circle = Texture::make("resources/textures/ui/hex_circle.png", renderer(), SDL_BLENDMODE_BLEND);
	}

	// Dialogs

	void Game::message(string title, string prompt, function<void()> cont)
	{
		auto dialog = make_unique<MessageDialog>(move(title), move(prompt), move(cont));
		_dialogs.push_back(move(dialog));
	}

	void Game::query_player_choice(function<void(PlayerActionDialog::Choice)> cont)
	{
		auto dialog = make_unique<PlayerActionDialog>(*_hud, move(cont));
		_dialogs.push_back(move(dialog));
	}

	void Game::query_count(string title, string prompt, int default, optional<int> min, optional<int> max, function<void(optional<int>)> cont)
	{
		auto dialog = make_unique<CountDialog>(move(title), move(prompt), default, min, max, [](int) { return true; }, move(cont));
		_dialogs.push_back(move(dialog));
	}
	void Game::query_count(string title, string prompt, int default, function<bool(int)> predicate, function<void(optional<int>)> cont)
	{
		auto dialog = make_unique<CountDialog>(move(title), move(prompt), default, nullopt, nullopt, move(predicate), move(cont));
		_dialogs.push_back(move(dialog));
	}

	void Game::query_magnitude(string title, string prompt, double default, function<bool(double)> predicate, function<void(optional<double>)> cont)
	{
		auto dialog = make_unique<MagnitudeDialog>(move(title), move(prompt), default, nullopt, nullopt, move(predicate), move(cont));
		_dialogs.push_back(move(dialog));
	}

	void Game::query_tile(string title, string prompt, function<bool(RegionTileCoords)> predicate, function<void(optional<RegionTileCoords>)> cont)
	{
		auto dialog = make_unique<TileDialog>(move(title), move(prompt), *_camera, move(predicate), move(cont));
		_dialogs.push_back(move(dialog));
	}

	void Game::query_being(string /*title*/, string /*prompt*/, function<bool(Being&)> /*predicate*/, function<void(optional<Being*>)> cont)
	{
		/// @todo This.
		cont(nullopt);
	}

	void Game::query_item(string /*title*/, string /*prompt*/, Being& /*source*/, function<bool(Being&)> /*predicate*/, function<void(optional<Item*>)> cont)
	{
		/// @todo This.
		cont(nullopt);
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
		double_seconds accrued_time = double_seconds::zero();
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
			if (accrued_time > accrued_update_time_max) {
				accrued_time = accrued_update_time_max;
			}

			// Update FPS.

			auto frame_ms = duration_cast<milliseconds>(frame_time).count();
			if (frame_ms != 0) {
				_fps_buffer.push_back(1000.0 / frame_ms);
			}
			if (_fps_buffer.size() > max_fps_buffer_size) {
				_fps_buffer.pop_front();
			}

			// Render

			switch (_state) {
				case State::splash:
					renderer().clear(splash_clear_color);
					render_splash();
					break;
				case State::menu:
					renderer().clear(menu_clear_color);
					render_menu();
					break;
				case State::playing:
					renderer().clear(playing_clear_color);
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
			txt_fps.draw(Point(_window->width() - 1, _window->height() - 1), HAlign::right, VAlign::bottom);

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

		for (Point& position : _splash_flame_positions) {
			position.y -= lround(splash_flames_speed * frame_duration);
			if (position.y < 0) {
				position.y += _window->height() + _txt_splash_flame->height();
				position.x = uniform(0, _window->width() - 1);
			}
		}

		if (_input.any_presses() || clock::now() - _time_last_state_change >= splash_duration) {
			// Clear/unload/stop splash stuff

			_splash_flame_positions.clear();
			_txt_splash_logo = nullptr;
			_txt_splash_flame = nullptr;
			_sfx_splash->stop();

			// Load menu stuff

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

			vector<Animation::Frame> frames;
			frames.emplace_back(double_seconds{1.0}, Point{0, 0}, Point{0, 0});
			frames.emplace_back(double_seconds{1.0}, Point{1, 0}, Point{0, 0});
			frames.emplace_back(double_seconds{1.0}, Point{2, 0}, Point{0, 0});
			_ani_test->add("", Animation::make(frames, true));
			_ani_test->start("");

			_time_last_state_change = clock::now();
			_state = State::menu;
		}
	}
	
	void Game::render_splash()
	{
		if (clock::now() - _time_last_state_change < splash_fade_in_duration) {
			auto ms_fading_in = duration_cast<double_seconds>(clock::now() - _time_last_state_change).count();
			uint8_t intensity = percentage_to_byte(static_cast<double>(ms_fading_in / splash_fade_in_duration.count()));
			_txt_splash_logo->color(Color{intensity, intensity, intensity});
			_txt_splash_flame->color(Color{intensity, intensity, intensity});
		} else {
			auto ms_fading_out = duration_cast<double_seconds>(clock::now() - _time_last_state_change - splash_fade_in_duration).count();
			uint8_t intensity = percentage_to_byte(1 - static_cast<double>(ms_fading_out / splash_fade_out_duration.count()));
			_txt_splash_logo->color(Color{intensity, intensity, intensity});
			_txt_splash_flame->color(Color{intensity, intensity, intensity});
		}
		_txt_splash_logo->draw(_window->center() + Vector{uniform(-splash_logo_jiggle, splash_logo_jiggle), uniform(-splash_logo_jiggle, splash_logo_jiggle)}, HAlign::center, VAlign::middle);
		for (Point position : _splash_flame_positions) {
			_txt_splash_flame->draw(position, HAlign::center, VAlign::bottom);
		}
	}

	void Game::update_menu()
	{
		_mnu_main.update(_input);
		for (const auto& option : _mnu_main.poll_selections()) {
			if (option.first == "Questless") {
				if (option.second == "Continue" || option.second == "Begin Anew") {
					_region = make_unique<Region>(*this, "Region1");
					//_region = make_unique<Region>(*this, "Slot1", "Region1");

					{ // Spawn the player's being.
						Being::ptr player_being = make_unique<Human>(*this, Agent::make<Player>, BeingId::next());
						_player_id = player_being->id();
						player_being->give_item(make_unique<Scroll>(make_unique<LightningBoltSpell>()));
						player_being->give_item(make_unique<Scroll>(make_unique<HealSpell>()));
						player_being->give_item(make_unique<Scroll>(make_unique<TeleportSpell>()));
						player_being->give_item((make_unique<Quarterstaff>()));
						_region->spawn_player(move(player_being));
					}
					// Pass the player's being ID to the HUD.
					_hud->player_id(_player_id);
					// Set the initial world view, world renderer, and camera position relative to the player's being.
					_world_view = make_unique<WorldView>(*being(_player_id), true);
					_world_renderer = make_unique<WorldRenderer>(*_world_view);
					_camera->position(PointF{Layout::dflt().to_world(being(_player_id)->coords().hex)});

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
		const int width = 20;
		const int count = 18;
		for (int i = 0; i < count; ++i) {
			renderer().draw_rect(Rect(_window->center().x - width * i, _window->center().y - width * i, 2 * width * i, 2 * width * i), Color::white(), false);
		}

		_world_renderer->draw_terrain(*_camera);

		_ani_test->draw(Point(0, 0));

		static Point pt_clicked_rounded; /// @todo Ewww...
		if (_input.pressed(MouseButton::right)) {
			pt_clicked_rounded = _camera->pt_hovered_rounded();
		}
		_camera->draw(*_txt_hex_highlight, _camera->pt_hovered_rounded());
		_camera->draw(*_txt_hex_circle, pt_clicked_rounded);

		_world_renderer->draw_objects(*this, *_camera);
		_world_renderer->draw_beings(*this, *_camera);

		for (auto& particle : _particles) {
			particle->draw(*_camera);
		}

		// Draw HUD.
		_hud->draw();

		// Draw dialogs.
		for (const auto& dialog : _dialogs) {
			dialog->draw(*_window);
		}

		ostringstream ss_cam_coords;
		ss_cam_coords.setf(std::ios::fixed);
		ss_cam_coords.precision(0);
		ss_cam_coords << "(" << _camera->position().x << ", " << _camera->position().y << ")";
		Texture txt_cam_coords = _fnt_20pt->render(ss_cam_coords.str(), renderer(), Color::white());
		txt_cam_coords.draw(Point(0, 0));

		ostringstream ss_cam_hex_coords;
		ss_cam_hex_coords.setf(std::ios::fixed);
		ss_cam_hex_coords.precision(0);
		HexCoords cam_hex_coords = Layout::dflt().to_hex_coords(_camera->position());
		ss_cam_hex_coords << "(" << cam_hex_coords.q << ", " << cam_hex_coords.r << ")";
		Texture txt_cam_hex_coords = _fnt_20pt->render(ss_cam_hex_coords.str(), renderer(), Color::white());
		txt_cam_hex_coords.draw(Point(0, 25));

		ostringstream ss_turn;
		ss_turn << "Time: " << _time;
		Texture txt_turn = _fnt_20pt->render(ss_turn.str(), renderer(), Color::white());
		txt_turn.draw(Point(0, 50));
	}

	void Game::update_playing()
	{
		// Update camera.
		_camera->update(_input);

		// Update HUD.
		_hud->update(_input);

		// Update region animations.
		//_region->update_animations();

		// Update dialogs.
		if (!_dialogs.empty()) {
			_dialogs.front()->update(_input);
			if (_dialogs.front()->closed()) {
				_dialogs.pop_front();

				// End of player's turn. Update world view.
				if (_dialogs.empty()) {
					/// @todo Do something nice when the player dies.
					if (Being* player_being = being(_player_id)) {
						// Reset the world view.
						_world_view = make_unique<WorldView>(*player_being, true);
						// Reset the world renderer.
						_world_renderer->reset_view(*_world_view);
					}
				}
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish acting yet.
			while (Being* next_ready_being = _region->next_ready_being()) {
				next_ready_being->agent().act();
				if (!_dialogs.empty()) {
					// Awaiting player input to complete current action. Stop taking turns, and start at the next agent once this action is complete.

					// Reset the world view.
					_world_view = make_unique<WorldView>(*being(_player_id), true);
					// Reset the world renderer.
					_world_renderer->reset_view(*_world_view);

					/// @todo The world render should probably be updated more frequently than before and after the player's turn.

					break;
				}
			}
			if (_dialogs.empty()) {
				// Advance the time.
				_time += 1.0;
				// Update the region.
				_region->update();
			}
		}

		// Update world renderer.
		_world_renderer->update();

		if (_input.down(SDLK_t)) {
			_particles.emplace_back(make_unique<WhiteMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<WhiteMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<WhiteMagic>(_camera->pt_hovered()));
		}
		if (_input.down(SDLK_y)) {
			_particles.emplace_back(make_unique<BlackMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<BlackMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<BlackMagic>(_camera->pt_hovered()));
		}
		if (_input.down(SDLK_u)) {
			_particles.emplace_back(make_unique<GreenMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<GreenMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<GreenMagic>(_camera->pt_hovered()));
		}
		if (_input.down(SDLK_i)) {
			_particles.emplace_back(make_unique<RedMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<RedMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<RedMagic>(_camera->pt_hovered()));
		}
		if (_input.down(SDLK_o)) {
			_particles.emplace_back(make_unique<BlueMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<BlueMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<BlueMagic>(_camera->pt_hovered()));
		}
		if (_input.down(SDLK_p)) {
			_particles.emplace_back(make_unique<YellowMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<YellowMagic>(_camera->pt_hovered()));
			_particles.emplace_back(make_unique<YellowMagic>(_camera->pt_hovered()));
		}

		for (size_t i = 0; i < _particles.size(); ++i) {
			_particles[i]->update();
			if (_particles[i]->dead()) {
				_particles.erase(_particles.begin() + i);
				--i;
			}
		}

		_ani_test->update();

		// Pan camera.
		if (_input.down(MouseButton::middle)) {
			VectorF pan = VectorF{_input.last_mouse_position() - _input.mouse_position()} / _camera->zoom();
			pan.rotate(_camera->angle());
			_camera->pan(pan);
		}
		const double pan_amount = 10.0;
		if (_input.down(SDLK_KP_8)) {
			_camera->pan(VectorF{0.0, -pan_amount} / _camera->zoom());
		}
		if (_input.down(SDLK_KP_4)) {
			_camera->pan(VectorF{-pan_amount, 0.0} / _camera->zoom());
		}
		if (_input.down(SDLK_KP_2)) {
			_camera->pan(VectorF{0.0, pan_amount} / _camera->zoom());
		}
		if (_input.down(SDLK_KP_6)) {
			_camera->pan(VectorF{pan_amount, 0.0} / _camera->zoom());
		}

		// Scale camera.
		_camera->zoom_factor(1 + _input.scroll() / 10.0f);

		// Rotate camera.
		double angle = _camera->angle();
		if (_input.down(SDLK_KP_9)) {
			_camera->rotate((-90.0 - angle) / 40.0);
		} else if (_input.down(SDLK_KP_7)) {
			_camera->rotate((90.0 - angle) / 40.0);
		} else {
			//_camera->angle(angle / 1.1);
		}

		// Camera follow.
		static bool follow = true;
		if (_input.presses(SDLK_BACKSPACE) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			// Move 1/x of the way back to the default camera values.
			double x = 5.0;

			VectorF to_player = PointF{Layout::dflt().to_world(being(_player_id)->coords().hex)} -_camera->position();
			_camera->position(_camera->position() + to_player / x);

			double to_zoom_1 = 1.0 - _camera->zoom();
			_camera->zoom(_camera->zoom() + to_zoom_1 / x);
			if (abs(_camera->zoom() - 1.0) < 0.001) {
				_camera->zoom(1.0);
			}

			_camera->angle(_camera->angle() * (1 - 1.0 / x));
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
				return being;
			}
			// Load being from its coordinates.
			GlobalCoords coords = std::get<GlobalCoords>(it->second);
			Region& region = *_region; /// @todo Load region based on the region name in the coords (coords.region).
			RegionSectionCoords section = coords.section;
			for (Being& being : region.section(section).beings()) {
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
				return object;
			}
			// Load object from its coordinates.
			GlobalCoords coords = std::get<GlobalCoords>(it->second);
			Region& region = *_region; /// @todo Load region based on the region name in the coords (coords.region).
			RegionSectionCoords section = coords.section;
			for (Object& object : region.section(section).objects()) {
				if (object.id() == id) {
					return &object;
				}
			}
			throw std::logic_error{"Object lookup failed."};
		}
	}
}
