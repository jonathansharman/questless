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

#include "sdl-wrappers/resources.h"

using namespace units;

namespace questless
{
	void Camera::zoom_factor(double factor)
	{
		if (factor >= 0.0) {
			_zoom *= factor;
		}
	}

	GameRadians Camera::positive_angle() const
	{
		return _angle < GameRadians::zero() ? _angle + GameRadians::circle() : _angle;
	}

	void Camera::angle(GameRadians theta)
	{
		_angle.count() = fmod(theta.count(), GameRadians::circle().count() / 2.0);
	}

	void Camera::rotate(GameRadians dtheta)
	{
		_angle.count() = fmod((_angle + dtheta).count(), GameRadians::circle().count() / 2.0);
	}

	void Camera::update(const sdl::Input& input)
	{
		ScreenVector center_to_mouse = input.mouse_position() - _window.center();
		GameVector scaled_center_to_mouse = GameVector{static_cast<double>(center_to_mouse.x), static_cast<double>(-center_to_mouse.y)} / _zoom;
		_point_hovered = _position + scaled_center_to_mouse;
		_point_hovered.rotate(_position, _angle);
		_tile_hovered = Layout::dflt().to_hex_coords<RegionTileCoords>(_point_hovered);
	}

	void Camera::draw
		( const sdl::Texture& texture
		, GamePoint position
		, Origin origin
		, sdl::Color color
		, HScale horizontal_scale
		, VScale vertical_scale
		, GameRadians angle
		, HFlip flip_horizontally
		, VFlip flip_vertically
		, const SrcRect& src_rect
		) const
	{
		if (origin.value) {
			position += GameVector{texture.width() / 2 - origin.value->x, texture.height() / 2 - origin.value->y};
		}
		sdl::Color mixed_color = sdl::Color
			{ static_cast<uint8_t>((static_cast<uint32_t>(color.r) * _color.r) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.g) * _color.g) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.b) * _color.b) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.a) * _color.a) / 255)
			};
		texture.draw_transformed
			( screen_point(position)
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

	void Camera::draw_lines(std::vector<GamePoint> points, sdl::Color color) const
	{
		// Transform segment end points.
		std::vector<ScreenPoint> screen_points;
		for (const GamePoint& point : points) {
			screen_points.push_back(screen_point(point));
		}

		// Draw transformed line segments using the renderer.
		sdl::renderer().draw_lines(screen_points, color);
	}

	ScreenPoint Camera::screen_point(GamePoint point) const
	{
		const GamePoint game_window_center = GamePoint{static_cast<double>(_window.width()), static_cast<double>(_window.height())} / 2.0;
		const GameVector camera_to_point = point - _position;
		const GamePoint scaled_point = _zoom * GameVector{camera_to_point.x, -camera_to_point.y} + game_window_center;
		const GamePoint rotated_scaled_point = scaled_point.rotated(game_window_center, _angle);
		return ScreenPoint{lround(rotated_scaled_point.x), lround(rotated_scaled_point.y)};
	}
}
