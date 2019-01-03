//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/camera.hpp"
#include "animation/still.hpp"

using namespace sdl;
using namespace sdl::spaces::view::literals;

using namespace vecx::literals;

namespace ql {
	void still::draw(spaces::window::point position) const {
		texture const& texture = the_texture_manager()[_texture_handle];
		spaces::window::point dst_point{x(position) - _origin[0], y(position) - _origin[1]};
		texture.draw(dst_point);
	}

	void still::draw(world::point position, camera const& camera, spaces::colors::color color) const {
		camera.draw
			( the_texture_manager()[_texture_handle]
			, world::point{position}
			, _origin
			, color
			, {1.0_view_length, 1.0_view_length}
			, 0.0_rad
			);
	}
}
