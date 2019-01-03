//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/resources.hpp"

#include "animation.hpp"
#include "world/world.hpp"

namespace ql {
	//! A still image.
	class still : public animation {
	public:
		//! @param texture_handle A handle to the texture to use for this still.
		//! @param origin The origin of this still's texture, relative to its center.
		still(sdl::texture_handle texture_handle, sdl::spaces::window::vector origin)
			: _texture_handle{texture_handle}, _origin{origin}
		{}

		void draw(sdl::spaces::window::point position) const final;

		void draw(world::point position, camera const& camera, sdl::spaces::colors::color color_factor = sdl::spaces::colors::white()) const final;
	private:
		sdl::texture_handle _texture_handle;

		sdl::spaces::window::vector _origin;

		void animation_subupdate() final {}
	};
}
