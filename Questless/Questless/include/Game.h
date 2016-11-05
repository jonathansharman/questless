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
#include <tuple>
#include <unordered_map>
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
#include "animation/WorldRenderer.h"
#include "world/Region.h"
#include "entities/beings/Being.h"
#include "agents/Player.h"
#include "items/Item.h"
#include "effects/Effect.h"

namespace questless
{
	class Game : public sdl::Renderable
	{
	public:
		/// Creates a game object, initializes the environment, and loads game resources.
		/// @param fullscreen Whether to run the game in fullscreen mode.
		Game(bool fullscreen);

		/// Releases game resources and breaks down the environment.
		~Game();

		/// Runs a new game of Questless.
		void run();

		// Dialog Methods

		void query_player_choice(std::function<void(PlayerActionDialog::Choice)> cont);

		Action::Complete message(std::string title, std::string prompt, std::function<Action::Complete()> cont);

		template <typename ItemType>
		Action::Complete query_list
			( sdl::Point origin
			, std::string title
			, std::vector<ItemType> options
			, std::function<std::string(const ItemType&)> item_to_name
			, std::function<Action::Complete(boost::optional<ItemType>)> cont
			)
		{
			auto dialog = std::make_unique<ListDialog<ItemType>>(origin, std::move(title), std::move(options), std::move(item_to_name), std::move(cont));
			_dialogs.push_back(std::move(dialog));
			return Action::Complete{};
		}

		Action::Complete query_count(std::string title, std::string prompt, int default, boost::optional<int> min, boost::optional<int> max, std::function<Action::Complete(boost::optional<int>)> cont);
		Action::Complete query_count(std::string title, std::string prompt, int default, std::function<bool(int)> predicate, std::function<Action::Complete(boost::optional<int>)> cont);

		Action::Complete query_magnitude(std::string title, std::string prompt, double default, std::function<bool(double)> predicate, std::function<Action::Complete(boost::optional<double>)> cont);

		Action::Complete query_tile(std::string title, std::string prompt, std::function<bool(RegionTileCoords)> predicate, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont);

		Action::Complete query_being(std::string title, std::string prompt, std::function<bool(Being&)> predicate, std::function<Action::Complete(boost::optional<Being*>)> cont);

		Action::Complete query_item(std::string title, std::string prompt, Being& source, std::function<bool(Being&)> predicate, std::function<Action::Complete(boost::optional<Item*>)> cont);

		// Other Methods

		sdl::Input& input() { return _input; }
		const sdl::Input& input() const { return _input; }

		Region& region() { return *_region; }
		const Region& region() const { return *_region; }

		/// Adds an effect to the world, notifying beings within range of its occurrence.
		/// @param effect The effect to add.
		void add_effect(const Effect::ptr& effect);

		/// Adds the given being to the graveyard.
		void add_to_graveyard(Being::ptr being) { _graveyard.push_back(std::move(being)); }

		/// Retrieves a pointer to the specified being from the global being directory, or nullptr if it doeesn't exist.
		/// If the being exists but isn't in memory, it will be loaded.
		/// @param id The ID of the being to retrieve.
		/// @return The being with the given ID, or nullptr if nonexistent.
		Being* being(BeingId id) { return _being(id); }
		/// Retrieves a pointer to the specified being from the global being directory, or nullptr if it doeesn't exist.
		/// If the being exists but isn't in memory, it will be loaded.
		/// @param id The ID of the being to retrieve.
		/// @return The being with the given ID, or nullptr if nonexistent.
		const Being* being(BeingId id) const { return _being(id); }
		/// Adds the given being and its global coordinates to the global being directory.
		/// @param being The being to add.
		/// @param coords The being's global coordinates.
		void add_being(Being* being, GlobalCoords coords) { _beings[being->id()] = std::make_tuple(coords, being); }
		/// Updates the global coordinates in the global being directory of the being with the given ID.
		/// @param id The ID of the being to update.
		/// @param coords The being's new global coordinates.
		void update_being_coords(BeingId id, GlobalCoords coords) { std::get<GlobalCoords>(_beings[id]) = coords; }
		/// Removes the specified being ID from the global being directory.
		/// @param id The ID of the being to delete.
		void remove_being_id(BeingId id) { _beings.erase(id); }

		/// Retrieves a pointer to the specified object from the global object directory, or nullptr if it doeesn't exist.
		/// If the object exists but isn't in memory, it will be loaded.
		/// @param id The ID of the object to retrieve.
		/// @return The object with the given ID, or nullptr if nonexistent.
		Object* object(ObjectId id) { return _object(id); }
		/// Retrieves a pointer to the specified object from the global object directory, or nullptr if it doeesn't exist.
		/// If the object exists but isn't in memory, it will be loaded.
		/// @param id The ID of the object to retrieve.
		/// @return The object with the given ID, or nullptr if nonexistent.
		const Object* object(ObjectId id) const { return _object(id); }
		/// Adds the given object and its global coordinates to the global object directory.
		/// @param object The object to add.
		/// @param coords The object's global coordinates.
		void add_object(Object* object, GlobalCoords coords) { _objects[object->id()] = std::make_tuple(coords, object); }
		/// Updates the global coordinates in the global object directory of the object with the given ID.
		/// @param id The ID of the object to update.
		/// @param coords The object's new global coordinates.
		void update_object_coords(ObjectId id, GlobalCoords coords) { std::get<GlobalCoords>(_objects[id]) = coords; }
		/// Removes the specified being ID from the global object directory.
		/// @param id The ID of the object to delete.
		void remove_object_id(ObjectId id) { _objects.erase(id); }

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
		const Hertz splash_flames_speed = 2800.0_Hz;
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

		std::unordered_map<BeingId, std::tuple<GlobalCoords, Being*>> _beings;
		std::unordered_map<ObjectId, std::tuple<GlobalCoords, Object*>> _objects;

		BeingId _player_being_id;
		Player* _player;

		double _time;

		sdl::Input _input;

		bool _game_over;

		bool _splash_sound_played;
		std::vector<sdl::Point> _splash_flame_positions;

		std::unique_ptr<Region> _region;

		std::vector<Being::ptr> _graveyard;

		clock::time_point _time_last_state_change;

		// UI

		DigraphMenuController _mnu_main;

		std::deque<std::unique_ptr<Dialog>> _dialogs;

		std::unique_ptr<HUDController> _hud;

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

		void update_player_view();

		// Render methods

		void render_splash();
		void render_menu();
		void render_playing();

		// Accessor helpers

		Being* _being(BeingId id) const;
		Object* _object(ObjectId id) const;
	};
}

#endif
