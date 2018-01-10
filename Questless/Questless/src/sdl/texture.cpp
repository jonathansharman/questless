//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/texture.hpp"

#include <string>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "sdl/resources.hpp"

using namespace units;
using namespace units::colors;

namespace
{
	//! @todo This is not completely portable. Find a fully portable way to translate an SDL surface format into a GL texture format.

	//! The OpenGL texture format corresponding to the format of @p surface.
	GLenum get_gl_format(SDL_Surface* surface)
	{
		if (surface->format->BytesPerPixel == 4)
		{
			if (surface->format->Rshift == 24 && surface->format->Aloss == 0) {
				return GL_ABGR_EXT;
			} else if (surface->format->Rshift == 16 && surface->format->Aloss == 8) {
				return GL_BGRA;
			} else if (surface->format->Rshift == 16 && surface->format->Ashift == 24) {
				return GL_BGRA;
			} else if (surface->format->Rshift == 0 && surface->format->Ashift == 24) {
				return GL_RGBA;
			}
		} else if (surface->format->BytesPerPixel == 3) {
			if (surface->format->Rshift == 16) {
				return GL_BGR;
			} else if (surface->format->Rshift == 0) {
				return GL_RGB;
			}
		}
		throw std::logic_error("Unsupported pixel format.");
	}
}

namespace sdl
{
	texture::texture(screen_space::vector size, colors::color color) : _size{size}
	{
		if (width() != 0 && height() != 0) {
			// Generate texture.
			glGenTextures(1, &_texture);
			// Bind texture.
			glBindTexture(GL_TEXTURE_2D, _texture);
			// Set texture data.
			std::vector<GLubyte> blank_buffer(width() * height() * 4);
			for (std::size_t i = 0; i < blank_buffer.size(); i += 4) {
				blank_buffer[i] = static_cast<GLubyte>(255 * color.red());
				blank_buffer[i + 1] = static_cast<GLubyte>(255 * color.green());
				blank_buffer[i + 2] = static_cast<GLubyte>(255 * color.blue());
				blank_buffer[i + 3] = static_cast<GLubyte>(255 * color.alpha());
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &blank_buffer[0]);
			// Set minification and magnification filters.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Create VBO.
			glGenBuffers(1, &_vbo);

			{ // Create IBO.
				GLuint index_data[]{0, 1, 2, 3};
				glGenBuffers(1, &_ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_DYNAMIC_DRAW);
			}
		} else {
			throw std::runtime_error("Attempted to create texture with zero dimensions.");
		}
	}

	texture::texture(char const* filename)
	{
		// Load image into a surface.
		SDL_Surface* surface = IMG_Load(filename);
		if (!surface) {
			throw std::runtime_error(("Failed to load texture \"" + std::string{filename} +"\".").c_str());
		}

		// Store width and height;
		_size = screen_space::vector{surface->w, surface->h};

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, format, GL_UNSIGNED_BYTE, surface->pixels);
		// Set minification and magnification filters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Free the surface.
		SDL_FreeSurface(surface);

		// Create VBO.
		glGenBuffers(1, &_vbo);

		{ // Create IBO.
			GLuint index_data[] { 0, 1, 2, 3 };
			glGenBuffers(1, &_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_STATIC_DRAW);
		}
	}

	texture::texture(SDL_Surface* surface)
	{
		// Store width and height;
		_size = screen_space::vector{surface->w, surface->h};

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, format, GL_UNSIGNED_BYTE, surface->pixels);
		// Set minification and magnification filters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Create VBO.
		glGenBuffers(1, &_vbo);

		{ // Create IBO.
			GLuint index_data[]{0, 1, 2, 3};
			glGenBuffers(1, &_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_STATIC_DRAW);
		}
	}

	texture::texture(texture&& that)
		: _texture{that._texture}
		, _vbo{that._vbo}
		, _ibo{that._ibo}
		, _size{that._size}
	{
		that._texture = NULL;
	}

	texture::~texture()
	{
		if (_texture) {
			glDeleteTextures(1, &_texture);
		}
	}

	texture& texture::operator =(texture&& that) &
	{
		_texture = that._texture;
		that._texture = NULL;

		_vbo = that._vbo;
		_ibo = that._ibo;
		_size = that._size;

		return *this;
	}

	void texture::draw
		( screen_space::point position
		, texture_space::h_align horizontal_alignment
		, texture_space::v_align vertical_alignment
		, color_vector color_vector
		, std::optional<texture_space::box> const& src_rect
		) const
	{
		switch (horizontal_alignment) {
			case texture_space::align_left:
				position.x() += width() / 2;
				break;
			case texture_space::align_center:
				break;
			case texture_space::align_right:
				position.x() -= width() / 2;
				break;
		}
		switch (vertical_alignment) {
			case texture_space::align_top:
				position.y() += height() / 2;
				break;
			case texture_space::align_middle:
				break;
			case texture_space::align_bottom:
				position.y() -= height() / 2;
				break;
		}
		draw_transformed(position, texture_space::vector::zero(), color_vector, 1.0f, 1.0f, game_space::radians::zero(), src_rect);
	}

	void texture::draw_transformed
		( screen_space::point position
		, texture_space::vector origin
		, color_vector color_vector
		, float horizontal_scale
		, float vertical_scale
		, units::game_space::radians angle
		, std::optional<texture_space::box> const& src_rect
		) const
	{
		// Bind program.
		dflt_program().use();

		// Enable vertex attributes.
		GLuint const position_attribute = dflt_program().get_attribute_handle("position"); //! @todo Cache this, invalidating cache after window resize.
		glEnableVertexAttribArray(position_attribute);
		GLuint const texture_coords_attribute = dflt_program().get_attribute_handle("texture_coords"); //! @todo Cache this, invalidating cache after window resize.
		glEnableVertexAttribArray(texture_coords_attribute);

		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		glEnable(GL_TEXTURE_2D);

		{ // Set color vector.
			GLuint const color_vector_uniform = dflt_program().get_uniform_handle("color_vector"); //! @todo Cache this, invalidating cache after window resize.
			glUniform4f(color_vector_uniform, color_vector.red(), color_vector.green(), color_vector.blue(), color_vector.alpha());
		}

		{ // Set model matrix.
			int const width = src_rect ? units::width(*src_rect) : _size.x();
			int const height = src_rect ? units::height(*src_rect) : _size.y();

			glm::mat4 model_matrix;
			// Position. Correct for odd width/height by nudging position by half a pixel.
			float const x = (width & 1) ? position.x() + 0.5f : position.x();
			float const y = (height & 1) ? position.y() + 0.5f : position.y();
			model_matrix = glm::translate(model_matrix, glm::vec3{x, y, 0.0f});
			// Orientation
			model_matrix = glm::rotate
				( model_matrix
				, static_cast<float>(-angle.count())
				, glm::vec3{0.0f, 0.0f, 1.0f}
				);
			// Scale
			model_matrix = glm::scale(model_matrix, glm::vec3{horizontal_scale * width, vertical_scale * height, 1.0f});
			// Origin
			model_matrix = glm::translate(model_matrix, glm::vec3{-2.0f * origin.u() / width, -2.0f * origin.v() / height, 0.0f});

			GLuint const model_matrix_uniform = dflt_program().get_uniform_handle("model_matrix"); //! @todo Cache this, invalidating cache after window resize.
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Get texture coordinates.
		float const u0 = src_rect ? static_cast<float>(left(*src_rect)) / width() : 0.0f;
		float const u1 = src_rect ? static_cast<float>(right(*src_rect)) / width() : 1.0f;
		float const v0 = src_rect ? static_cast<float>(top(*src_rect)) / height() : 0.0f;
		float const v1 = src_rect ? static_cast<float>(bottom(*src_rect)) / height() : 1.0f;

		// Set vertex data.
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		GLfloat vertex_data[] =
			{ -0.5f, -0.5f, u0, v0
			,  0.5f, -0.5f, u1, v0
			,  0.5f,  0.5f, u1, v1
			, -0.5f,  0.5f, u0, v1
			};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glVertexAttribPointer(texture_coords_attribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

		// Set index data and render.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
		glDisableVertexAttribArray(texture_coords_attribute);
	}

	GLuint texture::as_target_prologue(shader_program const& shader_program)
	{
		shader_program.use();

		// Turn on y-flipping.
		GLuint flip_y_uniform = shader_program.get_uniform_handle("flip_y");
		glUniform1i(flip_y_uniform, GL_TRUE);

		// Create frame buffer object.
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Bind the texture.
		glBindTexture(GL_TEXTURE_2D, _texture);

		// Set filtering to nearest (only do interpolation once at most).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Attach texture to frame buffer.
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);

		// Set draw buffer.
		GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &draw_buffer);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error{"Frame buffer error."};
		}

		// Render to the frame buffer.
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		return fbo;
	}

	void texture::as_target_epilogue(GLuint fbo, shader_program const& shader_program)
	{
		shader_program.use();

		// Reset minification and magnification filters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Render to the screen buffer again.
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		// Turn off y-flipping.
		GLuint flip_y_uniform = shader_program.get_uniform_handle("flip_y");
		glUniform1i(flip_y_uniform, GL_FALSE);

		glDeleteFramebuffers(1, &fbo);
	}
}
