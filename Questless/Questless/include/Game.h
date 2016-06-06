/**
* @file    Questless.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Questless Game class.
*/

#ifndef QUESTLESS_H
#define QUESTLESS_H

#include <string>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <type_traits>
#include <stdexcept>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "ui/DigraphMenuController.h"
#include "ui/PlayerActionDialog.h"
#include "ui/ListDialog.h"
#include "utility/Frequency.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "sdl-wrappers/Window.h"
#include "sdl-wrappers/Renderer.h"
#include "sdl-wrappers/Texture.h"
#include "sdl-wrappers/Font.h"
#include "sdl-wrappers/Sound.h"
#include "sdl-wrappers/Input.h"
#include "animation/AnimationCollection.h"
#include "animation/Camera.h"
#include "animation/particles/Particle.h"
#include "animation/WorldRenderer.h"
#include "world/Region.h"
#include "entities/beings/Being.h"
#include "entities/beings/Agent.h"
#include "items/Item.h"

namespace questless
{
	class Game : public Renderable
	{
	public:
		/// Creates a game object, initializes the environment, and loads game resources.
		/// @param fullscreen Whether to run the game in fullscreen mode.
		Game(bool fullscreen);

		/// Releases game resources and breaks down the environment.
		~Game();

		/// Runs a new game of Questless.
		void run();

		// Dialogs

		void message(std::string title, std::string prompt, std::function<void()> cont);

		void query_player_choice(std::function<void(PlayerActionDialog::Choice)> cont);

		template <typename ItemType>
		void query_list
			( sdl::Point origin
			, std::string title
			, std::vector<ItemType> options
			, std::function<std::string(const ItemType&)> item_to_name
			, std::function<void(optional<ItemType>)> cont
			)
		{
			auto dialog = make_unique<ListDialog<ItemType>>(origin, std::move(title), std::move(options), std::move(item_to_name), std::move(cont));
			_dialogs.push_back(std::move(dialog));
		}

		void query_count(std::string title, std::string prompt, int default, optional<int> min, optional<int> max, std::function<void(optional<int>)> cont);
		void query_count(std::string title, std::string prompt, int default, std::function<bool(int)> predicate, std::function<void(optional<int>)> cont);

		void query_magnitude(std::string title, std::string prompt, double default, std::function<bool(double)> predicate, std::function<void(optional<double>)> cont);

		void query_tile(std::string title, std::string prompt, std::function<bool(HexCoords)> predicate, std::function<void(optional<HexCoords>)> cont);

		void query_being(std::string title, std::string prompt, std::function<bool(Being&)> predicate, std::function<void(optional<Being*>)> cont);

		void query_item(std::string title, std::string prompt, const Being& source, std::function<bool(Being&)> predicate, std::function<void(optional<Item*>)> cont);

		// Accessors

		Input& input() { return _input; }
		const Input& input() const { return _input; }

		Region& region() { return *_region; }
		const Region& region() const { return *_region; }

		HUDController& hud() { return *_hud; }
		const HUDController& hud() const { return *_hud; }
	private:
		///////////////
		// Constants //
		///////////////

		const double_seconds splash_fade_out_duration{2.0};
		const double_seconds splash_fade_in_duration{2.0};
		const double_seconds splash_duration = splash_fade_out_duration + splash_fade_in_duration;

		// Debug

		const size_t max_fps_buffer_size = 25;

		// Display

#if 1
		const int dflt_window_width = 1024;
		const int dflt_window_height = 768;
#else
		const int dflt_window_width = 1920;
		const int dflt_window_height = 1080;
#endif
		const sdl::Color splash_clear_color = sdl::Color::black();
		const sdl::Color menu_clear_color = sdl::Color::blue();
		const sdl::Color playing_clear_color = sdl::Color::blue();

		// Splash screen

		const unsigned splash_flames_count = 20;
		const Hertz splash_flames_speed{2800.0};
		const int splash_logo_jiggle = 3;

		//////////
		// Data //
		//////////

		sdl::Window::ptr _window;

		std::unique_ptr<Camera> _camera;

		// Debug

		std::deque<double> _fps_buffer;

		// sdl::Textures

		sdl::Texture::ptr _txt_test; ///< A placeholder texture used for testing.
		sdl::Texture::ptr _txt_test2;
		sdl::Texture::ptr _txt_test3;

		sdl::Texture::ptr _txt_splash_logo;
		sdl::Texture::ptr _txt_splash_flame;
		
		sdl::Texture::ptr _txt_hex_highlight;
		sdl::Texture::ptr _txt_hex_circle;

		// Animations

		std::unique_ptr<AnimationCollection> _ani_test;

		std::unique_ptr<WorldRenderer> _world_renderer;

		// Fonts

		sdl::Font::ptr _fnt_20pt;

		// Sounds

		sdl::Sound::ptr _sfx_splash;

		// Game state

		enum class State { splash, menu, playing };
		State _state;

		Being* _player_being;

		std::unique_ptr<WorldView> _world_view;

		double _time;

		sdl::Input _input;

		bool _game_over;

		bool _splash_sound_played;
		std::vector<sdl::Point> _splash_flame_positions;

		std::unique_ptr<Region> _region;

		clock::time_point _time_last_state_change;

		// UI

		DigraphMenuController _mnu_main;

		std::deque<std::unique_ptr<Dialog>> _dialogs;

		std::unique_ptr<HUDController> _hud;

		// Miscellaneous

		std::vector<std::unique_ptr<Particle>> _particles;

		/////////////
		// Methods //
		/////////////

		void refresh() override { load_textures(); }

		void load_textures();

		void game_loop();

		// Update methods

		void update_splash();
		void update_menu();
		void update_playing();

		// Render methods

		void render_splash();
		void render_menu();
		void render_playing();
	};
}

#endif