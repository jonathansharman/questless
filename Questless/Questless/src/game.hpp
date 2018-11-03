//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/cache.hpp"
#include "utility/reference.hpp"

namespace sdl {
	class font;
	class input;
	class renderer;
	class sound;
	class texture;
	class window;
}

namespace ql {
	class being;
	class item;
	class light_source;
	class object;
	class player;
	namespace scene { class scene; }

	//! Represents an instance of the game Questless.
	class game {
	public:
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

		~game();

		//! Runs a new game of Questless.
		void run();
	private:
		enum class state { being_editor };
		uptr<scene::scene> _scene = nullptr;

		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		void load_textures();

		void initialize_opengl();
	};

	//! The game instance.
	inline game& the_game() {
		static game instance{false};
		return instance;
	}
}
