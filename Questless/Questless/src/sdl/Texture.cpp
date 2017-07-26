//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Texture.h"

#include <string>

#include <gl/GL.h>
#include <gl/GLU.h>

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
		( ScreenSpace::Box const& dst_rect
		, ColorFactor color_factor
		, std::optional<TextureSpace::Box> const& src_rect
		, ShaderProgram const& shader_program
		) const
	{
		if (_texture) {
			// Bind program.
			glUseProgram(shader_program.opengl_program_handle());

			// Enable vertex attributes.
			glEnableVertexAttribArray(shader_program.vs_attr_position());
			glEnableVertexAttribArray(shader_program.vs_attr_texture_coords());

			// Calculate texture coordinates.
			float u0, u1, v0, v1;
			if (src_rect) {
				u0 = static_cast<float>(src_rect->position.u()) / src_rect->size.u();
				u1 = static_cast<float>(src_rect->position.u() + src_rect->size.u()) / src_rect->size.u();
				v0 = static_cast<float>(src_rect->position.v()) / src_rect->size.v();
				v1 = static_cast<float>(src_rect->position.v() + src_rect->size.v()) / src_rect->size.v();
			} else {
				u0 = 0.0f;
				u1 = 1.0f;
				v0 = 0.0f;
				v1 = 1.0f;
			}

			// Set color factor.
			glUniform4f(shader_program.color_factor(), color_factor.red(), color_factor.green(), color_factor.blue(), color_factor.alpha());

			// Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			float x0 = static_cast<float>(left(dst_rect));
			float y0 = static_cast<float>(top(dst_rect));
			float x1 = static_cast<float>(right(dst_rect));
			float y1 = static_cast<float>(bottom(dst_rect));
			GLfloat vertex_data[] =
				{ x0, y0, u0, v0
				, x1, y0, u1, v0
				, x1, y1, u1, v1
				, x0, y1, u0, v1
				};
			glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertex_data, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(shader_program.vs_attr_position(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glVertexAttribPointer(shader_program.vs_attr_texture_coords(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

			// Set index data and render.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

			// Disable vertex attributes.
			glDisableVertexAttribArray(shader_program.vs_attr_position());
			glDisableVertexAttribArray(shader_program.vs_attr_texture_coords());
		}
	}

	void Texture::draw
		( ScreenSpace::Point position
		, HAlign horizontal_alignment
		, VAlign vertical_alignment
		, ColorFactor color_factor
		, std::optional<TextureSpace::Box> const& src_rect
		, ShaderProgram const& shader_program
		) const
	{
		switch (horizontal_alignment) {
			case HAlign::left:
				break;
			case HAlign::center:
				position.x() -= (_width - 1) / 2;
				break;
			case HAlign::right:
				position.x() -= _width - 1;
				break;
		}
		switch (vertical_alignment) {
			case VAlign::top:
				break;
			case VAlign::middle:
				position.y() -= (_height - 1) / 2;
				break;
			case VAlign::bottom:
				position.y() -= _height - 1;
				break;
		}
		//SDL_Rect const sdl_dst_rect{position.x, position.y, _w, _h};
		
		// This reinterpret_cast is safe because SDL_Rect and ScreenSpace::Box have the same data structure.
		//SDL_Rect const* sdl_src_rect = src_rect ? reinterpret_cast<const SDL_Rect*>(&src_rect.value()) : nullptr;

		//SDL_SetTextureColorMod(_texture, _color.r, _color.g, _color.b);
		//SDL_SetTextureAlphaMod(_texture, _color.a);
		//SDL_RenderCopy(_renderer.sdl_ptr(), _texture, sdl_src_rect, &sdl_dst_rect);

		// Draw using OpenGL directly.

		//if (_texture) {
		//	glBindTexture(GL_TEXTURE_2D, _texture);
		//	glEnable(GL_TEXTURE_2D);
		//	glColor4f(_color.r / 255.f, _color.g / 255.f, _color.b / 255.f, _color.a / 255.f);

		//	glBegin(GL_QUADS);
		//		glTexCoord2f(0.0, 0.0);
		//		glVertex3f(position.x, position.y, 0.0);
		//		glTexCoord2f(1.0, 0.0);
		//		glVertex3f(position.x + _w, position.y, 0.0);
		//		glTexCoord2f(1.0, 1.0);
		//		glVertex3f(position.x + _w, position.y + _h, 0.0);
		//		glTexCoord2f(0.0, 1.0);
		//		glVertex3f(position.x, position.y + _h, 0.0);
		//	glEnd();
		//	glDisable(GL_TEXTURE_2D);
		//}

		if (_texture) {
			// Bind program.
			glUseProgram(shader_program.opengl_program_handle());

			// Enable vertex attributes.
			glEnableVertexAttribArray(shader_program.vs_attr_position());
			glEnableVertexAttribArray(shader_program.vs_attr_texture_coords());

			// Bind texture.
			glBindTexture(GL_TEXTURE_2D, _texture);
			glEnable(GL_TEXTURE_2D);

			// Set color factor.
			glUniform4f(shader_program.color_factor(), color_factor.red(), color_factor.green(), color_factor.blue(), color_factor.alpha());

			//! @todo Account for src_rect.
			// Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			float x0 = static_cast<float>(position.x());
			float y0 = static_cast<float>(position.y());
			float x1 = static_cast<float>(position.x() + _width);
			float y1 = static_cast<float>(position.y() + _height);
			GLfloat vertex_data[] =
				{ x0, y0, 0.0f, 0.0f
				, x1, y0, 1.0f, 0.0f
				, x1, y1, 1.0f, 1.0f
				, x0, y1, 0.0f, 1.0f
				};
			glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(GLfloat), vertex_data, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(shader_program.vs_attr_position(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glVertexAttribPointer(shader_program.vs_attr_texture_coords(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

			// Set index data and render.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

			// Disable vertex attributes.
			glDisableVertexAttribArray(shader_program.vs_attr_position());
			glDisableVertexAttribArray(shader_program.vs_attr_texture_coords());
		}
	}

	void Texture::draw_transformed
		( ScreenSpace::Point position
		, std::optional<TextureSpace::Point> origin
		, ColorFactor color_factor
		, double horizontal_scale
		, double vertical_scale
		, units::GameSpace::Radians angle
		, std::optional<TextureSpace::Box> const& src_rect
		, ShaderProgram const& shader_program
		) const
	{
		int width = static_cast<int>(horizontal_scale * (src_rect ? units::width(*src_rect) : _width));
		int height = static_cast<int>(vertical_scale * (src_rect ? units::height(*src_rect) : _height));
		SDL_Rect sdl_dst_rect
			{ static_cast<int>(position.x() - (origin ? origin->u() : width / 2))
			, static_cast<int>(position.y() - (origin ? origin->v() : height / 2))
			, width
			, height
			};

		// This reinterpret_cast is safe because SDL_Rect and ScreenSpace::Box have the same data structure.
		//SDL_Rect const* sdl_src_rect = src_rect ? reinterpret_cast<const SDL_Rect*>(&src_rect.value()) : nullptr;

		//SDL_SetTextureColorMod
		//	( _texture
		//	, static_cast<uint8_t>((static_cast<uint32_t>(color.r) * _color.r) / 255)
		//	, static_cast<uint8_t>((static_cast<uint32_t>(color.g) * _color.g) / 255)
		//	, static_cast<uint8_t>((static_cast<uint32_t>(color.b) * _color.b) / 255)
		//	);
		//SDL_SetTextureAlphaMod(_texture, static_cast<uint8_t>((static_cast<uint32_t>(color.a) * _color.a) / 255));

		//constexpr double radians_to_negative_degrees = -360.0 / 6.283185307179586476925;
		//SDL_RenderCopyEx
		//	( _renderer.sdl_ptr()
		//	, _texture
		//	, sdl_src_rect
		//	, &sdl_dst_rect
		//	, radians_to_negative_degrees * angle.count()
		//	, nullptr
		//	, static_cast<SDL_RendererFlip>((flip_horizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (flip_vertically ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE))
		//	);

		// Draw using OpenGL directly.

		if (_texture) {
			glBindTexture(GL_TEXTURE_2D, _texture);
			glEnable(GL_TEXTURE_2D);

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(sdl_dst_rect.x, sdl_dst_rect.y, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(sdl_dst_rect.x + width, sdl_dst_rect.y, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(sdl_dst_rect.x + width, sdl_dst_rect.y + height, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(sdl_dst_rect.x, sdl_dst_rect.y + height, 0.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}

	GLuint Texture::as_target_prologue(ShaderProgram const& shader_program)
	{
		static GLint flip_y_uniform = glGetUniformLocation(shader_program.opengl_program_handle(), "flip_y");

		glUseProgram(shader_program.opengl_program_handle());

		// Turn on y-flipping.
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
		static GLint flip_y_uniform = glGetUniformLocation(shader_program.opengl_program_handle(), "flip_y");

		// Reset minification and magnification filters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Render to the screen buffer again.
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		// Turn off y-flipping.
		glUniform1i(flip_y_uniform, GL_FALSE);

		glDeleteFramebuffers(1, &fbo);
	}
}
