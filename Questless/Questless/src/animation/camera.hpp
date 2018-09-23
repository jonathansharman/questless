//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/utility.hpp"
#include "world/coordinates.hpp"

#include "units/colors.hpp"
#include "units/game_space.hpp"
#include "units/texture_space.hpp"
#include "units/view_space.hpp"
#include "units/window_space.hpp"

#include <optional>

namespace sdl {
	class texture;
}

namespace ql {
	//! A simple 2D camera, useful for panning, zooming, and rotating around a scene.
	class camera {
	public:
		//! A color vector to apply to anything drawn using this camera.
		units::colors::color_vector color_vector;

		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		camera(units::game_space::point position)
			: color_vector{units::colors::white_vector()}
			, _position{position}
			, _zoom{1.0}
			, _angle{0.0}
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
		//! @param color An additional color vector, applied on top of the camera's color vector.
		//! @param scale The scale of the texture.
		//! @param angle The counter-clockwise rotation of the texture, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( sdl::texture const& texture
			, units::game_space::point position
			, units::texture_space::vector origin = units::texture_space::vector::zero()
			, units::colors::color_vector color_vector = units::colors::white_vector()
			, units::view_space::vector scale = units::view_space::vector{1.0f, 1.0f}
			, units::game_space::radians angle = units::game_space::radians{0.0}
			, std::optional<units::texture_space::box> const& src_rect = std::nullopt
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

		//! The given game point transformed to window space, accounting for the camera.
		units::window_space::point to_window_point(units::game_space::point point) const;
	};
}
