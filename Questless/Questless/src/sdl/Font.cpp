//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Font.h"
#include "sdl/Renderable.h" //! @todo Remove after done debugging.

using namespace units;

namespace sdl
{
	namespace
	{
		SDL_Color sdl_color(colors::Color color)
		{
			return SDL_Color
				{ static_cast<uint8_t>(255.0f * color.red())
				, static_cast<uint8_t>(255.0f * color.green())
				, static_cast<uint8_t>(255.0f * color.blue())
				, static_cast<uint8_t>(255.0f * color.alpha())
				};
		}
	}

	Font::Font(char const* filename, int size, SDL_BlendMode blend_mode)
		: _blend_mode(blend_mode), _size(size)
	{
		_font = TTF_OpenFont(filename, _size);
		if (_font == nullptr) {
			throw std::runtime_error(("Failed to load font \"" + std::string{filename} + "\".").c_str());
		}
	}

	Font::Font(Font&& that)
		: _font(that._font)
		, _size(that._size)
		, _blend_mode(that._blend_mode)
	{
		that._font = nullptr;
	}

	Font::~Font()
	{
		if (_font != nullptr && TTF_WasInit()) {
			TTF_CloseFont(_font);
		}
	}

	Font& Font::operator =(Font that) &
	{
		swap(*this, that);
		return *this;
	}

	void swap(Font& first, Font& second)
	{
		std::swap(first._font, second._font);
		std::swap(first._size, second._size);
		std::swap(first._blend_mode, second._blend_mode);
	}

	Texture Font::render(char const* text, colors::Color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Blended(_font, text, sdl_color(text_color));
		if (surface == nullptr) {
			throw std::runtime_error{TTF_GetError()};
		}
		Texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}

	Texture Font::render(char const* text, colors::Color text_color, colors::Color background_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Shaded(_font, text, sdl_color(text_color), sdl_color(background_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		Texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}

	Texture Font::fast_render(char const* text, colors::Color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Solid(_font, text, sdl_color(text_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		Texture texture{surface};
		SDL_FreeSurface(surface);
		return texture;
	}
}
