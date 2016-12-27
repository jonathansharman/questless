/**
* @file    Camera.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Camera class.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "sdl-wrappers/Texture.h"
#include "sdl-wrappers/Window.h"
#include "sdl-wrappers/Input.h"
#include "world/coordinates.h"
#include "utility/utility.h"
#include "utility/Point.h"
#include "utility/TaggedType.h"

namespace questless
{
	using Origin = TaggedType<boost::optional<GamePoint>, struct OriginTag>;
	using HScale = TaggedType<double, struct HScaleTag>;
	using VScale = TaggedType<double, struct VScaleTag>;
	using HFlip = TaggedType<bool, struct HScaleTag>;
	using VFlip = TaggedType<bool, struct VScaleTag>;
	using SrcRect = TaggedType<boost::optional<TextureRect>, struct SrcRectTag>;

	class Camera
	{
	public:
		using ptr = std::unique_ptr<Camera>;

		/// Constructs a camera with the specified starting position.
		/// @param window The window.
		/// @param position The starting position of the camera.
		Camera(sdl::Window& window, GamePoint position)
			: _window{window}
			, _position{position}
			, _zoom{1.0}
			, _angle{0}
			, _color{sdl::Color::white()}
		{}

		/// @return A unique camera pointer constructed with the given arguments.
		template <typename... Args>
		static ptr make(Args&&... args) { return make_unique<Camera>(std::forward<Args>(args)...); }

		Camera& operator =(const Camera&) = delete;

		/// @return The camera's reference to the window.
		sdl::Window& window() { return _window; }

		/// @return The camera's reference to the window.
		const sdl::Window& window() const { return _window; }

		/// @return The camera's position.
		GamePoint position() const { return _position; }

		/// Sets the camera's position.
		/// @param position The new camera position.
		void position(const GamePoint& position) { _position = position; }

		/// Pans the camera the specified amount.
		/// @param offset The offset which is added to the camera's position.
		void pan(const GameVector& offset) { _position += offset; }

		/// @return The camera's zoom.
		inline double zoom() const { return _zoom; }

		/// Sets the camera's zoom.
		/// @param zoom A non-negative double for the zoom. If zoom is negative, it is clamped to 0.
		void zoom(double zoom) { _zoom = zoom < 0 ? 0 : zoom; }

		/// Multiplies the camera's zoom by the provided factor.
		/// @param factor A non-negative double by which zoom is multiplied.
		void zoom_factor(double factor);

		/// @return The camera's counter-clockwise rotation in degrees, in the range [-180, 180].
		AngleRadians angle() const { return AngleRadians{_angle}; }

		/// @return The camera's counter-clockwise rotation in degrees, in the range [0, 360).
		double positive_angle() const;

		/// Sets the camera's angle.
		/// @param theta A double in the range [0, tau) representing the camera's counter-clockwise rotation in radians.
		void angle(AngleRadians theta);

		/// Adjusts the camera's angle.
		/// @param dtheta The number of radians to offset the angle.
		void rotate(AngleRadians dtheta);

		/// @return The camera's color multiplier.
		sdl::Color color() const { return _color; }

		/// Sets the camera's color multiplier.
		/// @param color The new color multiplier.
		sdl::Color color(sdl::Color color) { _color = color; }

		/// @return The game point the mouse is hovering over.
		GamePoint point_hovered() const { return _point_hovered; }

		/// @return The hex coordinates of the tile the mouse is hovering over.
		RegionTileCoords tile_hovered() const { return _tile_hovered; }

		void update(const sdl::Input& input);

		/// Draws all or part of the provided texture with respect to the camera.
		/// @param texture The texture to be drawn.
		/// @param position The in-game coordinates of the texture.
		/// @param origin The origin point within the texture. If nullopt, the texture's center is used.
		/// @param color An additional color multiplier, applied on top of the camera's and texture's color members.
		/// @param horizontal_scale The horizontal scale of the texture.
		/// @param vertical_scale The vertical scale of the texture.
		/// @param angle The counter-clockwise rotation of the texture, in radians.
		/// @param flip_horizontally Whether to flip the texture horizontally.
		/// @param flip_horizontally Whether to flip the texture vertically.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( const sdl::Texture& texture
			, GamePoint position
			, Origin origin = Origin{boost::none}
			, sdl::Color color = sdl::Color::white()
			, HScale horizontal_scale = HScale{1.0}
			, VScale vertical_scale = VScale{1.0}
			, AngleRadians angle = AngleRadians{0.0}
			, HFlip flip_horizontally = HFlip{false}
			, VFlip flip_vertically = VFlip{false}
			, const SrcRect& src_rect = SrcRect{boost::none}
			) const;

		/// Draws lines relative to the camera connecting the series of points contained in the vector.
		/// @param points A vector of game points.
		/// @param color The color of the lines.
		void draw_lines(std::vector<GamePoint> points, sdl::Color color) const;
	private:
		sdl::Window& _window;
		GamePoint _position;
		double _zoom;
		double _angle;
		sdl::Color _color;

		GamePoint _point_hovered;
		RegionTileCoords _tile_hovered;

		/// @return The given game point transformed to screen space, accounting for the camera.
		ScreenPoint screen_point(GamePoint point) const;
	};
}

#endif
