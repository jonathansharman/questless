//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Texture.h"
#include "sdl/Window.h"
#include "sdl/Input.h"
#include "world/coordinates.h"
#include "utility/utility.h"
#include "utility/TaggedType.h"

namespace questless
{
	struct Origin : TaggedType<std::optional<units::GameSpace::Point>> { using TaggedType::TaggedType; };
	struct HScale : TaggedType<double> { using TaggedType::TaggedType; };
	struct VScale : TaggedType<double> { using TaggedType::TaggedType; };
	struct SrcRect : TaggedType<std::optional<units::TextureSpace::Box>> { using TaggedType::TaggedType; };

	//! A simple 2D camera, useful for panning, zooming, and rotating around a scene.
	class Camera
	{
	public:
		//! A color factor to apply to anything drawn using this camera.
		units::colors::ColorFactor color_factor;

		//! Constructs a camera with the specified starting position.
		//! @param position The starting position of the camera.
		Camera(units::GameSpace::Point position)
			: color_factor{units::colors::white_factor()}
			, _position{position}
			, _zoom{1.0}
			, _angle{0}
		{}

		Camera& operator =(Camera const&) = delete;

		//! The camera's position.
		units::GameSpace::Point position() const { return _position; }

		//! Sets the camera's position.
		//! @param position The new camera position.
		void position(units::GameSpace::Point const& position) { _position = position; }

		//! Pans the camera the specified amount.
		//! @param offset The offset which is added to the camera's position.
		void pan(units::GameSpace::Vector const& offset) { _position += offset; }

		//! The camera's zoom.
		inline double zoom() const { return _zoom; }

		//! Sets the camera's zoom.
		//! @param zoom A non-negative double for the zoom. If zoom is negative, it is clamped to 0.
		void zoom(double zoom) { _zoom = zoom < 0 ? 0 : zoom; }

		//! Multiplies the camera's zoom by the provided factor.
		//! @param factor A non-negative double by which zoom is multiplied.
		void zoom_factor(double factor);

		//! The camera's counter-clockwise rotation in radians, in the range [-pi, pi].
		units::GameSpace::Radians angle() const { return _angle; }

		//! The camera's counter-clockwise rotation in radians, in the range [0, tau).
		units::GameSpace::Radians positive_angle() const;

		//! Sets the camera's angle.
		//! @param theta The camera's counter-clockwise rotation in radians, in the range [0, tau).
		void angle(units::GameSpace::Radians theta);

		//! Adjusts the camera's angle.
		//! @param dtheta The number of radians to offset the angle.
		void rotate(units::GameSpace::Radians dtheta);

		//! The game point the mouse is hovering over.
		units::GameSpace::Point point_hovered() const { return _point_hovered; }

		//! The hex coordinates of the tile the mouse is hovering over.
		RegionTile::Point tile_hovered() const { return _tile_hovered; }

		//! Updates the camera. To be called once per frame.
		void update();

		//! Draws all or part of the provided texture with respect to the camera.
		//! @param texture The texture to be drawn.
		//! @param position The in-game coordinates of the texture.
		//! @param origin The origin point within the texture. If nullopt, the texture's center is used.
		//! @param color An additional color factor, applied on top of the camera's color factor.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the texture, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( sdl::Texture const& texture
			, units::GameSpace::Point position
			, Origin origin = Origin{std::nullopt}
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, HScale horizontal_scale = HScale{1.0}
			, VScale vertical_scale = VScale{1.0}
			, units::GameSpace::Radians angle = units::GameSpace::Radians{0.0}
			, SrcRect const& src_rect = SrcRect{std::nullopt}
			) const;

		//! Draws lines relative to the camera connecting the series of points contained in the vector.
		//! @param points A vector of game points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<units::GameSpace::Point> points, units::colors::Color color) const;
	private:
		units::GameSpace::Point _position;
		double _zoom;
		units::GameSpace::Radians _angle;

		units::GameSpace::Point _point_hovered;
		RegionTile::Point _tile_hovered;

		//! The given game point transformed to screen space, accounting for the camera.
		units::ScreenSpace::Point screen_point(units::GameSpace::Point point) const;
	};
}
