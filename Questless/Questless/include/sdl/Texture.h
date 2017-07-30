//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <optional>

#include <SDL.h>
#include <SDL_image.h>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "units/colors.h"
#include "units/GameSpace.h"
#include "units/ScreenSpace.h"
#include "units/TextureSpace.h"

namespace sdl
{
	// Forward-declare default shader accessor.
	ShaderProgram& dflt_program();

	enum class HAlign { left, center, right };
	enum class VAlign { top, middle, bottom };

	//! A simple wrapper around SDL_Texture.
	class Texture
	{
	public:
		//! Constructs a targetable, blank texture with the specified dimensions. Useful for building textures at run time.
		//! @param width The width of the texture.
		//! @param height The height of the texture.
		//! @param color The initial background color for the texture. Defaults to clear black.
		Texture(int width = 0, int height = 0, units::colors::Color color = units::colors::clear());

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
		//! @param shader_program The shader program with which to draw the texture.
		void draw
			( units::ScreenSpace::Box const& dst_rect
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			, ShaderProgram const& shader_program = dflt_program()
			) const;
	
		//! Copies all or part of the texture to the current render target.
		//! @param position The coordinates of the texture on the screen.
		//! @param horizontal_alignment The horizontal alignment of the texture.
		//! @param vertical_alignment The vertical alignment of the texture.
		//! @param color_factor A factor to be applied component-wise to the texture's color.
		//! @param src_rect An optional portion of the texture to be copied. If nullopt, the entire texture is used.
		//! @param shader_program The shader program with which to draw the texture.
		void draw
			( units::ScreenSpace::Point position
			, HAlign horizontal_alignment = HAlign::left
			, VAlign vertical_alignment = VAlign::top
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			, ShaderProgram const& shader_program = dflt_program()
			) const;

		//! Copies all or part the texture to the current render target, applying the provided transformations.
		//! @param position The coordinates of the texture on the screen.
		//! @param origin The offset from the texture's center to its origin point.
		//! @param color_factor An color factor to apply to each texel's color.
		//! @param horizontal_scale The horizontal scale of the texture.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the the texture around the origin, in radians.
		//! @param src_rect An optional Rect specifying the portion of the texture to be copied. If nullopt, the entire texture is used.
		//! @param shader_program The shader program with which to draw the texture.
		void draw_transformed
			( units::ScreenSpace::Point position
			, units::TextureSpace::Vector origin = units::TextureSpace::Vector::zero()
			, units::colors::ColorFactor color_factor = units::colors::white_factor()
			, float horizontal_scale = 1.0f
			, float vertical_scale = 1.0f
			, units::GameSpace::Radians angle = units::GameSpace::Radians::zero() //! @todo This should use screen space radians.
			, std::optional<units::TextureSpace::Box> const& src_rect = std::nullopt
			, ShaderProgram const& shader_program = dflt_program()
			) const;

		//! Executes the given code with this texture as the render target so that draw operations will affect this texture instead of the screen buffer.
		//! @param f The function to be executed while the texture is the render target.
		//! @param shader_program The shader program used in rendering calls within the body of @p f.
		template <typename F>
		void as_target(F&& f, ShaderProgram const& shader_program = dflt_program())
		{
			GLuint fbo = as_target_prologue(shader_program);
			std::forward<F>(f)();
			as_target_epilogue(fbo, shader_program);
		}
	private:
		GLuint _texture;
		GLuint _vbo = NULL;
		GLuint _ibo = NULL;
		int _width;
		int _height;

		//! Goes before the function call in @p as_target().
		//! @return The generated FBO.
		//! @param shader_program The shader program passed to @p as_target().
		GLuint as_target_prologue(ShaderProgram const& shader_program = dflt_program());

		//! Goes after the function call in @p as_target().
		//! @param fbo The FBO generated by @p as_target_prologue.
		//! @param shader_program The shader program passed to @p as_target().
		void as_target_epilogue(GLuint fbo, ShaderProgram const& shader_program = dflt_program());
	};
}
