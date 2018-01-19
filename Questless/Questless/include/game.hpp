//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <chrono>
#include <deque>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "agents/player.hpp"
#include "animation/camera.hpp"
#include "animation/world_renderer.hpp"
#include "effects/effect.hpp"
#include "entities/beings/being.hpp"
#include "items/item.hpp"
#include "sdl/font.hpp"
#include "sdl/input.hpp"
#include "sdl/renderer.hpp"
#include "sdl/sound.hpp"
#include "sdl/texture.hpp"
#include "sdl/window.hpp"
#include "ui/digraph_menu.hpp"
#include "ui/hud.hpp"
#include "ui/list_dialog.hpp"
#include "ui/player_action_dialog.hpp"
#include "units/frequency.hpp"
#include "utility/cache.hpp"
#include "world/region.hpp"

namespace ql
{
	//! Represents an instance of the game Questless.
	class game
	{
	public:
		using clock = std::chrono::steady_clock;

		static constexpr units::hertz frame_rate = 60.0_Hz;
		static constexpr units::game_space::seconds frame_duration = 1.0 / frame_rate;

		//! @todo Relocate these maps?
		static id<being> get_being_id(uptr<being> const& being);
		static id<object> get_object_id(uptr<object> const& object);
		static id<item> get_item_id(uptr<item> const& item);
		static id<light_source> get_light_source_id(uptr<light_source> const& light_source);
		template <typename T>
		static T* uptr_to_ptr(uptr<T> const& p) { return p.get(); }

		cache<id<being>, uptr<being>, being, get_being_id, uptr_to_ptr<being>> beings;
		cache<id<object>, uptr<object>, object, get_object_id, uptr_to_ptr<object>> objects;
		cache<id<item>, uptr<item>, item, get_item_id, uptr_to_ptr<item>> items;
		cache<id<light_source>, uptr<light_source>, light_source, get_light_source_id, uptr_to_ptr<light_source>> light_sources;

		friend game& the_game();

		//! Releases game resources and breaks down the environment.
		~game();

		//! Runs a new game of Questless.
		void run();

		//! Adds the given dialog to the dialogs stack.
		complete add_dialog(uptr<dialog> dialog);

		void query_player_choice(std::function<void(player_action_dialog::choice)> cont);

		ql::region& region() { return *_region; }
		ql::region const& region() const { return *_region; }

		world_renderer& world_renderer() { return *_world_renderer; }

		//! Adds an effect to the world, notifying beings within range of its occurrence.
		//! @param effect The effect to add.
		void add_effect(sptr<effect> const& effect);

		ql::hud& hud() { return *_hud; }
		ql::hud const& hud() const { return *_hud; }

		camera const& camera() const { return *_camera; }
	private:
		///////////////
		// Constants //
		///////////////

		static constexpr units::game_space::seconds _max_accrued_update_time = 1.0s;

		static constexpr units::game_space::seconds _splash_fade_out_duration = 2.0s;
		static constexpr units::game_space::seconds _splash_fade_in_duration = 2.0s;
		static constexpr units::game_space::seconds _splash_duration = _splash_fade_out_duration + _splash_fade_in_duration;

		// Debug

		std::size_t const _max_fps_buffer_size = 25;

		// Display

#if 1
		static constexpr int _dflt_window_width = 1024;
		static constexpr int _dflt_window_height = 768;
#else
		static constexpr int _dflt_window_width = 1920;
		static constexpr int _dflt_window_height = 1080;
#endif
		static constexpr units::colors::color _splash_clear_color = units::colors::black();
		static constexpr units::colors::color _menu_clear_color = units::colors::black();
		static constexpr units::colors::color _playing_clear_color = units::colors::black();

		// Splash screen

		static constexpr int _splash_flames_count = 20;
		static constexpr units::game_space::speed _splash_flames_vy{-2800.0};
		static constexpr int _splash_logo_jiggle = 3;

		//////////
		// Data //
		//////////

		uptr<ql::camera> _camera;

		// Debug

		std::deque<double> _fps_buffer;

		// Textures

		uptr<sdl::texture> _txt_test1;
		uptr<sdl::texture> _txt_test2;
		uptr<sdl::texture> _txt_test3;
		uptr<sdl::texture> _txt_test_even;

		uptr<sdl::texture> _txt_splash_logo;
		uptr<sdl::texture> _txt_splash_flame;
		
		uptr<sdl::texture> _txt_hex_highlight;
		uptr<sdl::texture> _txt_hex_circle;

		// Animations

		uptr<ql::world_renderer> _world_renderer;

		// Fonts

		uptr<sdl::font> _fnt_20pt;

		// Sounds

		uptr<sdl::sound> _sfx_splash;

		// Game state

		enum class state { splash, menu, playing };
		state _state;

		std::optional<ql::id<being>> _player_being_id;
		player* _player;

		bool _splash_sound_played;
		std::vector<units::window_space::point> _splash_flame_positions;

		uptr<ql::region> _region;

		clock::time_point _time_last_state_change;

		// UI

		digraph_menu _main_menu;

		uptr<player_action_dialog> _player_action_dialog;
		std::deque<uptr<dialog>> _dialogs;

		uptr<ql::hud> _hud;

		units::game_space::point _point_clicked_rounded{0.0, 0.0};

		/////////////
		// Methods //
		/////////////

		//! Creates a game object, initializes the environment, and loads game resources.
		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		void load_textures();

		void initialize_opengl();

		// Update methods

		enum class update_result { continue_game, game_over };

		update_result update(units::game_space::seconds& accrued_time, clock::time_point& last_update_time);
		update_result update_splash();
		update_result update_menu();
		update_result update_playing();

		void update_player_view();

		// Render methods

		void render();
		void render_splash();
		void render_menu();
		void render_playing();
	};

	//! The game instance.
	inline game& the_game()
	{
		static game instance{false};
		return instance;
	}
}
