//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "sdl/resources.hpp"
#include "units/game_space.hpp"

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

		void draw(units::window_space::point position) const final;

		void draw(units::game_space::point position, camera const& camera, units::colors::color_vector color_vector = units::colors::white_vector()) const final;
	private:
		sdl::texture_handle _texture_handle;

		units::texture_space::vector _origin;

		void animation_subupdate() final {}
	};
}
