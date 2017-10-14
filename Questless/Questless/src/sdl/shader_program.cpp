//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/shader_program.h"

#include <stdexcept>

namespace sdl
{
	shader_program::shader_program(char const* vertex_source, char const* fragment_source)
		: _program{glCreateProgram()}
	{
		// Create and compile vertex shader and attach it to the program.
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
		glCompileShader(vertex_shader);
		{ // Check vertex shader for errors.
			GLint success = GL_FALSE;
			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {
				throw std::runtime_error{"Failed to compile vertex shader."};
			}
		}
		glAttachShader(_program, vertex_shader);

		// Create and compile fragment shader and attach it to the program.
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
		glCompileShader(fragment_shader);
		{ // Check fragment shader for errors.
			GLint success = GL_FALSE;
			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {
				throw std::runtime_error{"Failed to compile fragment shader."};
			}
		}
		glAttachShader(_program, fragment_shader);

		// Link program.
		glLinkProgram(_program);
		{ // Check for link errors.
			GLint success = GL_TRUE;
			glGetProgramiv(_program, GL_LINK_STATUS, &success);
			if (success != GL_TRUE) {
				throw std::runtime_error{"Failed to link OpenGL program."};
			}
		}
	}

	GLint shader_program::get_attribute_handle(char const* attribute_name) const
	{
		GLint result = glGetAttribLocation(_program, attribute_name);
		if (result == -1) {
			throw std::runtime_error{"GLSL program attribute not found."};
		}
		return result;
	}

	GLint shader_program::get_uniform_handle(char const* uniform_name) const
	{
		GLint result = glGetUniformLocation(_program, uniform_name);
		if (result == -1) {
			throw std::runtime_error{"GLSL program uniform not found."};
		}
		return result;
	}
}
