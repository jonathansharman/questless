//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <stdexcept>

#include <glew.h>

#include <SDL.h>

#include "window.hpp"
#include "units/colors.hpp"
#include "units/window_space.hpp"
#include "units/view_space.hpp"

namespace sdl
{
	//! A simple wrapper around SDL_Renderer.
	class renderer
	{
	public:
		//! Constructs a renderer object with the given window and logical dimensions.
		//! @param window The window onto which the renderer will render images.
		//! @param width The logical width of the view port. Rendered images will be scaled as needed.
		//! @param height The logical height of the view port. Rendered image will be scaled as needed.
		renderer(window& the_window, int width, int height);

		renderer(const renderer*) = delete;
		renderer(renderer&&) = default;

		~renderer();

		renderer& operator =(renderer const&) = delete;
		renderer& operator =(renderer&&) & = default;

		//! The internal SDL_Renderer pointer.
		SDL_Renderer* sdl_ptr() { return _renderer; }

		//! The logical width of the render target.
		int width() const { return _w; }
	
		//! The logical height of the render target.
		int height() const { return _h; }

		//! Whether the renderer supports render targets.
		bool supports_render_targets() const { return (_info.flags & SDL_RENDERER_TARGETTEXTURE) != 0; }

		//! The current render target; nullptr if target is the screen buffer.
		SDL_Texture* target() const;

		//! Sets the render target to the given texture. If target is not null, all draw operations will affect that texture instead of the screen buffer. Use nullptr to set target to the screen buffer.
		//! @param target A texture to use as the render target or nullptr to render to the screen buffer.
		void target(SDL_Texture* target);

		//! Updates the screen with the rendering performed.
		void present() { SDL_RenderPresent(_renderer); }

		//! Clears the screen with the current drawing color.
		void clear(units::colors::color color);

		//! Draws lines connecting the given @p vertices, with color @p color.
		void draw_lines(std::vector<units::view_space::point> const& vertices, units::colors::color color);

		//! Draws lines connecting the given @p vertices, with color @p color.
		void draw_lines(std::vector<units::window_space::point> const& vertices, units::colors::color color);

		//! Draws a solid polygon.
		//! @param polygon The polygon to draw.
		//! @param color The color of the polygon.
		void draw_polygon(units::view_space::polygon const& polygon, units::colors::color color);

		//! Draws a solid polygon.
		//! @param polygon The polygon to draw.
		//! @param color The color of the polygon.
		void draw_polygon(units::window_space::polygon const& polygon, units::colors::color color);

		//! Draws a filled polygon with a border.
		//! @param polygon The polygon to draw.
		//! @param border_width The width of the polygon's border.
		//! @param border_color The color of the polygon's border.
		//! @param fill_color The color of the polygon's interior.
		//! @note This function assumes the width of the polygon is sufficiently greater than the border width to avoid overlapping the border with the polygon's edges. If the polygon is too narrow or contains too-sharp corners, overlap may occur.
		void draw_polygon
			( units::view_space::polygon const& polygon
			, units::view_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			);

		//! Draws a filled polygon with a border.
		//! @param polygon The polygon to draw.
		//! @param border_width The width of the polygon's border.
		//! @param border_color The color of the polygon's border.
		//! @param fill_color The color of the polygon's interior.
		//! @note This function assumes the width of the polygon is sufficiently greater than the border width to avoid overlapping the border with the polygon's edges. If the polygon is too narrow or contains too-sharp corners, overlap may occur.
		void draw_polygon
			( units::window_space::polygon const& polygon
			, units::window_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			);

		//! Draws a solid box.
		//! @param box The box to be drawn.
		//! @param color The color of the box.
		void draw_box(units::view_space::box const& box, units::colors::color color);

		//! Draws a solid box.
		//! @param box The box to be drawn.
		//! @param color The color of the box.
		void draw_box(units::window_space::box const& box, units::colors::color color);

		//! Draws a box with a border.
		//! @param box The box to be drawn.
		//! @param border_width The width of the box's border.
		//! @param border_color The color of the box's border.
		//! @param fill_color The color of the box's interior.
		void draw_box
			( units::view_space::box const& box
			, units::view_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			);

		//! Draws a box with a border.
		//! @param box The box to be drawn.
		//! @param border_width The width of the box's border.
		//! @param border_color The color of the box's border.
		//! @param fill_color The color of the box's interior.
		void draw_box
			( units::window_space::box const& box
			, units::window_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			);

		//! Draws a disc with a border using a polygonal approximation.
		//! @param boundary The bounding circle of the disc to be drawn.
		//! @param border_width The width of the disc's border.
		//! @param border_color The color of the disc's border.
		//! @param fill_color The color of the disc's interior.
		//! @param segments_per_radius The number of segments to use in the polygonal approximation, divided by the radius of the disc. Improves disc quality at the cost of time.
		void draw_disc
			( units::view_space::sphere const& boundary
			, units::view_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			, float segments_per_radius = 1.0f
			);

		//! Draws a disc with a border using a polygonal approximation.
		//! @param boundary The bounding circle of the disc to be drawn.
		//! @param border_width The width of the disc's border.
		//! @param border_color The color of the disc's border.
		//! @param fill_color The color of the disc's interior.
		//! @param segments_per_radius The number of segments to use in the polygonal approximation, divided by the radius of the disc. Improves disc quality at the cost of time.
		void draw_disc
			( units::window_space::sphere const& boundary
			, units::window_space::scalar border_width
			, units::colors::color border_color
			, units::colors::color fill_color
			, float segments_per_radius = 1.0f
			);
	private:
		SDL_Renderer* _renderer;
		int _w;
		int _h;
		SDL_RendererInfo _info;
		SDL_Texture* _target;

		GLuint _vbo;
		GLuint _ibo;

		void set_draw_color(units::colors::color color);
		void draw_triangle_strip(units::view_space::polygon const& polygon, units::colors::color color);
	};
}
