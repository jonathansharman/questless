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

#include "agents/Player.h"
#include "animation/Camera.h"
#include "animation/WorldRenderer.h"
#include "effects/Effect.h"
#include "entities/beings/Being.h"
#include "items/Item.h"
#include "sdl/Font.h"
#include "sdl/Input.h"
#include "sdl/Renderer.h"
#include "sdl/Sound.h"
#include "sdl/Texture.h"
#include "sdl/Window.h"
#include "ui/DigraphMenu.h"
#include "ui/HUD.h"
#include "ui/ListDialog.h"
#include "ui/PlayerActionDialog.h"
#include "utility/Cache.h"
#include "world/Region.h"

namespace questless
{
	//! Represents an instance of the game Questless.
	class Game : public sdl::Renderable
	{
	public:
		using clock = std::chrono::steady_clock;
		using seconds = std::chrono::duration<double>;

		static constexpr units::Hertz frame_rate = 60.0_Hz;
		static constexpr units::GameSeconds frame_duration = 1.0 / frame_rate;

		//! @todo Relocate these maps?
		static Id<Being> get_being_id(uptr<Being> const& being);
		static Id<Object> get_object_id(uptr<Object> const& object);
		static Id<Item> get_item_id(uptr<Item> const& item);
		static Id<LightSource> get_light_source_id(uptr<LightSource> const& light_source);
		template <typename T>
		static T* uptr_to_ptr(uptr<T> const& p) { return p.get(); }

		Cache<Id<Being>, uptr<Being>, Being, get_being_id, uptr_to_ptr<Being>> beings;
		Cache<Id<Object>, uptr<Object>, Object, get_object_id, uptr_to_ptr<Object>> objects;
		Cache<Id<Item>, uptr<Item>, Item, get_item_id, uptr_to_ptr<Item>> items;
		Cache<Id<LightSource>, uptr<LightSource>, LightSource, get_light_source_id, uptr_to_ptr<LightSource>> light_sources;

		friend Game& game();

		//! Releases game resources and breaks down the environment.
		~Game();

		//! Runs a new game of Questless.
		void run();

		//! Adds the given dialog to the dialogs stack.
		Complete add_dialog(uptr<Dialog> dialog);

		void query_player_choice(std::function<void(PlayerActionDialog::Choice)> cont);

		Region& region() { return *_region; }
		Region const& region() const { return *_region; }

		//! Adds an effect to the world, notifying beings within range of its occurrence.
		//! @param effect The effect to add.
		void add_effect(sptr<Effect> const& effect);

		HUD& hud() { return *_hud; }
		HUD const& hud() const { return *_hud; }

		Camera const& camera() const { return *_camera; }
	private:
		///////////////
		// Constants //
		///////////////

		static constexpr units::GameSeconds _max_accrued_update_time = 1.0s;

		static constexpr units::GameSeconds _splash_fade_out_duration = 2.0s;
		static constexpr units::GameSeconds _splash_fade_in_duration = 2.0s;
		static constexpr units::GameSeconds _splash_duration = _splash_fade_out_duration + _splash_fade_in_duration;

		// Debug

		size_t const _max_fps_buffer_size = 25;

		// Display

#if 1
		static constexpr int _dflt_window_width = 1024;
		static constexpr int _dflt_window_height = 768;
#else
		static constexpr int _dflt_window_width = 1920;
		static constexpr int _dflt_window_height = 1080;
#endif
		static constexpr sdl::Color _splash_clear_color = sdl::Color::black();
		static constexpr sdl::Color _menu_clear_color = sdl::Color::black();
		static constexpr sdl::Color _playing_clear_color = sdl::Color::black();

		// Splash screen

		static constexpr int _splash_flames_count = 20;
		static constexpr units::Rate<double, double> _splash_flames_vy{-2800.0};
		static constexpr int _splash_logo_jiggle = 3;

		//////////
		// Data //
		//////////

		uptr<Camera> _camera;

		// Debug

		std::deque<double> _fps_buffer;

		// Textures

		uptr<sdl::Texture> _txt_test; //!< A placeholder texture used for testing.
		uptr<sdl::Texture> _txt_test2;
		uptr<sdl::Texture> _txt_test3;

		uptr<sdl::Texture> _txt_splash_logo;
		uptr<sdl::Texture> _txt_splash_flame;
		
		uptr<sdl::Texture> _txt_hex_highlight;
		uptr<sdl::Texture> _txt_hex_circle;

		// Animations

		std::unique_ptr<WorldRenderer> _world_renderer;

		// Fonts

		uptr<sdl::Font> _fnt_20pt;

		// Sounds

		uptr<sdl::Sound> _sfx_splash;

		// Game state

		enum class State { splash, menu, playing };
		State _state;

		std::optional<Id<Being>> _player_being_id;
		Player* _player;

		bool _game_over;

		bool _splash_sound_played;
		std::vector<units::ScreenPoint> _splash_flame_positions;

		std::unique_ptr<Region> _region;

		clock::time_point _time_last_state_change;

		// UI

		DigraphMenu _main_menu;

		std::unique_ptr<PlayerActionDialog> _player_action_dialog;
		std::deque<uptr<Dialog>> _dialogs;

		std::unique_ptr<HUD> _hud;

		units::GamePoint _point_clicked_rounded{0.0, 0.0};

		/////////////
		// Methods //
		/////////////

		//! Creates a game object, initializes the environment, and loads game resources.
		//! @param fullscreen Whether to run the game in fullscreen mode.
		Game(bool fullscreen);

		void refresh() override { load_textures(); }

		void load_textures();

		void game_loop();

		// Update methods

		void update_splash();
		void update_menu();
		void update_playing();

		void update_player_view();

		// Render methods

		void render_splash();
		void render_menu();
		void render_playing();
	};

	//! The game instance.
	inline Game& game()
	{
		static Game instance{false};
		return instance;
	}
}
