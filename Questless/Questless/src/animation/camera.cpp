//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "camera.hpp"

#include <cmath>

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql {
	void camera::update() {
		window_space::vector center_to_mouse = the_input().mouse_position() - the_window().window_center();
		world_space::vector scaled_center_to_mouse = world_space::vector
			{ static_cast<double>(center_to_mouse.x())
			, static_cast<double>(-center_to_mouse.y())
			} / zoom;
		_point_hovered = position + scaled_center_to_mouse;
		_point_hovered.rotate(position, _angle);
		_tile_hovered = to_region_tile(_point_hovered);
	}

	void camera::draw
		( texture const& texture
		, world_space::point position
		, texture_space::vector origin
		, colors::color_vector draw_color_vector
		, view_space::vector scale
		, world_space::radians angle
		, std::optional<texture_space::box> const& src_rect
		) const
	{
		texture.draw_transformed
			( to_window_point(position)
			, origin
			, draw_color_vector * color_vector
			, scale
			, angle - _angle
			, src_rect
			);
	}

	void camera::draw_lines(std::vector<world_space::point> points, colors::color color) const {
		// Transform segment end points.
		std::vector<window_space::point> window_points;
		for (world_space::point const& point : points) {
			window_points.push_back(to_window_point(point));
		}

		// Draw transformed line segments using the renderer.
		the_renderer().draw_lines(window_points, color);
	}

	window_space::point camera::to_window_point(world_space::point point) const {
		world_space::point const window_center = world_space::point{0.0, 0.0} + world_space::vector{static_cast<double>(the_window().width()), static_cast<double>(the_window().height())} / 2.0;
		world_space::vector const camera_to_point = point - position;
		world_space::point const scaled_point = zoom.value() * world_space::vector{camera_to_point.x(), -camera_to_point.y()} + window_center;
		world_space::point const rotated_scaled_point = scaled_point.rotated(window_center, _angle);
		return window_space::point{lround(rotated_scaled_point.x()), lround(rotated_scaled_point.y())};
	}
}
