//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/nonnegative.hpp"
#include "utility/utility.hpp"
#include "utility/static_property.hpp"
#include "world/coordinates.hpp"

#include "units/colors.hpp"
#include "units/world_space.hpp"
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
	private:
		static void angle_mutator(units::world_space::radians& theta, units::world_space::radians const& new_theta) {
			theta = units::world_space::radians{fmod(new_theta.count(), units::world_space::radians::circle().count())};
		}
	public:
		//! A color vector to apply to anything drawn using this camera.
		spaces::colors::color_vector color_vector;

		units::world_space::point position;

		units::world_space::radians angle{0.0};

		nonnegative<double> zoom = 0.0;

		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		camera(units::world_space::point position)
			: color_vector{spaces::colors::white_vector()}
			, position{position}
		{}

		camera& operator =(camera const&) = delete;

		//! The camera's counter-clockwise rotation in radians, in the range [0, tau).
		static_property<units::world_space::radians, angle_mutator> angle;

		//! The game point the mouse is hovering over.
		units::world_space::point point_hovered() const { return _point_hovered; }

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
			, units::world_space::point position
			, units::texture_space::vector origin = units::texture_space::vector::zero()
			, spaces::colors::color_vector color_vector = spaces::colors::white_vector()
			, spaces::view::vector scale = spaces::view::vector{1.0f, 1.0f}
			, units::world_space::radians angle = units::world_space::radians{0.0}
			, std::optional<units::texture_space::box> const& src_rect = std::nullopt
			) const;

		//! Draws lines relative to the camera connecting the series of points contained in the vector.
		//! @param points A vector of game points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<units::world_space::point> points, spaces::colors::color color) const;
	private:
		units::world_space::point _point_hovered;
		region_tile::point _tile_hovered;

		//! The given game point transformed to window space, accounting for the camera.
		spaces::window::point to_window_point(units::world_space::point point) const;
	};
}
