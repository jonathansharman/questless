//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Provides access to shared resources.

#pragma once

#include <memory>

#include "ShaderProgram.h"
#include "Font.h"
#include "Input.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "Texture.h"
#include "Window.h"

namespace sdl
{
	//! The default OpenGL program, using basic shaders.
	ShaderProgram& dflt_program();

	//! Sets the default shader program to the given value.
	void dflt_program(std::unique_ptr<ShaderProgram> shader_program);

	//! The OpenGL program for rendering to texture targets.
	ShaderProgram& texture_program();

	//! Sets the texture shader program to the given value.
	void texture_program(std::unique_ptr<ShaderProgram> shader_program);

	//! The ID of the position attribute in the vertex shader.
	GLint vs_attr_position(ShaderProgram const& shader_program);

	//! The ID of the texture_coords attribute in the vertex shader.
	GLint vs_attr_texture_coords(ShaderProgram const& shader_program);

	//! The ID of the color_factor attribute in the vertex shader.
	GLint vs_attr_color_factor(ShaderProgram const& shader_program);

	//! The renderer.
	Renderer& renderer();

	//! Sets the renderer to the given value.
	void renderer(std::unique_ptr<Renderer> renderer);

	//! The application window.
	Window& window();

	//! Sets the window to the given value.
	void window(std::unique_ptr<Window> window);

	//! The input object.
	Input& input();

	//! The texture manager.
	ResourceManager<Texture>& texture_manager();
	using TextureHandle = Handle<Texture>;

	//! The font manager.
	ResourceManager<Font>& font_manager();
	using FontHandle = Handle<Font>;

	//! The sound manager.
	ResourceManager<Sound>& sound_manager();
	using SoundHandle = Handle<Sound>;
}
