//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/Camera.h"

#include <cmath>

#include "sdl/resources.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void Camera::zoom_factor(double factor)
	{
		if (factor >= 0.0) {
			_zoom *= factor;
		}
	}

	GameSpace::Radians Camera::positive_angle() const
	{
		return _angle < GameSpace::Radians::zero() ? _angle + GameSpace::Radians::circle() : _angle;
	}

	void Camera::angle(GameSpace::Radians theta)
	{
		_angle.count() = fmod(theta.count(), GameSpace::Radians::circle().count() / 2.0);
	}

	void Camera::rotate(GameSpace::Radians dtheta)
	{
		_angle.count() = fmod((_angle + dtheta).count(), GameSpace::Radians::circle().count() / 2.0);
	}

	void Camera::update()
	{
		ScreenSpace::Vector center_to_mouse = input().mouse_position() - window().center();
		GameSpace::Vector scaled_center_to_mouse = GameSpace::Vector{static_cast<double>(center_to_mouse.x()), static_cast<double>(-center_to_mouse.y())} / _zoom;
		_point_hovered = _position + scaled_center_to_mouse;
		_point_hovered.rotate(_position, _angle);
		_tile_hovered = Layout::dflt().to_hex_coords<RegionTile::Point>(_point_hovered);
	}

	void Camera::draw
		( Texture const& texture
		, GameSpace::Point position
		, Origin origin
		, colors::ColorFactor draw_color_factor
		, HScale horizontal_scale
		, VScale vertical_scale
		, GameSpace::Radians angle
		, SrcRect const& src_rect
		) const
	{
		if (origin.value) {
			position += GameSpace::Vector{texture.width() / 2 - origin.value->x(), texture.height() / 2 - origin.value->y()};
		}
		texture.draw_transformed
			( screen_point(position)
			, std::nullopt
			, draw_color_factor * color_factor
			, _zoom * horizontal_scale
			, _zoom * vertical_scale
			, angle - _angle
			, src_rect
			);
	}

	void Camera::draw_lines(std::vector<GameSpace::Point> points, colors::Color color) const
	{
		// Transform segment end points.
		std::vector<ScreenSpace::Point> screen_points;
		for (GameSpace::Point const& point : points) {
			screen_points.push_back(screen_point(point));
		}

		// Draw transformed line segments using the renderer.
		renderer().draw_lines(screen_points, color);
	}

	ScreenSpace::Point Camera::screen_point(GameSpace::Point point) const
	{
		GameSpace::Point const window_center = GameSpace::Point{0.0, 0.0} + GameSpace::Vector{static_cast<double>(window().width()), static_cast<double>(window().height())} / 2.0;
		GameSpace::Vector const camera_to_point = point - _position;
		GameSpace::Point const scaled_point = _zoom * GameSpace::Vector{camera_to_point.x(), -camera_to_point.y()} + window_center;
		GameSpace::Point const rotated_scaled_point = scaled_point.rotated(window_center, _angle);
		return ScreenSpace::Point{lround(rotated_scaled_point.x()), lround(rotated_scaled_point.y())};
	}
}
