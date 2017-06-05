//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Animation.h"
#include "sdl/resources.h"
#include "units/GameSpace.h"

namespace questless
{
	//! A still image.
	class Still : public Animation
	{
	public:
		//! @param texture_handle A handle to the texture to use for this still.
		//! @param origin The origin of this still's texture, relative to its center.
		Still(sdl::TextureHandle texture_handle, units::TextureSpace::Point origin)
			: _texture_handle{texture_handle}, _origin{origin}
		{}

		void draw(units::ScreenSpace::Point position) const final;

		void draw(units::GameSpace::Point position, Camera const& camera, units::colors::ColorFactor color_factor = units::colors::white_factor()) const final;
	private:
		sdl::TextureHandle _texture_handle;

		units::TextureSpace::Point _origin;

		void animation_subupdate() final {}
	};
}
