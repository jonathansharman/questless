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

#include "utility/utility.h"
#include "utility/PointF.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "sdl-wrappers/Texture.h"
#include "sdl-wrappers/Window.h"
#include "sdl-wrappers/Input.h"
#include "world/coordinates.h"

namespace questless
{
	class Camera
	{
	public:
		/// Constructs a camera with the specified starting position.
		/// @param window The window.
		/// @param position The starting position of the camera.
		Camera(sdl::Window& window, PointF position)
			: _window{window}
			, _position{position}
			, _zoom{1.0}
			, _angle{0}
			, _color{sdl::Color::white()}
		{}

		/// Constructs a camera with the specified starting position.
		/// @param window The window.
		/// @param position The starting position of the camera.
		Camera(sdl::Window& window, sdl::Point position)
			: _window{window}
			, _position{position}
			, _zoom{1.0}
			, _angle{0}
			, _color{sdl::Color::white()}
		{}

		Camera& operator =(const Camera&) = delete;

		/// @return The camera's reference to the window.
		sdl::Window& window() { return _window; }

		/// @return The camera's reference to the window.
		const sdl::Window& window() const { return _window; }

		/// @return The camera's position.
		PointF position() const { return _position; }

		/// Sets the camera's position.
		/// @param position The new camera position.
		void position(const PointF& position) { _position = position; }

		/// Pans the camera the specified amount.
		/// @param offset The offset which is added to the camera's position.
		void pan(VectorF offset) { _position += offset; }

		/// @return The camera's zoom.
		inline double zoom() const { return _zoom; }

		/// Sets the camera's zoom.
		/// @param zoom A non-negative double for the zoom. If zoom is negative, it is clamped to 0.
		void zoom(double zoom) { _zoom = zoom < 0 ? 0 : zoom; }

		/// Multiplies the camera's zoom by the provided factor.
		/// @param factor A non-negative double by which zoom is multiplied.
		void zoom_factor(double factor);

		/// @return The camera's counter-clockwise rotation in degrees, in the range [-180, 180].
		double angle() const { return _angle; }

		/// @return The camera's counter-clockwise rotation in degrees, in the range [0, 360).
		double positive_angle() const;

		/// Sets the camera's angle.
		/// @param theta A double in the range [0, 360) representing the camera's counter-clockwise rotation in degrees.
		void angle(double theta);

		/// Adjusts the camera's angle.
		/// @param dtheta The number of degrees to offset the angle.
		void rotate(double dtheta);

		/// @return The camera's color multiplier.
		sdl::Color color() const { return _color; }

		/// Sets the camera's color multiplier.
		/// @param color The new color multiplier.
		sdl::Color color(sdl::Color color) { _color = color; }

		/// @return The exact point the mouse is hovering over.
		PointF pt_hovered() const { return _pt_hovered; }

		/// @return The rounded point the mouse is hovering over.
		sdl::Point pt_hovered_rounded() const { return _pt_hovered_rounded; }

		/// @return The hex coordinates of the tile the mouse is hovering over.
		RegionTileCoords tile_hovered() const { return {Layout::dflt().to_hex_coords<RegionTileCoords>(PointF{_pt_hovered_rounded})}; }

		void update(const sdl::Input& input);

		/// Draws all or part of the provided texture with respect to the camera.
		/// @param texture The texture to be drawn.
		/// @param position The in-game coordinates of the texture.
		/// @param origin The origin point within the texture. If nullopt, the texture's center is used.
		/// @param horizontal_scale The horizontal scale of the texture.
		/// @param vertical_scale The vertical scale of the texture.
		/// @param flip_horizontally Whether to flip the texture horizontally.
		/// @param flip_horizontally Whether to flip the texture vertically.
		/// @param angle The number of degrees to rotate the texture, counter-clockwise.
		/// @param color An additional color multiplier, applied on top of the camera's and texture's color members.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( const sdl::Texture& texture
			, sdl::Point position
			, const boost::optional<sdl::Point>& origin = boost::none
			, double horizontal_scale = 1.0
			, double vertical_scale = 1.0
			, bool flip_horizontally = false
			, bool flip_vertically = false
			, double angle = 0.0
			, sdl::Color color = sdl::Color::white()
			, const boost::optional<sdl::Rect>& src_rect = boost::none
			) const;
	private:
		sdl::Window& _window;
		PointF _position;
		double _zoom;
		double _angle;
		sdl::Color _color;

		PointF _pt_hovered;
		sdl::Point _pt_hovered_rounded;
		RegionTileCoords _tile_hovered;
	};
}

#endif