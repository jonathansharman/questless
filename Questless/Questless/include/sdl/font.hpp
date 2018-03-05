//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <algorithm>

#include <SDL.h>
#include <SDL_ttf.h>

#include "texture.hpp"
#include "units/color_space.hpp"

namespace sdl
{
	//! A simple wrapper around TTF_Font.
	class font
	{
	public:
		//! Constructs an unloaded sound object.
		//! @param filename The name of a TrueType font file.
		//! @param blend_mode The SDL blend mode to use for this font. Determines the way the alpha channel is used.
		//! @param size The point size of the font.
		font(char const* filename, int size, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		font(font const&) = delete;

		font(font&& that);

		~font();

		font& operator =(font that) &;

		friend void swap(font& first, font& second);

		//! The internal TTF_Font pointer.
		TTF_Font* std_ptr() { return _font; }

		//! The pt. size of the font.
		int size() const { return _size; }

		//! Renders the provided string of text as a texture with a transparent background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		texture render(char const* text, units::color_space::point text_color) const;
	
		//! Renders the provided string of text as a texture with a solid background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		//! @param background_color The background color.
		texture render(char const* text, units::colors::color text_color, units::colors::color background_color) const;

		//! Quickly renders the provided string of text as a texture with a transparent background.
		//! @param text The text to be rendered. Must be at least one character in length.
		//! @param text_color The text color.
		//! @note Render quality is inferior to that produced by render, but the resulting texture is produced and drawn much faster.
		texture fast_render(char const* text, units::colors::color text_color) const;
	private:
		TTF_Font* _font;
		int _size;
		SDL_BlendMode _blend_mode;
	};
}
