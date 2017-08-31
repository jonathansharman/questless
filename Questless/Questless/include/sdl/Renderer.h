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

namespace sdl
{
	//! @todo Where should Fill go? Inside Renderer?d

	//! The fill of a drawn shape, either a filled solid or an empty outline.
	enum class Fill : bool { solid, outline };

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

		//! Draws lines connecting the series of points contained in the vector.
		//! @param points A vector of screen points.
		//! @param color The color of the lines.
		void draw_lines(std::vector<units::ScreenSpace::Point> points, units::colors::Color color);

		//! Draws a box. Can be filled or just outlined.
		//! @param box The rectangle to be drawn.
		//! @param color The color of the rectangle.
		//! @param fill Whether to draw a solid box or an outline.
		void draw_box(units::ScreenSpace::Box const& box, units::colors::Color color, Fill fill);

		//! Draws a filled box with a border.
		//! @param box The rectangle to be drawn.
		//! @param border_color The color of the rectangle's border.
		//! @param fill_color The color of the rectangle's interior.
		void draw_box(units::ScreenSpace::Box const& box, units::colors::Color border_color, units::colors::Color fill_color);
	private:
		SDL_Renderer* _renderer;
		int _w;
		int _h;
		SDL_RendererInfo _info;
		SDL_Texture* _target;

		GLuint _vbo;
		GLuint _quad_ibo;

		void set_draw_color(units::colors::Color color);
	};
}
