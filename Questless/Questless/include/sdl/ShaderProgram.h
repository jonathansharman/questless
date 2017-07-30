//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <stdexcept>

#include <glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace sdl
{
	class ShaderProgram
	{
	public:
		ShaderProgram() : _program{glCreateProgram()} {}

		GLuint opengl_program_handle() const { return _program; }

		//! The handle of the position attribute in the vertex shader.
		GLint vs_attr_position() const
		{
			static GLint result = attribute_handle("position");
			return result;
		}

		//! The handle of the texture_coords attribute in the vertex shader.
		GLint vs_attr_texture_coords() const
		{
			static GLint result = attribute_handle("texture_coords");
			return result;
		}

		//! The handle of the model_matrix uniform in the vertex shader.
		GLint model_matrix() const
		{
			static GLint result = uniform_handle("model_matrix");
			return result;
		}

		//! The handle of the flip_y uniform in the vertex shader.
		GLint flip_y() const
		{
			static GLint result = uniform_handle("flip_y");
			return result;
		}

		//! The handle of the color_factor uniform in the fragment shader.
		GLint color_factor() const
		{
			static GLint result = uniform_handle("color_factor");
			return result;
		}
	private:
		GLuint _program;

		GLint attribute_handle(std::string const& attribute_name) const
		{
			GLint result = glGetAttribLocation(_program, attribute_name.c_str());
			if (result == -1) {
				throw std::runtime_error{'\"' + attribute_name + "\" GLSL program attribute not found."};
			}
			return result;
		}
		GLint uniform_handle(std::string const& uniform_name) const
		{
			GLint result = glGetUniformLocation(_program, uniform_name.c_str());
			if (result == -1) {
				throw std::runtime_error{'\"' + uniform_name + "\" GLSL program uniform not found."};
			}
			return result;
		}
	};
}
