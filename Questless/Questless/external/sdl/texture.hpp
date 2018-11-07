//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "spaces/colors.hpp"
#include "spaces/world.hpp"
#include "spaces/texture.hpp"
#include "spaces/window.hpp"

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
		texture(units::window_space::vector size = units::window_space::vector::zero(), units::colors::color color = units::colors::clear());

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
		units::px width() const { return _size.x(); }

		//! The height of this texture.
		units::px height() const { return _size.y(); }

		//! The width and height of the texture, as a screen space vector.
		units::window_space::vector size() const { return _size; }

		//! Draws all or part of the texture to the screen (or current frame buffer).
		//! @param position The coordinates of the texture on the screen.
		//! @param horizontal_alignment The horizontal alignment of the texture.
		//! @param vertical_alignment The vertical alignment of the texture.
		//! @param color_vector A factor to be applied component-wise to the texture's color.
		//! @param src_rect The section of the texture to be drawn. If nullopt, the entire texture is used.
		//! @tparam Point The point type. Supported types are units::view_space::point and units::window_space::point.
		template <typename Point>
		void draw
			( Point position
			, units::texture_space::h_align horizontal_alignment = units::texture_space::align_left
			, units::texture_space::v_align vertical_alignment = units::texture_space::align_top
			, units::colors::color_vector color_vector = units::colors::white_vector()
			, std::optional<units::texture_space::box> const& src_rect = std::nullopt
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
				, color_vector
				, units::view_space::vector{1.0f, 1.0f}
				, units::world_space::radians::zero()
				, src_rect
				);
		}

		//! Draws all or part the texture to the screen (or current frame buffer), using the provided transformations.
		//! @param position The coordinates of the texture on the screen in view or window space.
		//! @param origin The offset from the texture's center to its origin point.
		//! @param color_vector A factor to be applied component-wise to the texture's color.
		//! @param scale The scale of the texture: <horizontal scale, vertical scale>.
		//! @param vertical_scale The vertical scale of the texture.
		//! @param angle The counter-clockwise rotation of the the texture around the origin, in radians.
		//! @param src_rect The section of the texture to be drawn. If nullopt, the entire texture is used.
		void draw_transformed
			( std::variant<units::window_space::point, units::view_space::point> position
			, units::texture_space::vector origin = units::texture_space::vector::zero()
			, units::colors::color_vector color_vector = units::colors::white_vector()
			, units::view_space::vector scale = units::view_space::vector{1.0f, 1.0f}
			, units::world_space::radians angle = units::world_space::radians::zero() //! @todo This should use screen space radians.
			, std::optional<units::texture_space::box> const& src_rect = std::nullopt
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
		units::window_space::vector _size;

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
