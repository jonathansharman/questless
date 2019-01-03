//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/nonnegative.hpp"
#include "utility/utility.hpp"
#include "utility/static_property.hpp"
#include "world/coordinates.hpp"
#include "world/world.hpp"

#include "sdl/spaces/colors.hpp"
#include "sdl/spaces/view.hpp"
#include "sdl/spaces/window.hpp"

#include <optional>

namespace sdl {
	class texture;
}

namespace ql {
	//! A simple 2D camera, useful for panning, zooming, and rotating around a scene.
	class camera {
	private:
		static void angle_mutator(vecx::radians& theta, vecx::radians const& new_theta) {
			theta = fmod(new_theta, vecx::circle_rad.value);
		}
	public:
		//! A color factor to apply to anything drawn using this camera.
		sdl::spaces::colors::color color_factor;

		world::point position;

		//! The camera's counter-clockwise rotation in radians, in the range [0, tau).
		static_property<vecx::radians, angle_mutator> angle;

		nonnegative<double> zoom = 0.0;

		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		camera(world::point position)
			: color_factor{sdl::spaces::colors::white()}
			, position{position}
		{}

		camera& operator =(camera const&) = delete;

		//! The game point the mouse is hovering over.
		world::point point_hovered() const { return _point_hovered; }

		//! The hex coordinates of the tile the mouse is hovering over.
		region_tile::point tile_hovered() const { return _tile_hovered; }

		//! Updates the camera. To be called once per frame.
		void update();

		//! Draws all or part of the provided texture with respect to the camera.
		//! @param texture The texture to be drawn.
		//! @param position The in-game coordinates of the texture.
		//! @param origin The offset from the texture's center to its origin point.
		//! @param color_factor An additional color factor, applied on top of the camera's color vector.
		//! @param scale The scale of the texture.
		//! @param angle The counter-clockwise rotation of the texture, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( sdl::texture const& texture
			, world::point position
			, sdl::spaces::window::vector origin = sdl::spaces::window::vector::zero()
			, sdl::spaces::colors::color color_factor = sdl::spaces::colors::white()
			, sdl::spaces::view::vector scale = sdl::spaces::view::vector{sdl::spaces::view::length{1.0}, sdl::spaces::view::length{1.0}}
			, vecx::radians angle = vecx::radians{0.0}
			, std::optional<sdl::spaces::window::box> const& src_rect = std::nullopt
			) const;

		//! Draws lines relative to the camera connecting the series of points contained in the vector.
		//! @param points A vector of game points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<world::point> points, sdl::spaces::colors::color color) const;
	private:
		world::point _point_hovered;
		region_tile::point _tile_hovered;

		//! The given game point transformed to window space, accounting for the camera.
		sdl::spaces::window::point to_window_point(world::point point) const;
	};
}
