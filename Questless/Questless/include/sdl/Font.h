//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Texture.h"

namespace sdl
{
	//! A simple wrapper around TTF_Font.
	class Font
	{
	public:
		using uptr = std::unique_ptr<Font>;

		//! Constructs an unloaded sound object.
		//! @param filename The name of a TrueType font file.
		//! @param blend_mode The SDL blend mode to use for this font. Determines the way the alpha channel is used.
		//! @param size The point size of the font.
		Font(char const* filename, int size, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		Font(Font const&) = delete;

		Font(Font&& that);

		~Font();

		Font& operator =(Font that) &;

		friend void swap(Font& first, Font& second);

		//! The internal TTF_Font pointer.
		TTF_Font* const std_ptr() { return _font; }

		//! The pt. size of the font.
		int size() const { return _size; }

		//! Renders the provided string of text as a texture with a transparent background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		Texture render(char const* text, Color text_color) const;
	
		//! Renders the provided string of text as a texture with a solid background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		//! @param background_color The background color.
		Texture render(char const* text, Color text_color, Color background_color) const;

		//! Quickly renders the provided string of text as a texture with a transparent background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		//! @note Render quality is inferior to that produced by render, but the resulting texture is produced and drawn much faster.
		Texture fast_render(char const* text, Color text_color) const;
	private:
		TTF_Font* _font;
		int _size;
		SDL_BlendMode _blend_mode;
	};
}
