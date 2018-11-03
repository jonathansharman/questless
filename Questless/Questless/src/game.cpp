//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "game.hpp"

#include "animation/entity_animator.hpp"
#include "entities/beings/being.hpp"
#include "entities/objects/object.hpp"
#include "ui/scenes/splash.hpp"
#include "utility/io.hpp"
#include "utility/random.hpp"
#include "world/coordinates.hpp"
#include "world/light_source.hpp"

#include "sdl/resources.hpp"
#include "sdl/shader_program.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std::chrono;

using namespace sdl;
using namespace units;

namespace ql {
	id<being> game::get_being_id(uptr<being> const& being) { return being->id; }
	id<object> game::get_object_id(uptr<object> const& object) { return object->id; }
	id<item> game::get_item_id(uptr<item> const& item) { return item->id; }
	id<light_source> game::get_light_source_id(uptr<light_source> const& light_source) { return light_source->id; }

	game::game(bool fullscreen) {
		// Set OpenGL version.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// Initialize SDL.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
			throw std::runtime_error("Failed to initialize SDL. SDL Error: " + std::string{SDL_GetError()});
		}

		// Initialize window.
		if (fullscreen) {
			set_the_window(umake<window>("Questless", "resources/textures/icon.png", true));
		} else {
#if true
			constexpr int _dflt_window_width = 1024;
			constexpr int _dflt_window_height = 768;
#else
			constexpr int _dflt_window_width = 1920;
			constexpr int _dflt_window_height = 1080;
#endif
			set_the_window(umake<window>("Questless", "resources/textures/icon.png", false, _dflt_window_width, _dflt_window_height, true, true, true, false));
		}

		// Initialize OpenGL.
		initialize_opengl();

		// Initialize renderer.
		set_the_renderer(umake<renderer>(the_window(), the_window().width(), the_window().height()));

		if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
			throw std::runtime_error("Failed to initialize IMG. SDL Error: " + std::string{SDL_GetError()});
		}

		// Initialize and load fonts.

		if (TTF_Init()) {
			throw std::runtime_error("Failed to initialize TTF. SDL Error: " + std::string{SDL_GetError()});
		}

		// Initialize sound.

	    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024)) {
			throw std::runtime_error("Failed to initialize Mix. SDL Error: " + std::string{SDL_GetError()});
	    }

		// Load textures and graphics.

		load_textures();

		// Initialize game state.

		for (int i = 0; i < _splash_flames_count; ++i) {
			_splash_flame_positions.emplace_back(uniform(0, the_window().width() - 1), (i + 1) * the_window().height() / _splash_flames_count);
		}

		_scene = umake<scene::splash>();
	}

	game::~game() {
		// Delete renderer.
		set_the_renderer(nullptr);

		// Delete window.
		set_the_window(nullptr);

		// Quit SDL components.
		Mix_CloseAudio();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void game::load_textures() {
		_txt_test1 = umake<texture>("resources/textures/test1.png");
		_txt_test2 = umake<texture>("resources/textures/test2.png");
		_txt_test3 = umake<texture>("resources/textures/test3.png");
		_txt_test_even = umake<texture>("resources/textures/test-even.png");

		_txt_hex_highlight = umake<texture>("resources/textures/terrain/tile.png");

		_txt_hex_circle = umake<texture>("resources/textures/ui/hex_circle.png");
	}

	void game::initialize_opengl() {
		// Create OpenGL context.
		opengl_context() = SDL_GL_CreateContext(the_window().sdl_ptr()); //! @todo Destroy context?
		if (!opengl_context()) {
			throw std::runtime_error("Failed to create OpenGL context. SDL Error: " + std::string{SDL_GetError()});
		}

		// Initialize GLEW.
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW.");
		}

		// Use late swap tearing.
		if (SDL_GL_SetSwapInterval(-1)) {
			// Fall back to VSync.
			if (SDL_GL_SetSwapInterval(1)) {
				throw std::runtime_error("Failed to set swap interval. SDL Error: " + std::string{SDL_GetError()});
			}
		}

		// Generate GLSL programs.
		set_dflt_program(umake<shader_program>
			( contents_of_file("resources/shaders/dflt.vert").c_str()
			, contents_of_file("resources/shaders/dflt.frag").c_str()
			));
		set_solid_program(umake<shader_program>
			( contents_of_file("resources/shaders/solid.vert").c_str()
			, contents_of_file("resources/shaders/solid.frag").c_str()
			));

		{ // Set default program uniforms.
			dflt_program().use();

			GLint const viewport_size_uniform = dflt_program().get_uniform_handle("viewport_size");
			int const window_width = the_window().width();
			int const window_height = the_window().height();
			glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

			GLint const flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
			glUniform1i(flip_y_uniform, GL_FALSE);
		}
		{ // Set solid program uniforms.
			solid_program().use();

			GLint const viewport_size_uniform = solid_program().get_uniform_handle("viewport_size");
			int const window_width = the_window().width();
			int const window_height = the_window().height();
			glUniform2f(viewport_size_uniform, static_cast<float>(window_width), static_cast<float>(window_height));

			GLint const flip_y_uniform = dflt_program().get_uniform_handle("flip_y");
			glUniform1i(flip_y_uniform, GL_FALSE);
		}

		// Set blend function.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Main Game Logic

	void game::run() {
		world_space::seconds accrued_time = world_space::seconds::zero();
		clock::time_point last_update_time = clock::now();
		while (update(accrued_time, last_update_time) == update_result::continue_game) {
			render();
		}
		if (_region) {
			_region->save("Slot1");
		}
	}
}
