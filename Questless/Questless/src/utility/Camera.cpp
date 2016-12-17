/**
* @file    Camera.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Camera class.
*/

#include "sdl-wrappers/resources.h"
#include "animation/Camera.h"

#include <cmath>

namespace questless
{
	void Camera::zoom_factor(double factor)
	{
		if (factor >= 0.0) {
			_zoom *= factor;
		}
	}

	double Camera::positive_angle() const
	{
		return _angle < 0.0 ? _angle + 360.0 : _angle;
	}

	void Camera::angle(AngleRadians theta)
	{
		_angle = fmod(theta, tau / 2.0);
	}

	void Camera::rotate(AngleRadians dtheta)
	{
		_angle = fmod(_angle + dtheta, tau / 2.0);
	}

	void Camera::update(const sdl::Input& input)
	{
		_pt_hovered = _position + VectorF{input.mouse_position() - _window.center()} / _zoom;
		_pt_hovered.rotate(_position, AngleRadians{_angle});
		_pt_hovered_rounded = Layout::dflt().to_world(Layout::dflt().to_hex_coords<RegionTileCoords>(_pt_hovered)).to_point();
	}

	void Camera::draw
		( const sdl::Texture& texture
		, sdl::Point position
		, Origin origin
		, sdl::Color color
		, HScale horizontal_scale
		, VScale vertical_scale
		, AngleRadians angle
		, HFlip flip_horizontally
		, VFlip flip_vertically
		, const SrcRect& src_rect
		) const
	{
		if (origin.value()) {
			position += sdl::Vector{texture.width() / 2 - origin.value()->x, texture.height() / 2 - origin.value()->y};
		}
		sdl::Color mixed_color = sdl::Color
			{ static_cast<uint8_t>((static_cast<uint32_t>(color.r) * _color.r) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.g) * _color.g) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.b) * _color.b) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.a) * _color.a) / 255)
			};
		texture.draw_transformed
			( relative_point(position)
			, boost::none
			, mixed_color
			, _zoom * horizontal_scale
			, _zoom * vertical_scale
			, angle - _angle
			, flip_horizontally
			, flip_vertically
			, src_rect
			);
	}

	void Camera::draw_lines(std::vector<sdl::Point> points, sdl::Color color) const
	{
		// Transform segment end points.
		for (sdl::Point& point : points) {
			point = relative_point(point);
		}
		// Draw transformed line segments using the renderer.
		sdl::renderer().draw_lines(points, color);
	}

	PointF Camera::relative_point(PointF point) const
	{
		PointF window_center = PointF{_window.resolution()} / 2.0;
		return (_zoom * (point - _position) + window_center).rotated(window_center, AngleRadians{_angle});
	}
}
