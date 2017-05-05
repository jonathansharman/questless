//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <stdexcept>
#include <memory>
#include <algorithm>
#include <functional>
#include <memory>

#include <optional>

#include <SDL.h>
#include <SDL_image.h>

#include "units/ScreenVector.h"
#include "units/ScreenPoint.h"
#include "units/ScreenRect.h"
#include "units/TexturePoint.h"
#include "units/TextureRect.h"
#include "units/GameRadians.h"

#include "Renderer.h"
#include "Color.h"

namespace sdl
{
	enum class HAlign { left, center, right };
	enum class VAlign { top, middle, bottom };

	//! A simple wrapper around SDL_Texture.
	class Texture
	{
	public:
		//! Constructs a targetable, blank texture with the specified dimensions. Useful for building textures at run time.
		//! @param width The width of the texture.
		//! @param height The height of the texture.
		//! @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		Texture(int width = 0, int height = 0, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		//! Constructs a texture object from the specified image file.
		//! @param filename The name of the image file.
		//! @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		Texture(char const* filename, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		//! Constructs a texture object from an SDL texture.
		//! @param texture An SDL texture pointer.
		Texture(SDL_Texture* texture);
		Texture(Texture const&) = delete;
		Texture(Texture&& that);

		~Texture();

		Texture& operator =(Texture that) &;

		friend void swap(Texture& first, Texture& second);

		//! The internal SDL_Texture pointer.
		SDL_Texture* const sdl_ptr() { return _texture; }

		//! Sets the color multiplier of the texture.
		//! @param color The color value.
		void color(Color color) { _color = color; }

		//! The width of the texture.
		int width() const { return _w; }

		//! The height of the texture.
		int height() const { return _h; }

		//! A point containing the width and height of the texture, respectively.
		units::ScreenVector dimensions() const { return units::ScreenVector{_w, _h}; }

		//! Copies all or part of the texture to the current render target.
		//! @param dst_rect The portion of the screen to which the texture should be copied.
		//! @param src_rect The portion of the texture which should be copied.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw(units::ScreenRect const& dst_rect, std::optional<units::TextureRect> const& src_rect = std::nullopt) const;
	
		//! Copies all or part of the texture to the current render target.
		//! @param position The coordinates of the texture on the screen.
		//! @param horizontal_alignment The horizontal alignment of the texture.
		//! @param vertical_alignment The vertical alignment of the texture.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( units::ScreenPoint position
			, HAlign horizontal_alignment = HAlign::left
			, VAlign vertical_alignment = VAlign::top
			, std::optional<units::TextureRect> const& src_rect = std::nullopt
			) const;

		//! Copies all or part the texture to the current render target, applying the provided transformations.
		//! @param position The coordinates of the texture on the screen.
		//! @param origin The origin point within the texture. If nullopt, the texture's center is used.
		//! @param color An additional color multiplier, applied on top of the texture's color member.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the the texture, in radians.
		//! @param flip_horizontally Whether to flip the texture horizontally.
		//! @param flip_horizontally Whether to flip the texture vertically.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw_transformed
			(units::ScreenPoint position
			, std::optional<units::TexturePoint> origin = std::nullopt
			, Color color = Color::white()
			, double horizontal_scale = 1.0
			, double vertical_scale = 1.0
			, units::GameRadians angle = units::GameRadians{0.0} //! @todo This should probably use "ScreenRadians" or something.
			, bool flip_horizontally = false
			, bool flip_vertically = false
			, std::optional<units::TextureRect> const& src_rect = std::nullopt
			) const;

		//! Executes the given code with this texture as the render target so that draw operations will affect this texture instead of the screen buffer.
		//! @param f The function to be executed while the texture is the render target.
		template <typename F>
		void as_target(F&& f)
		{
			_renderer.target(_texture);
			std::forward<F>(f)();
			_renderer.target(nullptr);
		}
	private:
		SDL_Texture* _texture;
		Renderer& _renderer;
		SDL_BlendMode _blend_mode;
		uint32_t _format;
		int _access;
		int _w;
		int _h;
		Color _color; //! @todo Is there any purpose to this field?
	};
}
