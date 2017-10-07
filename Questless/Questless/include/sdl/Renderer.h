//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <stdexcept>

#include <glew.h>

#include <SDL.h>

#include "Window.h"
#include "units/colors.h"
#include "units/ScreenSpace.h"
#include "units/ViewSpace.h"

namespace sdl
{
	//! A simple wrapper around SDL_Renderer.
	class Renderer
	{
	public:
		//! Constructs a renderer object with the given window and logical dimensions.
		//! @param window The window onto which the renderer will render images.
		//! @param width The logical width of the view port. Rendered images will be scaled as needed.
		//! @param height The logical height of the view port. Rendered image will be scaled as needed.
		Renderer(Window& window, int width, int height);

		Renderer(const Renderer*) = delete;
		Renderer(Renderer&&) = default;

		~Renderer();

		Renderer& operator =(Renderer const&) = delete;
		Renderer& operator =(Renderer&&) & = default;

		//! The internal SDL_Renderer pointer.
		SDL_Renderer* const sdl_ptr() { return _renderer; }

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
		void clear(units::colors::Color color);

		//! Draws lines connecting the given @p vertices, with color @p color.
		void draw_lines(std::vector<units::ViewSpace::Point> const& vertices, units::colors::Color color);

		//! Draws lines connecting the given @p vertices, with color @p color.
		void draw_lines(std::vector<units::ScreenSpace::Point> const& vertices, units::colors::Color color);

		//! Draws a solid polygon.
		//! @param vertices The vertices of the polygon.
		//! @param color The color of the polygon.
		void draw_polygon(std::vector<units::ViewSpace::Point> const& vertices, units::colors::Color color);

		//! Draws a solid polygon.
		//! @param vertices The vertices of the polygon.
		//! @param color The color of the polygon.
		void draw_polygon(std::vector<units::ScreenSpace::Point> const& vertices, units::colors::Color color);

		//! Draws a filled polygon with a border.
		//! @param vertices The vertices of the polygon.
		//! @param border_width The width of the polygon's border.
		//! @param border_color The color of the polygon's border.
		//! @param fill_color The color of the polygon's interior.
		//! @note This function assumes the width of the polygon is sufficiently greater than the border width to avoid overlapping the border with the polygon's edges. If the polygon is too narrow or contains too-sharp corners, overlap may occur.
		void draw_polygon
			( std::vector<units::ViewSpace::Point> const& vertices
			, units::ViewSpace::scalar_t border_width
			, units::colors::Color border_color
			, units::colors::Color fill_color
			);

		//! Draws a filled polygon with a border.
		//! @param vertices The vertices of the polygon.
		//! @param border_width The width of the polygon's border.
		//! @param border_color The color of the polygon's border.
		//! @param fill_color The color of the polygon's interior.
		//! @note This function assumes the width of the polygon is sufficiently greater than the border width to avoid overlapping the border with the polygon's edges. If the polygon is too narrow or contains too-sharp corners, overlap may occur.
		void draw_polygon
			( std::vector<units::ScreenSpace::Point> const& vertices
			, units::ScreenSpace::scalar_t border_width
			, units::colors::Color border_color
			, units::colors::Color fill_color
			);

		//! Draws a solid box.
		//! @param box The box to be drawn.
		//! @param color The color of the box.
		void draw_box(units::ViewSpace::Box const& box, units::colors::Color color);

		//! Draws a solid box.
		//! @param box The box to be drawn.
		//! @param color The color of the box.
		void draw_box(units::ScreenSpace::Box const& box, units::colors::Color color);

		//! Draws a box with a border.
		//! @param box The box to be drawn.
		//! @param border_width The width of the box's border.
		//! @param border_color The color of the box's border.
		//! @param fill_color The color of the box's interior.
		void draw_box
			( units::ViewSpace::Box const& box
			, units::ViewSpace::scalar_t border_width
			, units::colors::Color border_color
			, units::colors::Color fill_color
			);

		//! Draws a box with a border.
		//! @param box The box to be drawn.
		//! @param border_width The width of the box's border.
		//! @param border_color The color of the box's border.
		//! @param fill_color The color of the box's interior.
		void draw_box
			( units::ScreenSpace::Box const& box
			, units::ScreenSpace::scalar_t border_width
			, units::colors::Color border_color
			, units::colors::Color fill_color
			);
	private:
		SDL_Renderer* _renderer;
		int _w;
		int _h;
		SDL_RendererInfo _info;
		SDL_Texture* _target;

		GLuint _vbo;
		GLuint _ibo;

		void set_draw_color(units::colors::Color color);
		void draw_triangle_strip(std::vector<units::ViewSpace::Point> vertices, units::colors::Color color);
	};
}
