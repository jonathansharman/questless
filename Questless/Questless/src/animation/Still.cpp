//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/Still.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void Still::draw(units::ScreenSpace::Point position) const
	{
		Texture const& texture = texture_manager()[_texture_handle];
		ScreenSpace::Point dst_point{lround(position.x() - _origin.u()), lround(position.y() - _origin.v())};
		texture.draw(dst_point);
	}

	void Still::draw(units::GameSpace::Point position, Camera const& camera, colors::ColorFactor color) const
	{
		camera.draw
			( texture_manager()[_texture_handle]
			, units::GameSpace::Point{position}
			, Origin{_origin}
			, color
			, HScale{1.0}
			, VScale{1.0}
			, GameSpace::Radians{0.0}
			);
	}
}
