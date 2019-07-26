//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "game.hpp"

#include "entities/beings/being.hpp"
#include "rsrc/utility.hpp"
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
		auto const window_style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
		_window.create(sf::VideoMode{_dflt_window_width, _dflt_window_height}, "Questless", window_style);
		auto icon = rsrc::load<sf::Image>("resources/textures/icon.png");
		_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// Initialize game state.

		_scene = umake<scenes::splash>(_window, _fonts);
	}

	game::~game() = default;

	void game::run() {
		bool running = true;
		while (running) {
			match(
				_scene->update(_im), //
				[&](scenes::continue_scene) { _window.draw(*_scene); },
				[&](scenes::switch_scene ss) { _scene = std::move(ss.new_scene); },
				[&](scenes::game_over) { running = false; });
		}
	}
}
