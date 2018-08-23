//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>

namespace sdl {
	class shader_program {
	public:
		//! Compiles and links the given vertex and fragment shader codes into a shader program.
		//! @param vertex_source The GLSL source code for the vertex shader.
		//! @param fragment_source The GLSL source code for the fragment shader.
		shader_program(char const* vertex_source, char const* fragment_source);

		~shader_program();

		//! Locates the handle for the attribute @p attribute_name.
		//! @param attribute_name The name of the attribute as it appears in the GLSL source code.
		//! @return The handle for the given attribute.
		GLint get_attribute_handle(char const* attribute_name) const;

		//! Locates the handle for the uniform @p uniform_name.
		//! @param uniform_name The name of the uniform as it appears in the GLSL source code.
		//! @return The handle for the given uniform.
		GLint get_uniform_handle(char const* uniform_name) const;

		//! Uses this shader program as the active program.
		void use() const { glUseProgram(_program); }
	private:
		GLuint _program;
	};
}
