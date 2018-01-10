//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/font.hpp"

using namespace units;

namespace sdl
{
	namespace
	{
		SDL_Color sdl_color(colors::color color)
		{
			return SDL_Color
				{ static_cast<uint8_t>(255.0f * color.red())
				, static_cast<uint8_t>(255.0f * color.green())
				, static_cast<uint8_t>(255.0f * color.blue())
				, static_cast<uint8_t>(255.0f * color.alpha())
				};
		}
	}

	font::font(char const* filename, int size, SDL_BlendMode blend_mode)
		: _blend_mode(blend_mode), _size(size)
	{
		_font = TTF_OpenFont(filename, _size);
		if (_font == nullptr) {
			throw std::runtime_error(("Failed to load font \"" + std::string{filename} + "\".").c_str());
		}
	}

	font::font(font&& that)
		: _font(that._font)
		, _size(that._size)
		, _blend_mode(that._blend_mode)
	{
		that._font = nullptr;
	}

	font::~font()
	{
		if (_font != nullptr && TTF_WasInit()) {
			TTF_CloseFont(_font);
		}
	}

	font& font::operator =(font that) &
	{
		swap(*this, that);
		return *this;
	}

	void swap(font& first, font& second)
	{
		std::swap(first._font, second._font);
		std::swap(first._size, second._size);
		std::swap(first._blend_mode, second._blend_mode);
	}

	texture font::render(char const* text, colors::color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Blended(_font, text, sdl_color(text_color));
		if (surface == nullptr) {
			throw std::runtime_error{TTF_GetError()};
		}
		texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}

	texture font::render(char const* text, colors::color text_color, colors::color background_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Shaded(_font, text, sdl_color(text_color), sdl_color(background_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}

	texture font::fast_render(char const* text, colors::color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Solid(_font, text, sdl_color(text_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}
}
