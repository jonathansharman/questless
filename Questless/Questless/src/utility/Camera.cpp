/**
* @file    Camera.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Camera class.
*/

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

	void Camera::angle(double theta)
	{
		_angle = fmod(theta, 180.0);
	}

	void Camera::rotate(double dtheta)
	{
		_angle = fmod(_angle + dtheta, 180.0);
	}

	void Camera::update(const Input& input)
	{
		_pt_hovered = _position + (input.mouse_position() - _window.center()) / _zoom;
		_pt_hovered.rotate(_position, _angle);
		_pt_hovered_rounded = Layout::dflt().to_world(Layout::dflt().to_hex_coords(_pt_hovered));
	}

	void Camera::draw
		( const Texture& texture
		, Point position
		, const optional<Point>& origin
		, double horizontal_scale
		, double vertical_scale
		, bool flip_horizontally
		, bool flip_vertically
		, double angle
		, Color color
		, const optional<Rect>& src_rect) const
	{
		if (origin) {
			position += Vector(texture.width() / 2 - origin->x, texture.height() / 2 - origin->y);
		}
		Point window_resolution = _window.resolution();
		PointF relative_position = _zoom * (static_cast<PointF>(position)-_position) + window_resolution / 2;
		relative_position.rotate(window_resolution / 2, -_angle);
		Color mixed_color = Color
			{ static_cast<uint8_t>((static_cast<uint32_t>(color.r) * _color.r) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.g) * _color.g) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.b) * _color.b) / 255)
			};
		texture.draw_transformed
			( relative_position.to_point()
			, nullopt
			, _zoom * horizontal_scale
			, _zoom * vertical_scale
			, flip_horizontally
			, flip_vertically
			, mixed_color
			, angle - _angle
			, src_rect
			);
	}
}