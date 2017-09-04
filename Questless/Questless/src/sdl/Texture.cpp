//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Texture.h"

#include <string>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "sdl/resources.h"

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
	Texture::Texture(int width, int height, colors::Color color)
		: _width{width}
		, _height{height}
	{
		if (_width != 0 && _height != 0) {
			// Generate texture.
			glGenTextures(1, &_texture);
			// Bind texture.
			glBindTexture(GL_TEXTURE_2D, _texture);
			// Set texture data.
			std::vector<GLubyte> blank_buffer(width * height * 4);
			for (std::size_t i = 0; i < blank_buffer.size(); i += 4) {
				blank_buffer[i] = static_cast<GLubyte>(255 * color.red());
				blank_buffer[i + 1] = static_cast<GLubyte>(255 * color.green());
				blank_buffer[i + 2] = static_cast<GLubyte>(255 * color.blue());
				blank_buffer[i + 3] = static_cast<GLubyte>(255 * color.alpha());
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &blank_buffer[0]);
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

	Texture::Texture(char const* filename)
	{
		// Load image into a surface.
		SDL_Surface* surface = IMG_Load(filename);
		if (!surface) {
			throw std::runtime_error(("Failed to load texture \"" + std::string{filename} +"\".").c_str());
		}

		// Store width and height;
		_width = surface->w;
		_height = surface->h;

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
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

	Texture::Texture(SDL_Surface* surface)
	{
		// Store width and height;
		_width = surface->w;
		_height = surface->h;

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
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

	Texture::Texture(Texture&& that)
		: _texture{that._texture}
		, _vbo{that._vbo}
		, _ibo{that._ibo}
		, _width{that._width}
		, _height{that._height}
	{
		that._texture = NULL;
	}

	Texture::~Texture()
	{
		if (_texture) {
			glDeleteTextures(1, &_texture);
		}
	}

	Texture& Texture::operator =(Texture&& that) &
	{
		_texture = that._texture;
		that._texture = NULL;

		_vbo = that._vbo;
		_ibo = that._ibo;
		_width = that._width;
		_height = that._height;

		return *this;
	}

	void Texture::draw
		( ScreenSpace::Point position
		, TextureSpace::HAlign horizontal_alignment
		, TextureSpace::VAlign vertical_alignment
		, ColorFactor color_factor
		, std::optional<TextureSpace::Box> const& src_rect
		) const
	{
		switch (horizontal_alignment) {
			case TextureSpace::align_left:
				position.x() += _width / 2;
				break;
			case TextureSpace::align_center:
				break;
			case TextureSpace::align_right:
				position.x() -= _width / 2;
				break;
		}
		switch (vertical_alignment) {
			case TextureSpace::align_top:
				position.y() += _height / 2;
				break;
			case TextureSpace::align_middle:
				break;
			case TextureSpace::align_bottom:
				position.y() -= _height / 2;
				break;
		}
		draw_transformed(position, TextureSpace::Vector::zero(), color_factor, 1.0f, 1.0f, GameSpace::Radians::zero(), src_rect);
	}

	void Texture::draw_transformed
		( ScreenSpace::Point position
		, TextureSpace::Vector origin
		, ColorFactor color_factor
		, float horizontal_scale
		, float vertical_scale
		, units::GameSpace::Radians angle
		, std::optional<TextureSpace::Box> const& src_rect
		) const
	{
		// Bind program.
		dflt_program().use();

		// Enable vertex attributes.
		static GLuint position_attribute = dflt_program().get_attribute_handle("position");
		glEnableVertexAttribArray(position_attribute);
		static GLuint texture_coords_attribute = dflt_program().get_attribute_handle("texture_coords");
		glEnableVertexAttribArray(texture_coords_attribute);

		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		glEnable(GL_TEXTURE_2D);

		{ // Set color factor.
			static GLuint color_factor_uniform = dflt_program().get_uniform_handle("color_factor");
			glUniform4f(color_factor_uniform, color_factor.red(), color_factor.green(), color_factor.blue(), color_factor.alpha());
		}

		{ // Set model matrix.
			int const width = src_rect ? units::width(*src_rect) : _width;
			int const height = src_rect ? units::height(*src_rect) : _height;

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

			static GLuint model_matrix_uniform = dflt_program().get_uniform_handle("model_matrix");
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Get texture coordinates.
		float const u0 = src_rect ? static_cast<float>(left(*src_rect)) / _width : 0.0f;
		float const u1 = src_rect ? static_cast<float>(right(*src_rect)) / _width : 1.0f;
		float const v0 = src_rect ? static_cast<float>(top(*src_rect)) / _height : 0.0f;
		float const v1 = src_rect ? static_cast<float>(bottom(*src_rect)) / _height : 1.0f;

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

	GLuint Texture::as_target_prologue(ShaderProgram const& shader_program)
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

	void Texture::as_target_epilogue(GLuint fbo, ShaderProgram const& shader_program)
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
