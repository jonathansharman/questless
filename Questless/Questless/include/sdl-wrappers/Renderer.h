/**
* @file    Renderer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Renderer, a simple wrapper class around SDL_Renderer.
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <stdexcept>

#include <SDL.h>

#include "basic-sdl-wrappers.h"
#include "Window.h"

namespace sdl
{
	class Renderer
	{
	public:
		/// Constructs a renderer object from the given window and dimensions.
		/// @param window The window onto which the renderer will render images.
		/// @param width The logical width of the window. Rendered images will be scaled as needed.
		/// @param height The logical height of the window. Rendered image will be scaled as needed.
		Renderer(Window& window, int width, int height);

		Renderer(const Renderer* other) = delete;
		Renderer(Renderer&& other) = default;

		~Renderer();

		Renderer& operator =(const Renderer& other) = delete;
		Renderer& operator =(Renderer&& other) & = default;

		/// @return The internal SDL_Renderer pointer.
		SDL_Renderer* const sdl_ptr() { return _renderer; }

		/// @return The logical width of the render target.
		int width() const { return _w; }
	
		/// @return The logical height of the render target.
		int height() const { return _h; }

		/// @return Whether the renderer supports render targets.
		bool supports_render_targets() const { return (_info.flags & SDL_RENDERER_TARGETTEXTURE) != 0; }

		/// @return The current render target; nullptr if target is the screen buffer.
		SDL_Texture* target() const;

		/// Sets the render target to the given texture. If target is not null, all draw operations will affect that texture instead of the screen buffer. Use nullptr to set target to the screen buffer.
		/// @param target A texture to use as the render target or nullptr to render to the screen buffer.
		void target(SDL_Texture* target);

		/// Updates the screen with the rendering performed.
		void present() { SDL_RenderPresent(_renderer); }

		/// Clears the screen with the current drawing color.
		void clear(Color color);

		/// Draws lines connecting the series of points contained in the vector.
		/// @param points A vector of SDL points.
		/// @param color The color of the lines.
		void draw_lines(std::vector<Point> points, Color color);

		/// Draws a rectangle. Can be filled or just an outline.
		/// @param rect The rectangle to be drawn.
		/// @param color The color of the rectangle.
		/// @param filled If true, a filled rectangle is drawn. Otherwise, only its outline is drawn.
		void draw_rect(Rect rect, Color color, bool filled);

		/// Draws a filled rectangle with a border.
		/// @param rect The rectangle to be drawn.
		/// @param border_color The color of the rectangle's border.
		/// @param fill_color The color of the rectangle's interior.
		void draw_rect(Rect rect, Color border_color, Color fill_color);
	private:
		SDL_Renderer* _renderer;
		int _w;
		int _h;
		SDL_RendererInfo _info;
		SDL_Texture* _target;

		void set_draw_color(Color color);
	};
}

#endif
