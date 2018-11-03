//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/camera.hpp"
#include "animation/still.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	void still::draw(window_space::point position) const {
		texture const& texture = the_texture_manager()[_texture_handle];
		window_space::point dst_point{position.x() - _origin.u(), position.y() - _origin.v()};
		texture.draw(dst_point);
	}

	void still::draw(world_space::point position, camera const& camera, colors::color_vector color) const {
		camera.draw
			( the_texture_manager()[_texture_handle]
			, world_space::point{position}
			, _origin
			, color
			, view_space::vector{1.0f, 1.0f}
			, world_space::radians{0.0}
			);
	}
}
