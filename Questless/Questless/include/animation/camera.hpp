//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/texture.hpp"
#include "sdl/window.hpp"
#include "sdl/input.hpp"
#include "world/coordinates.hpp"
#include "utility/utility.hpp"
#include "utility/tagged_type.hpp"

namespace ql
{
	struct origin : tagged_type<units::texture_space::vector> { using tagged_type::tagged_type; };
	struct h_scale : tagged_type<double> { using tagged_type::tagged_type; };
	struct v_scale : tagged_type<double> { using tagged_type::tagged_type; };
	struct src_rect : tagged_type<std::optional<units::texture_space::box>> { using tagged_type::tagged_type; };

	//! A simple 2D camera, useful for panning, zooming, and rotating around a scene.
	class camera
	{
	public:
		//! A color factor to apply to anything drawn using this camera.
		units::colors::color_factor color_factor;

		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		camera(units::game_space::point position)
			: color_factor{units::colors::white_factor()}
			, _position{position}
			, _zoom{1.0}
			, _angle{0}
		{}

		camera& operator =(camera const&) = delete;

		//! The camera's position.
		units::game_space::point position() const { return _position; }

		//! Sets the camera's position.
		//! @param position The new camera position.
		void position(units::game_space::point const& position) { _position = position; }

		//! Pans the camera the specified amount.
		//! @param offset The offset which is added to the camera's position.
		void pan(units::game_space::vector const& offset) { _position += offset; }

		//! The camera's zoom.
		inline double zoom() const { return _zoom; }

		//! Sets the camera's zoom.
		//! @param zoom A non-negative double for the zoom. If zoom is negative, it is clamped to 0.
		void zoom(double zoom) { _zoom = zoom < 0 ? 0 : zoom; }

		//! Multiplies the camera's zoom by the provided factor.
		//! @param factor A non-negative double by which zoom is multiplied.
		void zoom_factor(double factor);

		//! The camera's counter-clockwise rotation in radians, in the range [-pi, pi].
		units::game_space::radians angle() const { return _angle; }

		//! The camera's counter-clockwise rotation in radians, in the range [0, tau).
		units::game_space::radians positive_angle() const;

		//! Sets the camera's angle.
		//! @param theta The camera's counter-clockwise rotation in radians, in the range [0, tau).
		void angle(units::game_space::radians theta);

		//! Adjusts the camera's angle.
		//! @param dtheta The number of radians to offset the angle.
		void rotate(units::game_space::radians dtheta);

		//! The game point the mouse is hovering over.
		units::game_space::point point_hovered() const { return _point_hovered; }

		//! The hex coordinates of the tile the mouse is hovering over.
		region_tile::point tile_hovered() const { return _tile_hovered; }

		//! Updates the camera. To be called once per frame.
		void update();

		//! Draws all or part of the provided texture with respect to the camera.
		//! @param texture The texture to be drawn.
		//! @param position The in-game coordinates of the texture.
		//! @param origin The offset from the texture's center to its origin point.
		//! @param color An additional color factor, applied on top of the camera's color factor.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the texture, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( sdl::texture const& texture
			, units::game_space::point position
			, origin origin = origin{units::texture_space::vector::zero()}
			, units::colors::color_factor color_factor = units::colors::white_factor()
			, h_scale horizontal_scale = h_scale{1.0}
			, v_scale vertical_scale = v_scale{1.0}
			, units::game_space::radians angle = units::game_space::radians{0.0}
			, src_rect const& src_rect = src_rect{std::nullopt}
			) const;

		//! Draws lines relative to the camera connecting the series of points contained in the vector.
		//! @param points A vector of game points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<units::game_space::point> points, units::colors::color color) const;
	private:
		units::game_space::point _position;
		double _zoom;
		units::game_space::radians _angle;

		units::game_space::point _point_hovered;
		region_tile::point _tile_hovered;

		//! The given game point transformed to screen space, accounting for the camera.
		units::screen_space::point screen_point(units::game_space::point point) const;
	};
}
