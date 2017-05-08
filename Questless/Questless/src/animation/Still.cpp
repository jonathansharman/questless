//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/Still.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void Still::draw(units::ScreenPoint position) const
	{
		Texture const& texture = texture_manager()[_texture_handle];
		ScreenPoint dst_point{lround(position.x - _origin.x), lround(position.y - _origin.y)};
		texture.draw(dst_point);
	}

	void Still::draw(units::GamePoint position, Camera const& camera, Color color) const
	{
		camera.draw
			( texture_manager()[_texture_handle]
			, units::GamePoint{position - GameVector{static_cast<double>(_origin.x), static_cast<double>(-_origin.y)}} //! @todo Uncouth point casting here.
			, Origin{std::nullopt}
			, color
			, HScale{1.0}
			, VScale{1.0}
			, GameRadians{0.0}
			, HFlip{false}
			, VFlip{false}
			);
	}
}
