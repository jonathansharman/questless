//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entity_widget.hpp"
#include "tile_widget.hpp"

#include "animation/sprite_animation.hpp"
#include "rsrc/world_widget.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include <optional>
#include <unordered_map>

namespace ql {
	namespace effects {
		struct effect;
	}
	struct world_view;

	//! Handles interaction with the world, as the player sees it.
	struct world_widget : widget {
		world_widget(rsrc::world_widget const& resources);

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		auto render_view(world_view const& view) -> void;

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		auto on_parent_resize(view::vector parent_size) -> void final;

		auto on_key_press(sf::Event::KeyEvent const& event) -> event_handled final;

		auto on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_release(sf::Event::MouseButtonEvent const& event) -> event_handled final;

		auto on_mouse_move(view::point mouse_position) -> void final;

		//! Causes the tiles for which @p predicate returns true to be highlighted.
		auto set_highlight_predicate(std::function<bool(tile_hex::point)> predicate) -> void;

		//! Clears the current highlight predicate so that no tiles are highlighted.
		auto clear_highlight_predicate() -> void;

		auto render_effect(effects::effect const& effect) -> void;

	private:
		rsrc::world_widget _rsrc;
		sf::Sound _arrow_sound;
		sf::Sound _hit_sound;
		sf::Sound _pierce_sound;
		sf::Sound _shock_sound;
		sf::Sound _telescope_sound;

		view::point _position;
		view::vector _size;

		std::optional<view::point> _o_drag_start;

		std::unordered_map<id, tile_widget> _tile_widgets;
		std::unordered_map<id, entity_widget> _entity_widgets;

		std::vector<uptr<animation>> _effect_animations;

		std::optional<std::function<bool(tile_hex::point)>> _highlight_predicate;

		auto render_terrain(world_view const& view) -> void;

		auto render_entities(world_view const& view) -> void;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
