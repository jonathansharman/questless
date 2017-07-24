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

		//! The ID of the position attribute in the vertex shader.
		GLint vs_attr_position() const
		{
			static bool first_call = true;
			static GLint result = -1;
			if (first_call) {
				result = glGetAttribLocation(_program, "position");
				if (result == -1) {
					throw std::runtime_error{"\"position\" is not a valid GLSL program variable."};
				}
				first_call = false;
			}
			return result;
		}

		//! The ID of the texture_coords attribute in the vertex shader.
		GLint vs_attr_texture_coords() const
		{
			static bool first_call = true;
			static GLint result = -1;
			if (first_call) {
				result = glGetAttribLocation(_program, "texture_coords");
				if (result == -1) {
					throw std::runtime_error{"\"texture_coords\" is not a valid GLSL program variable."};
				}
				first_call = false;
			}
			return result;
		}

		//! The ID of the color_factor attribute in the vertex shader.
		GLint vs_attr_color_factor() const
		{
			static bool first_call = true;
			static GLint result = -1;
			if (first_call) {
				result = glGetAttribLocation(_program, "color_factor");
				if (result == -1) {
					throw std::runtime_error{"\"color_factor\" is not a valid GLSL program variable."};
				}
				first_call = false;
			}
			return result;
		}
	private:
		GLuint _program;
	};
}
