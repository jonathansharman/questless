/**
* @file    Font.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for Font, a simple wrapper class around TTF_Font.
*/

#include "sdl/Font.h"
#include "sdl/Renderable.h" /// @todo Remove after done debugging.

namespace sdl
{
	Font::Font(std::string const& filename, int size, SDL_BlendMode blend_mode)
		: _blend_mode(blend_mode), _size(size)
	{
		_font = TTF_OpenFont(filename.c_str(), _size);
		if (_font == nullptr) {
			throw std::runtime_error(("Failed to load font \"" + filename + "\".").c_str());
		}
	}

	Font::Font(Font&& font)
		: _font(font._font)
		, _size(font._size)
		, _blend_mode(font._blend_mode)
	{
		font._font = nullptr;
	}

	Font::~Font()
	{
		if (_font != nullptr && TTF_WasInit()) {
			TTF_CloseFont(_font);
		}
	}

	Font& Font::operator =(Font font) &
	{
		swap(*this, font);
		return *this;
	}

	void swap(Font& first, Font& second)
	{
		std::swap(first._font, second._font);
		std::swap(first._size, second._size);
		std::swap(first._blend_mode, second._blend_mode);
	}

	Texture Font::render(std::string const& text, Color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Blended(_font, text.c_str(), static_cast<SDL_Color>(text_color));
		if (surface == nullptr) {
			throw std::runtime_error{TTF_GetError()};
		}
		SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(renderer().sdl_ptr(), surface);
		if (sdl_texture == nullptr) {
			throw std::runtime_error{"SDL texture created from surface is null."};
		}
		Texture texture{sdl_texture};
		SDL_FreeSurface(surface);
		return texture;
	}

	Texture Font::render(std::string const& text, Color text_color, Color background_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Shaded(_font, text.c_str(), static_cast<SDL_Color>(text_color), static_cast<SDL_Color>(background_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(renderer().sdl_ptr(), surface);
		if (sdl_texture == nullptr) {
			throw std::runtime_error{"SDL texture created from surface is null."};
		}
		Texture texture{sdl_texture};
		SDL_FreeSurface(surface);
		return texture;
	}

	Texture Font::fast_render(std::string const& text, Color text_color) const
	{
		SDL_Surface* surface = TTF_RenderText_Solid(_font, text.c_str(), static_cast<SDL_Color>(text_color));
		if (surface == nullptr) {
			throw std::runtime_error(TTF_GetError());
		}
		Texture texture{SDL_CreateTextureFromSurface(renderer().sdl_ptr(), surface)};
		SDL_FreeSurface(surface);
		return texture;
	}
}
