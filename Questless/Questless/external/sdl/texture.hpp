//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "spaces/colors.hpp"
#include "spaces/texture.hpp"
#include "spaces/window.hpp"

#include "vecx/angle.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <variant>

namespace sdl {
	// Forward-declare shader type.
	class shader_program;
	// Forward-declare default shader accessor.
	shader_program& dflt_program();

	//! A simple wrapper around SDL_Texture.
	class texture {
	public:
		//! Constructs a targetable, blank texture with the specified dimensions. Useful for building textures at run time.
		//! @param size The size of this texture in screen space.
		//! @param color The initial background color for the texture. Defaults to clear black.
		texture(spaces::window::vector size = spaces::window::vector::zero(), spaces::colors::color color = spaces::colors::clear());

		//! Constructs a texture object from the specified image file.
		//! @param filename The name of the image file.
		texture(char const* filename);

		//! Constructs a texture object from the SDL surface @p surface.
		texture(SDL_Surface* surface);

		texture(texture const&) = delete;
		texture(texture&&);

		~texture();

		texture& operator =(texture const&) & = delete;
		texture& operator =(texture&&) &;

		//! The width of this texture.
		spaces::window::px width() const { return x(_size); }

		//! The height of this texture.
		spaces::window::px height() const { return y(_size); }

		//! The width and height of the texture, as a screen space vector.
		spaces::window::vector size() const { return _size; }

		//! Draws all or part of the texture to the screen (or current frame buffer).
		//! @param position The coordinates of the texture on the screen.
		//! @param horizontal_alignment The horizontal alignment of the texture.
		//! @param vertical_alignment The vertical alignment of the texture.
		//! @param color_factor A factor to be applied component-wise to the texture's color.
		//! @param src_rect The section of the texture to be drawn. If nullopt, the entire texture is used.
		//! @tparam Point The point type. Supported types are spaces::view::point and spaces::window::point.
		template <typename Point>
		void draw
			( Point position
			, spaces::texture::h_align horizontal_alignment = spaces::texture::align_left
			, spaces::texture::v_align vertical_alignment = spaces::texture::align_top
			, spaces::colors::color color_factor = spaces::colors::white()
			, std::optional<spaces::texture::box> const& src_rect = std::nullopt
			) const {
			switch (horizontal_alignment) {
				case units::texture_space::align_left:
					position.x() += width() / 2;
					break;
				case units::texture_space::align_center:
					break;
				case units::texture_space::align_right:
					position.x() -= width() / 2;
					break;
			}
			switch (vertical_alignment) {
				case units::texture_space::align_top:
					position.y() += height() / 2;
					break;
				case units::texture_space::align_middle:
					break;
				case units::texture_space::align_bottom:
					position.y() -= height() / 2;
					break;
			}
			draw_transformed
				( position
				, units::texture_space::vector::zero()
				, color_factor
				, spaces::view::vector{1.0f, 1.0f}
				, units::world_space::radians::zero()
				, src_rect
				);
		}

		//! Draws all or part the texture to the screen (or current frame buffer), using the provided transformations.
		//! @param position The coordinates of the texture on the screen in view or window space.
		//! @param origin The offset from the texture's center to its origin point.
		//! @param color_factor A factor to be applied component-wise to the texture's color.
		//! @param scale The scale of the texture: <horizontal scale, vertical scale>.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the the texture around the origin, in radians.
		//! @param src_rect The section of the texture to be drawn. If nullopt, the entire texture is used.
		void draw_transformed
			( std::variant<spaces::window::point, spaces::view::point> position
			, spaces::texture::vector origin = spaces::texture::vector::zero()
			, spaces::colors::color color_factor = spaces::colors::white()
			, spaces::view::vector scale = {spaces::view::length{1.0}, spaces::view::length{1.0}}
			, vecx::radians angle = vecx::radians{0.0}
			, std::optional<spaces::texture::box> const& src_rect = std::nullopt
			) const;

		//! Executes the given code with this texture as the render target so that draw operations will affect this texture instead of the screen buffer.
		//! @param f The function to be executed while the texture is the render target.
		//! @param shader_program The shader program used in rendering calls within the body of @p f.
		template <typename F>
		void as_target(F&& f, shader_program const& shader_program = dflt_program()) {
			GLuint fbo = as_target_prologue(shader_program);
			std::forward<F>(f)();
			as_target_epilogue(fbo, shader_program);
		}
	private:
		GLuint _texture;
		GLuint _vbo = NULL;
		GLuint _ibo = NULL;
		spaces::window::vector _size;

		//! Goes before the function call in @p as_target().
		//! @return The generated FBO.
		//! @param shader_program The shader program passed to @p as_target().
		GLuint as_target_prologue(shader_program const& shader_program = dflt_program());

		//! Goes after the function call in @p as_target().
		//! @param fbo The FBO generated by @p as_target_prologue.
		//! @param shader_program The shader program passed to @p as_target().
		void as_target_epilogue(GLuint fbo, shader_program const& shader_program = dflt_program());
	};
}
