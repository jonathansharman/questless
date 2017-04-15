/**
* @file    Texture.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for Texture, a simple wrapper class around SDL_Texture.
*/

#include "sdl/Texture.h"

#include <string>

#include "sdl/resources.h"

using namespace units;

namespace sdl
{
	Texture::Texture(int width, int height, SDL_BlendMode blend_mode)
		: _texture{nullptr}
		, _renderer{renderer()}
		, _blend_mode{blend_mode}
		, _format{SDL_PIXELFORMAT_UNKNOWN}
		, _access{SDL_TEXTUREACCESS_TARGET}
		, _w{width}
		, _h{height}
		, _color{255, 255, 255}
	{
		if (_w != 0 && _h != 0) {
			_texture = SDL_CreateTexture(_renderer.sdl_ptr(), _format, _access, _w, _h);
			if (_texture == nullptr) {
				throw std::runtime_error("Failed to create blank texture.");
			}
			if (SDL_SetTextureBlendMode(_texture, _blend_mode)) {
				throw std::runtime_error("Failed to set texture blend mode.");
			}
		} else {
			throw std::runtime_error("Attempted to create texture with zero dimensions.");
		}
	}

	Texture::Texture(char const* filename, bool targetable, SDL_BlendMode blend_mode)
		: _texture{nullptr }
		, _renderer{renderer()}
		, _blend_mode{blend_mode}
		, _format{SDL_PIXELFORMAT_UNKNOWN}
		, _access{targetable ? SDL_TEXTUREACCESS_TARGET : SDL_TEXTUREACCESS_STATIC}
		, _color{255, 255, 255}
	{
		_texture = IMG_LoadTexture(_renderer.sdl_ptr(), filename);
		if (!_texture) {
			throw std::runtime_error(("Failed to load texture \"" + std::string{filename} +"\".").c_str());
		}
		SDL_QueryTexture(_texture, nullptr, nullptr, &_w, &_h);
		if (SDL_SetTextureBlendMode(_texture, _blend_mode)) {
			throw std::runtime_error("Failed to set texture blend mode.");
		}
	}
	
	Texture::Texture(SDL_Texture* texture)
		: _texture{texture}
		, _renderer{renderer()}
	{
		if (_texture) {
			SDL_GetTextureBlendMode(_texture, &_blend_mode);
			SDL_GetTextureColorMod(_texture, &_color.r, &_color.g, &_color.b);
			SDL_GetTextureAlphaMod(_texture, &_color.a);
			SDL_QueryTexture(_texture, &_format, &_access, &_w, &_h);
		}
	}

	Texture::Texture(Texture&& that) : _renderer{that._renderer}
	{
		swap(*this, that);
		that._texture = nullptr;
	}

	Texture::~Texture()
	{
		SDL_DestroyTexture(_texture);
	}

	Texture& Texture::operator =(Texture that) &
	{
		swap(*this, that);
		return *this;
	}

	void swap(Texture& first, Texture& second)
	{
		std::swap(first._texture, second._texture);
		std::swap(first._blend_mode, second._blend_mode);
		std::swap(first._format, second._format);
		std::swap(first._access, second._access);
		std::swap(first._w, second._w);
		std::swap(first._h, second._h);
		std::swap(first._color, second._color);
	}

	void Texture::draw(ScreenRect const& dst_rect, std::optional<TextureRect> const& src_rect) const
	{
		// This reinterpret_cast is safe because SDL_Rect and TextureRect have the same data structure.
		SDL_Rect const* sdl_dst_rect = reinterpret_cast<const SDL_Rect*>(&dst_rect);

		// This reinterpret_cast is safe because SDL_Rect and ScreenRect have the same data structure.
		SDL_Rect const* sdl_src_rect = src_rect ? reinterpret_cast<const SDL_Rect*>(&src_rect.value()) : nullptr;

		SDL_SetTextureColorMod(_texture, _color.r, _color.g, _color.b);
		SDL_SetTextureAlphaMod(_texture, _color.a);
		SDL_RenderCopy(_renderer.sdl_ptr(), _texture, sdl_src_rect, sdl_dst_rect);
	}

	void Texture::draw(ScreenPoint position, HAlign horizontal_alignment, VAlign vertical_alignment, std::optional<TextureRect> const& src_rect) const
	{
		switch (horizontal_alignment) {
			case HAlign::left:
				break;
			case HAlign::center:
				position.x -= (_w - 1) / 2;
				break;
			case HAlign::right:
				position.x -= _w - 1;
				break;
		}
		switch (vertical_alignment) {
			case VAlign::top:
				break;
			case VAlign::middle:
				position.y -= (_h - 1) / 2;
				break;
			case VAlign::bottom:
				position.y -= _h - 1;
				break;
		}
		SDL_Rect const sdl_dst_rect{position.x, position.y, _w, _h};
		
		// This reinterpret_cast is safe because SDL_Rect and ScreenRect have the same data structure.
		SDL_Rect const* sdl_src_rect = src_rect ? reinterpret_cast<const SDL_Rect*>(&src_rect.value()) : nullptr;

		SDL_SetTextureColorMod(_texture, _color.r, _color.g, _color.b);
		SDL_SetTextureAlphaMod(_texture, _color.a);
		SDL_RenderCopy(_renderer.sdl_ptr(), _texture, sdl_src_rect, &sdl_dst_rect);
	}

	void Texture::draw_transformed
		( ScreenPoint position
		, std::optional<TexturePoint> origin
		, Color color
		, double horizontal_scale
		, double vertical_scale
		, units::GameRadians angle
		, bool flip_horizontally
		, bool flip_vertically
		, std::optional<TextureRect> const& src_rect
		) const
	{
		if (origin) {
			position.x += _w / 2 - origin->x;
			position.y += _h / 2 - origin->y;
		}
		int w = static_cast<int>(horizontal_scale * (src_rect ? src_rect->w : _w));
		int h = static_cast<int>(vertical_scale * (src_rect ? src_rect->h : _h));
		SDL_Rect sdl_dst_rect = { static_cast<int>(position.x - w / 2), static_cast<int>(position.y - h / 2), w, h };

		// This reinterpret_cast is safe because SDL_Rect and ScreenRect have the same data structure.
		SDL_Rect const* sdl_src_rect = src_rect ? reinterpret_cast<const SDL_Rect*>(&src_rect.value()) : nullptr;

		SDL_SetTextureColorMod
			( _texture
			, static_cast<uint8_t>((static_cast<uint32_t>(color.r) * _color.r) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.g) * _color.g) / 255)
			, static_cast<uint8_t>((static_cast<uint32_t>(color.b) * _color.b) / 255)
			);
		SDL_SetTextureAlphaMod(_texture, static_cast<uint8_t>((static_cast<uint32_t>(color.a) * _color.a) / 255));

		constexpr double radians_to_negative_degrees = -360.0 / 6.283185307179586476925;
		SDL_RenderCopyEx
			( _renderer.sdl_ptr()
			, _texture
			, sdl_src_rect
			, &sdl_dst_rect
			, radians_to_negative_degrees * angle.count()
			, nullptr
			, static_cast<SDL_RendererFlip>((flip_horizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flip_vertically ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
			);
	}

	void Texture::as_target(std::function<void()> f)
	{
		_renderer.target(_texture);
		f();
		_renderer.target(nullptr);
	}
}
