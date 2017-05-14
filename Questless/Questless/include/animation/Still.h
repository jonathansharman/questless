//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Animation.h"
#include "sdl/resources.h"
#include "units/GamePoint.h"
#include "units/GameVector.h"

namespace questless
{
	//! A still image.
	class Still : public Animation
	{
	public:
		//! @param texture_handle A handle to the texture to use for this still.
		//! @param origin The origin of this still's texture, relative to its center.
		Still(sdl::TextureHandle texture_handle, units::TexturePoint origin)
			: _texture_handle{texture_handle}, _origin{origin}
		{}

		void draw(units::ScreenPoint position) const final;

		void draw(units::GamePoint position, Camera const& camera, sdl::Color color = sdl::Color::white()) const final;
	private:
		sdl::TextureHandle _texture_handle;

		units::TexturePoint _origin;

		void animation_subupdate() final {}
	};
}
