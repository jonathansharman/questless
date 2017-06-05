//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"
#include "units/colors.h"
#include "units/GameSpace.h"
#include "units/ScreenSpace.h"
#include "units/TextureSpace.h"

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
		Texture(int width = 0, int height = 0);

		//! Constructs a texture object from the specified image file.
		//! @param filename The name of the image file.
		Texture(char const* filename);

		//! Constructs a texture object from the SDL surface @p surface.
		Texture(SDL_Surface* surface);

		Texture(Texture const&) = delete;
		Texture(Texture&&);

		~Texture();

		Texture& operator =(Texture const&) & = delete;
		Texture& operator =(Texture&&) &;

		//! The width of the texture.
		int width() const { return _width; }

		//! The height of the texture.
		int height() const { return _height; }

		//! The width and height of the texture, respectively.
		units::TextureSpace::Vector size() const { return units::TextureSpace::Vector{_width, _height}; }

		//! Copies all or part of the texture to the current render target.
		//! @param dst_rect The portion of the screen to which the texture should be copied.
		//! @param color_factor A factor to be applied component-wise to the texture's color.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( units::ScreenSpace::Box const& dst_rect
			, units::ColorSpace::Vector color_factor = units::colors::white_factor()
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			) const;
	
		//! Copies all or part of the texture to the current render target.
		//! @param position The coordinates of the texture on the screen.
		//! @param horizontal_alignment The horizontal alignment of the texture.
		//! @param vertical_alignment The vertical alignment of the texture.
		//! @param color_factor A factor to be applied component-wise to the texture's color.
		//! @param src_rect An optional portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw
			( units::ScreenSpace::Point position
			, HAlign horizontal_alignment = HAlign::left
			, VAlign vertical_alignment = VAlign::top
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			) const;

		//! Copies all or part the texture to the current render target, applying the provided transformations.
		//! @param position The coordinates of the texture on the screen.
		//! @param origin The origin point within the texture. If nullopt, the texture's center is used.
		//! @param color_factor An color factor to apply to each texel's color.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the the texture around the origin, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		void draw_transformed
			( units::ScreenSpace::Point position
			, std::optional<units::TextureSpace::Point> origin = std::nullopt
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, double horizontal_scale = 1.0
			, double vertical_scale = 1.0
			, units::GameSpace::Radians angle = units::GameSpace::Radians::zero() //! @todo This should use screen space radians.
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			) const;

		//! Executes the given code with this texture as the render target so that draw operations will affect this texture instead of the screen buffer.
		//! @param f The function to be executed while the texture is the render target.
		template <typename F>
		void as_target(F&& /*f*/)
		{
			//! @todo This.
			//_renderer.target(_texture);
			//std::forward<F>(f)();
			//_renderer.target(nullptr);
		}
	private:
		GLuint _texture;
		GLuint _vbo = 0;
		GLuint _ibo = 0;
		int _access;
		int _width;
		int _height;
	};
}
