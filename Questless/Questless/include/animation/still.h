//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.h"
#include "sdl/resources.h"
#include "units/game_space.h"

namespace ql
{
	//! A still image.
	class still : public animation
	{
	public:
		//! @param texture_handle A handle to the texture to use for this still.
		//! @param origin The origin of this still's texture, relative to its center.
		still(sdl::texture_handle texture_handle, units::texture_space::vector origin)
			: _texture_handle{texture_handle}, _origin{origin}
		{}

		void draw(units::screen_space::point position) const final;

		void draw(units::game_space::point position, camera const& camera, units::colors::color_factor color_factor = units::colors::white_factor()) const final;
	private:
		sdl::texture_handle _texture_handle;

		units::texture_space::vector _origin;

		void animation_subupdate() final {}
	};
}
