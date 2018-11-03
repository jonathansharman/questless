//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "utility/reference.hpp"

#include "sdl/sound.hpp"
#include "sdl/texture.hpp"
#include "units/window_space.hpp"

namespace ql::scene {
	//! The splash screen scene.
	class splash : scene {
	public:
		splash();
	private:
		bool _sound_played = false;
		std::vector<units::window_space::point> _flame_positions{};

		sdl::texture _txt_splash_logo;
		sdl::texture _txt_splash_flame;

		sdl::sound _sfx_splash;

		update_result subupdate() final;

		void subdraw() final;
	};
}
