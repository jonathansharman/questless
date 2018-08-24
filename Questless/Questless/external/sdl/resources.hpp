//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Provides access to shared resources.

#pragma once

#include <memory>

#include "shader_program.hpp"
#include "font.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "sound.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace sdl {
	//! The OpenGL context.
	SDL_GLContext& opengl_context();

	//! The default OpenGL program, using basic shaders.
	shader_program& dflt_program();

	//! Sets the default shader program to the given value.
	void set_dflt_program(std::unique_ptr<shader_program> shader_program);

	//! The solid OpenGL program, using shaders for solids.
	shader_program& solid_program();

	//! Sets the solid shader program to the given value.
	void set_solid_program(std::unique_ptr<shader_program> shader_program);

	//! The renderer.
	renderer& the_renderer();

	//! Sets the renderer to the given value.
	void set_the_renderer(std::unique_ptr<renderer> renderer);

	//! The application window.
	window& the_window();

	//! Sets the window to the given value.
	void set_the_window(std::unique_ptr<window> the_window);

	//! The input object.
	input& the_input();

	//! The texture manager.
	resource_manager<texture>& the_texture_manager();
	using texture_handle = handle<texture>;

	//! The font manager.
	resource_manager<font>& the_font_manager();
	using font_handle = handle<font>;

	//! The sound manager.
	resource_manager<sound>& the_sound_manager();
	using sound_handle = handle<sound>;
}
