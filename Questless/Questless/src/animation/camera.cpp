//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "camera.hpp"

#include <cmath>

#include "sdl/resources.hpp"

using namespace ql::world::literals;

using namespace sdl;
using namespace sdl::spaces::window::literals;

using namespace vecx;

namespace ql {
	void camera::update() {
		spaces::window::vector center_to_mouse = the_input().mouse_position() - the_window().window_center();
		world::vector scaled_center_to_mouse = world::vector
			{ center_to_mouse[0] * 1.0_world_length / 1_px
			, center_to_mouse[1] * -1.0_world_length / 1_px
			} / zoom;
		_point_hovered = position + scaled_center_to_mouse;
		_point_hovered.rotate(position, angle);
		_tile_hovered = to_region_tile(_point_hovered);
	}

	void camera::draw
		( texture const& texture
		, world::point draw_position
		, spaces::window::vector origin
		, spaces::colors::color draw_color_factor
		, spaces::view::vector scale
		, radians draw_angle
		, std::optional<spaces::window::box> const& src_rect
		) const
	{
		texture.draw_transformed
			( to_window_point(draw_position)
			, origin
			, component_wise_quotient(component_wise_product(draw_color_factor, color_factor), spaces::colors::white())
			, scale
			, draw_angle - angle.value()
			, src_rect
			);
	}

	void camera::draw_lines(std::vector<world::point> points, spaces::colors::color color) const {
		// Transform segment end points.
		std::vector<spaces::window::point> window_points;
		for (auto const& point : points) {
			window_points.push_back(to_window_point(point));
		}

		// Draw transformed line segments using the renderer.
		the_renderer().draw_lines(window_points, color);
	}

	spaces::window::point camera::to_window_point(world::point point) const {
		world::point const window_center = world::point{0.0_world_length, 0.0_world_length} + world::vector
			{ the_window().width()
			, the_window().height()
			} / 2.0;
		world::vector const camera_to_point = point - position;
		world::point const scaled_point = zoom.value() * world::vector{x(camera_to_point), -y(camera_to_point)} + window_center;
		world::point const rotated_scaled_point = scaled_point.rotated(window_center, angle);
		return spaces::window::point
			{ spaces::window::px{lround(x(rotated_scaled_point).value)}
			, spaces::window::px{lround(y(rotated_scaled_point).value)}
			};
	}
}
