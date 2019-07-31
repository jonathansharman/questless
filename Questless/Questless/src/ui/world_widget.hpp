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
		world_widget( //
			widget& parent,
			rsrc::world_widget const& resources);

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		void render_view(world_view const& view);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Causes the tiles for which @p predicate returns true to be highlighted.
		void set_highlight_predicate(std::function<bool(region_tile::point)> predicate);

		//! Clears the current highlight predicate so that no tiles are highlighted.
		void clear_highlight_predicate();

		void render_effect(effects::effect const& effect);

	private:
		rsrc::world_widget const& _resources;

		sf::View _view;

		std::unordered_map<ent, tile_widget> _tile_widgets;
		std::unordered_map<ent, entity_widget> _entity_widgets;

		std::vector<uptr<animation>> _effect_animations;

		std::optional<std::function<bool(region_tile::point)>> _highlight_predicate;

		void render_terrain(world_view const& view);
		void render_entities(world_view const& view);

		//! Draws the visible terrain.
		void draw_terrain(sf::RenderTarget& target, sf::RenderStates states) const;

		//! Draws the visible entities in the world.
		void draw_entities(sf::RenderTarget& target, sf::RenderStates states) const;

		//! Draws visualizations of any active effects in the world.
		void draw_effects(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}
