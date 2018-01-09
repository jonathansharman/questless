//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/still.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	void still::draw(units::screen_space::point position) const
	{
		texture const& texture = the_texture_manager()[_texture_handle];
		screen_space::point dst_point{position.x() - _origin.u(), position.y() - _origin.v()};
		texture.draw(dst_point);
	}

	void still::draw(units::game_space::point position, camera const& camera, colors::color_vector color) const
	{
		camera.draw
			( the_texture_manager()[_texture_handle]
			, units::game_space::point{position}
			, origin{_origin}
			, color
			, h_scale{1.0}
			, v_scale{1.0}
			, game_space::radians{0.0}
			);
	}
}
