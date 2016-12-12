/**
* @file    Font.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Font, a simple wrapper class around TTF_Font.
*/

#ifndef FONT_H
#define FONT_H

#include <string>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>

#include "basic-sdl-wrappers.h"
#include "Texture.h"

namespace sdl
{
	class Font
	{
	public:
		using ptr = std::unique_ptr<Font>;

		/// Constructs an unloaded sound object.
		/// @param filename The name of a TrueType font file.
		/// @param blend_mode The SDL blend mode to use for this font. Determines the way the alpha channel is used.
		/// @param size The point size of the font.
		Font(const std::string& filename, int size, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		Font(const Font& font) = delete;

		Font(Font&& font);

		/// @return A unique font pointer constructed with the given arguments.
		template <typename... Args>
		static ptr make(Args&&... args) { return std::make_unique<Font>(std::forward<Args>(args)...); }

		~Font();

		Font& operator =(Font font) &;

		friend void swap(Font& first, Font& second);

		/// @return The internal TTF_Font pointer.
		TTF_Font* const std_ptr() { return _font; }

		/// @return The pt. size of the font.
		int size() const { return _size; }

		/// Renders the provided string of text as a texture with a transparent background.
		/// @param text The text to be rendered.
		/// @param renderer The renderer object to create and render the font.
		/// @param text_color The text color.
		/// @note The text must be at least one character in length, or an exception is thrown.
		Texture render(const std::string& text, Renderer& renderer, Color text_color) const;
	
		/// Renders the provided string of text as a texture with a solid background.
		/// @param text The text to be rendered.
		/// @param renderer The renderer object to create and render the font.
		/// @param text_color The text color.
		/// @param background_color The background color.
		/// @note The text must be at least one character in length, or an exception is thrown.
		Texture render(const std::string& text, Renderer& renderer, Color text_color, Color background_color) const;

		/// Quickly renders the provided string of text as a texture with a transparent background.
		/// @param text The text to be rendered.
		/// @param renderer The renderer object to create and render the font.
		/// @param text_color The text color.
		/// @note The text must be at least one character in length, or an exception is thrown.
		/// @note Render quality is inferior to that produced by render, but the resulting texture is produced and drawn much faster.
		Texture quick_render(const std::string& text, Renderer& renderer, Color text_color) const;
	private:
		TTF_Font* _font;
		int _size;
		SDL_BlendMode _blend_mode;
	};
}

#endif
