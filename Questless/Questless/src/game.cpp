//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "game.hpp"

#include "entities/beings/being.hpp"
#include "scenes/scene.hpp"
#include "scenes/splash.hpp"
#include "utility/io.hpp"
#include "utility/random.hpp"
#include "utility/visitation.hpp"
#include "world/coordinates.hpp"
#include "world/light_source.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std::chrono;

namespace ql {
	game::game(bool fullscreen) {
#if true
		constexpr int _dflt_window_width = 1024;
		constexpr int _dflt_window_height = 768;
#else
		constexpr int _dflt_window_width = 1920;
		constexpr int _dflt_window_height = 1080;
#endif
		_window.sf::RenderWindow window{sf::VideoMode{_dflt_window_width, _dflt_window_height}, "Questless"};
		sf::Image icon;
		icon.loadFromFile("resources/textures/icon.png");
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// Load textures and graphics.

		load_textures();

		// Initialize game state.

		_scene = umake<scene::splash>();
	}

	game::~game() = default;

	void game::load_textures() {
		_txt_hex_highlight = umake<texture>("resources/textures/terrain/tile.png");

		_txt_hex_circle = umake<texture>("resources/textures/ui/hex_circle.png");
	}

	void game::run() {
		bool running = true;
		while (running) {
			match(_scene->update(_im), //
				[&](scene::continue_scene) { _window.draw(*_scene); },
				[&](scene::switch_scene ss) { _scene = std::move(ss.new_scene); },
				[&](scene::game_over) { running = false; });
		}
	}
}
