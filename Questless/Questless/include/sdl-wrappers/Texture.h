/**
* @file    Texture.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Texture, a simple wrapper class around SDL_Texture.
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "utility/optional.h"
#include <functional>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>

#include "basic-sdl-wrappers.h"
#include "Renderer.h"

namespace sdl
{
	enum class HAlign { left, center, right };
	enum class VAlign { top, middle, bottom };

	class Texture
	{
	public:
		using ptr = std::unique_ptr<Texture>;

		/// Constructs a blank texture with the specified dimensions.
		/// @param renderer The renderer object to create and render the texture.
		/// @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		/// @param width The initial width of the blank texture.
		/// @param height The initial height of the blank texture.
		/// @param targetable Whether the texture can be used as a render target.
		Texture(Renderer& renderer, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND, int width = 0, int height = 0, bool targetable = false);

		/// Constructs a texture object from the specified image file.
		/// @param filename The name of the image file.
		/// @param renderer The renderer object to create and render the texture.
		/// @param blend_mode The SDL blend mode to use for this texture. Determines the way the alpha channel is used.
		/// @param width The initial width of the blank texture.
		/// @param height The initial height of the blank texture.
		/// @param targetable Whether the texture can be used as a render target.
		Texture(const std::string& filename, Renderer& renderer, SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND, bool targetable = false);

		/// Constructs a texture object from an SDL texture.
		/// @param texture An SDL texture pointer.
		Texture(SDL_Texture* texture, Renderer& renderer);

		/// Copy constructor disabled.
		Texture(const Texture& texture) = delete;

		/// Move constructor.
		Texture(Texture&& texture);

		/// A convenience function for making a unique texture pointer.
		template <typename... Args>
		static ptr make(Args&&... args) { return std::make_unique<Texture>(std::forward<Args>(args)...); }

		/// @todo Replace all "make" functions with perfect-forwarding versions like this one.

		~Texture();

		Texture& operator =(Texture texture);

		friend void swap(Texture& first, Texture& second);

		/// @return The internal SDL_Texture pointer.
		SDL_Texture* const sdl_ptr() { return _texture; }

		/// Sets the alpha multiplier of the texture.
		/// @param alpha The alpha value from 0 to 255.
		void alpha(uint8_t alpha);

		/// Sets the color multiplier of the texture.
		/// @param color The color value.
		void color(Color color) { _color = color; }

		/// @return The width of the texture.
		int width() const { return _w; }

		/// @return The height of the texture.
		int height() const { return _h; }

		/// @return A point containing the width and height of the texture, respectively.
		Point dimensions() const { return Point(_w, _h); }

		/// Copies all or part of the texture to the current render target.
		/// @param dst_rect The portion of the screen to which the texture should be copied.
		/// @param src_rect The portion of the texture which should be copied.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw(const Rect& dst_rect, const optional<Rect>& src_rect = nullopt) const;
	
		/// Copies all or part of the texture to the current render target.
		/// @param position The coordinates of the texture on the screen.
		/// @param horizontal_alignment The horizontal alignment of the texture.
		/// @param vertical_alignment The vertical alignment of the texture.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( Point position
			, HAlign horizontal_alignment = HAlign::left
			, VAlign vertical_alignment = VAlign::top
			, const optional<Rect>& src_rect = nullopt
			) const;

		/// Copies all or part the texture to the current render target, applying the provided transformations.
		/// @param position The coordinates of the texture on the screen.
		/// @param origin The origin point within the texture. If nullopt, the texture's center is used.
		/// @param horizontal_scale The horizontal scale of the texture.
		/// @param vertical_scale The vertical scale of the texture.
		/// @param flip_horizontally Whether to flip the texture horizontally.
		/// @param flip_horizontally Whether to flip the texture vertically.
		/// @param color An additional color multiplier, applied on top of the texture's color member.
		/// @param angle The number of degrees to rotate the texture, counter-clockwise.
		/// @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw_transformed
			( Point position
			, const optional<Point>& origin = nullopt
			, double horizontal_scale = 1.0
			, double vertical_scale = 1.0
			, bool flip_horizontally = false
			, bool flip_vertically = false
			, Color color = Color::white()
			, double angle = 0
			, const optional<Rect>& src_rect = nullopt
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
		Color _color;
	};
}

#endif
