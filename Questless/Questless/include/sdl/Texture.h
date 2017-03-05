/**
* @file    Texture.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Texture, a simple wrapper class around SDL_Texture.
*/

#pragma once

#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <functional>
#include <memory>

#include <boost/optional.hpp>

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

	class Texture
	{
	public:
		using ptr = std::unique_ptr<Texture>;

		/// Constructs a targetable, blank texture with the specified dimensions. Useful for building textures at run time.
		/// @param width The width of the texture.
		/// @param height The height of the texture.
		/// @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		Texture(int width = 0, int height = 0, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		/// Constructs a texture object from the specified image file.
		/// @param filename The name of the image file.
		/// @param targetable Whether the texture can be used as a render target.
		/// @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		Texture(std::string const& filename, bool targetable = false, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND);

		/// Constructs a texture object from an SDL texture.
		/// @param texture An SDL texture pointer.
		Texture(SDL_Texture* texture);

		/// Copy constructor disabled.
		Texture(Texture const& texture) = delete;

		/// Move constructor.
		Texture(Texture&& texture);

		~Texture();

		Texture& operator =(Texture texture) &;

		friend void swap(Texture& first, Texture& second);

		/// @return The internal SDL_Texture pointer.
		SDL_Texture* const sdl_ptr() { return _texture; }

		/// Sets the color multiplier of the texture.
		/// @param color The color value.
		void color(Color color) { _color = color; }

		/// @return The width of the texture.
		int width() const { return _w; }

		/// @return The height of the texture.
		int height() const { return _h; }

		/// @return A point containing the width and height of the texture, respectively.
		units::ScreenVector dimensions() const { return units::ScreenVector{_w, _h}; }

		/// Copies all or part of the texture to the current render target.
		/// @param dst_rect The portion of the screen to which the texture should be copied.
		/// @param src_rect The portion of the texture which should be copied.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw(units::ScreenRect const& dst_rect, boost::optional<units::TextureRect> const& src_rect = boost::none) const;
	
		/// Copies all or part of the texture to the current render target.
		/// @param position The coordinates of the texture on the screen.
		/// @param horizontal_alignment The horizontal alignment of the texture.
		/// @param vertical_alignment The vertical alignment of the texture.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( units::ScreenPoint position
			, HAlign horizontal_alignment = HAlign::left
			, VAlign vertical_alignment = VAlign::top
			, boost::optional<units::TextureRect> const& src_rect = boost::none
			) const;

		/// Copies all or part the texture to the current render target, applying the provided transformations.
		/// @param position The coordinates of the texture on the screen.
		/// @param origin The origin point within the texture. If nullopt, the texture's center is used.
		/// @param color An additional color multiplier, applied on top of the texture's color member.
		/// @param horizontal_scale The horizontal scale of the texture.
		/// @param vertical_scale The vertical scale of the texture.
		/// @param angle The counter-clockwise rotation of the the texture, in radians.
		/// @param flip_horizontally Whether to flip the texture horizontally.
		/// @param flip_horizontally Whether to flip the texture vertically.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw_transformed
			(units::ScreenPoint position
			, boost::optional<units::TexturePoint> origin = boost::none
			, Color color = Color::white()
			, double horizontal_scale = 1.0
			, double vertical_scale = 1.0
			, units::GameRadians angle = units::GameRadians{0.0} /// @todo This should probably use "ScreenRadians" or something.
			, bool flip_horizontally = false
			, bool flip_vertically = false
			, boost::optional<units::TextureRect> const& src_rect = boost::none
			) const;

		/// Executes the given code with this texture as the render target so that draw operations will affect this texture instead of the screen buffer.
		/// @param f The function to be executed while the texture is the render target.
		void as_target(std::function<void()> f);
	private:
		SDL_Texture* _texture;
		Renderer& _renderer;
		SDL_BlendMode _blend_mode;
		uint32_t _format;
		int _access;
		int _w;
		int _h;
		Color _color; /// @todo Is there any purpose to this field?
	};
}
