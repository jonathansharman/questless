//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/input_manager.hpp"
#include "utility/cache.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	struct being;
	struct item;
	struct light_source;
	struct object;
	namespace scene {
		class scene;
	}

	//! Represents an instance of the game Questless.
	struct game {
		~game();

		//! @todo Relocate these maps?
		static id<being> get_being_id(being const& being);
		static id<object> get_object_id(object const& object);
		static id<item> get_item_id(item const& item);
		static id<light_source> get_light_source_id(light_source const& light_source);

		template <typename T>
		static T* to_ptr(T const& p) {
			return &p;
		}
		cache<id<being>, being, being, get_being_id, to_ptr<being>> beings;
		cache<id<object>, object, object, get_object_id, to_ptr<object>> objects;
		cache<id<item>, item, item, get_item_id, to_ptr<item>> items;
		cache<id<light_source>, light_source, light_source, get_light_source_id, to_ptr<light_source>> light_sources;

		friend game& the_game();

		//! Runs a new game of Questless.
		void run();

	private:
		enum class state { being_editor }; //! @todo Should not be needed. Replace with scene.

		sf::RenderWindow _window;

		input_manager _im{_window};

		uptr<scene::scene> _scene;

		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		void load_textures();
	};

	//! The game instance.
	inline game& the_game() {
		static game instance{false};
		return instance;
	}
}
