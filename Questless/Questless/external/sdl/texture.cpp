//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "texture.hpp"

#include "resources.hpp"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <string>

using namespace vecx;
using namespace sdl::spaces;
using namespace sdl::spaces::colors;
using namespace sdl::spaces::colors::literals;
using namespace sdl::spaces::view::literals;
using namespace sdl::spaces::window::literals;

namespace {
	//! @todo This is not completely portable. Find a fully portable way to translate an SDL surface format into a GL texture format.

	//! The OpenGL texture format corresponding to the format of @p surface.
	GLenum get_gl_format(SDL_Surface* surface) {
		if (surface->format->BytesPerPixel == 4) {
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
		assert(false && "Unsupported pixel format.");
		return GLenum{};
	}
}

namespace sdl {
	texture::texture(spaces::window::vector size, color color) : _size{size} {
		if (width() != 0_px && height() != 0_px) {
			// Generate texture.
			glGenTextures(1, &_texture);
			// Bind texture.
			glBindTexture(GL_TEXTURE_2D, _texture);
			// Set texture data.
			std::vector<GLubyte> blank_buffer(width().value * height().value * 4);
			for (std::size_t i = 0; i < blank_buffer.size(); i += 4) {
				blank_buffer[i] = static_cast<GLubyte>(255 * r(color).value);
				blank_buffer[i + 1] = static_cast<GLubyte>(255 * g(color).value);
				blank_buffer[i + 2] = static_cast<GLubyte>(255 * b(color).value);
				blank_buffer[i + 3] = static_cast<GLubyte>(255 * a(color).value);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width().value, height().value, 0, GL_RGBA, GL_UNSIGNED_BYTE, &blank_buffer[0]);
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

	texture::texture(char const* filename) {
		// Load image into a surface.
		SDL_Surface* surface = IMG_Load(filename);
		if (!surface) {
			throw std::runtime_error(("Failed to load texture \"" + std::string{filename} +"\".").c_str());
		}

		// Store width and height;
		_size = spaces::window::vector{spaces::window::px{surface->w}, spaces::window::px{surface->h}};

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width().value, height().value, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
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

	texture::texture(SDL_Surface* surface) {
		// Store width and height;
		_size = spaces::window::vector{spaces::window::px{surface->w}, spaces::window::px{surface->h}};

		// Determine surface format.
		GLenum format = get_gl_format(surface);

		// Generate texture.
		glGenTextures(1, &_texture);
		// Bind texture.
		glBindTexture(GL_TEXTURE_2D, _texture);
		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width().value, height().value, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
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

	texture::~texture() {
		if (_texture) {
			glDeleteTextures(1, &_texture);
		}
	}

	texture& texture::operator =(texture&& that) & {
		_texture = that._texture;
		that._texture = NULL;

		_vbo = that._vbo;
		_ibo = that._ibo;
		_size = that._size;

		return *this;
	}

	void texture::draw_transformed
		( std::variant<spaces::window::point, spaces::view::point> position
		, spaces::texture::vector origin
		, color color_factor
		, view::vector scale
		, radians angle
		, std::optional<spaces::texture::box> const& src_rect
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
			glUniform4f(color_vector_uniform, r(color_factor).value, g(color_factor).value, b(color_factor).value, a(color_factor).value);
		}

		{ // Set model matrix.
			int const width = src_rect ? spaces::texture::width(*src_rect).value : x(_size).value;
			int const height = src_rect ? spaces::texture::height(*src_rect).value : y(_size).value;

			glm::mat4 model_matrix;
			// Position. Correct for odd width/height in window space by nudging position by half a pixel.
			auto const view_space_position = [&] {
				struct to_float_coords {
					int const width;
					int const height;
					auto operator ()(view::point position) { return position; }
					auto operator ()(spaces::window::point position) {
						return view::point
							{ x(position) + ((width & 1) ? 0.5_view_length : 0.0_view_length)
							, y(position) + ((height & 1) ? 0.5_view_length : 0.0_view_length)
							};
					}
				};
				return std::visit(to_float_coords{width, height}, position);
			}();
			model_matrix = glm::translate(model_matrix, glm::vec3{x(view_space_position).value, y(view_space_position).value, 0.0f});
			// Orientation
			model_matrix = glm::rotate
				( model_matrix
				, static_cast<float>(-angle.value)
				, glm::vec3{0.0f, 0.0f, 1.0f}
				);
			// Scale
			model_matrix = glm::scale(model_matrix, glm::vec3{x(scale).value * width, y(scale).value * height, 1.0f});
			// Origin
			model_matrix = glm::translate(model_matrix, glm::vec3{-1.0f * u(origin).value / width, -1.0f * v(origin).value / height, 0.0f});

			GLuint const model_matrix_uniform = dflt_program().get_uniform_handle("model_matrix"); //! @todo Cache this, invalidating cache after window resize.
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Get texture coordinates.
		float const u0 = src_rect ? static_cast<float>(spaces::texture::left(*src_rect).value) / width().value : 0.0f;
		float const u1 = src_rect ? static_cast<float>(spaces::texture::right(*src_rect).value) / width().value : 1.0f;
		float const v0 = src_rect ? static_cast<float>(spaces::texture::top(*src_rect).value) / height().value : 0.0f;
		float const v1 = src_rect ? static_cast<float>(spaces::texture::bottom(*src_rect).value) / height().value : 1.0f;

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

	GLuint texture::as_target_prologue(shader_program const& shader_program) {
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

	void texture::as_target_epilogue(GLuint fbo, shader_program const& shader_program) {
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
