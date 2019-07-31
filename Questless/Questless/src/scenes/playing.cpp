//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "playing.hpp"

#include "agents/agent.hpp"
#include "entities/beings/body.hpp"
#include "items/magic/gatestone.hpp"
#include "rsrc/fonts.hpp"
#include "rsrc/hud.hpp"
#include "utility/reference.hpp"
#include "world/region.hpp"
#include "world/spawn_player.hpp"
#include "world/world_space.hpp"

#include <fmt/format.h>

using namespace vecx;
using namespace vecx::literals;

namespace ql::scenes {
	playing::playing(view::vector window_size, rsrc::fonts const& fonts, ent region_id)
		: scene{fonts}
		, _region_id{region_id}
		, _player_id{spawn_player(_region_id, &_hud)}
		, _root{_hud, window_size}
		, _hud{_root, rsrc::hud{_entity_resources, fonts, _item_resources, _particle_resources, _tile_resources}, _player_id} //
	{}

	update_result playing::scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) {
		// Update the HUD.
		_hud.update(elapsed_time, events);

		auto const elapsed_ticks = 1_tick;

		// Update gatestones.
		reg.view<gatestone>().each([&](gatestone& gatestone) { gatestone.update(elapsed_ticks); });

		// For each being, update its body and allow it to act.
		reg.view<body, agent>().each([&](body& body, agent& agent) {
			body.update(elapsed_ticks);
			agent.act().get();
		});

		// Update the region.
		reg.get<region>(_region_id).update(elapsed_ticks);

		return continue_scene{};
	}

	void playing::scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Draw the HUD.
		_hud.draw(target, states);

		{ // Draw the current time.
			auto const& region = reg.get<ql::region>(_region_id);
			tick const time_of_day = region.time_of_day();
			std::string time_name;
			switch (region.period_of_day()) {
				case period_of_day::morning:
					time_name = "Morning";
					break;
				case period_of_day::afternoon:
					time_name = "Afternoon";
					break;
				case period_of_day::dusk:
					time_name = "Dusk";
					break;
				case period_of_day::evening:
					time_name = "Evening";
					break;
				case period_of_day::night:
					time_name = "Night";
					break;
				case period_of_day::dawn:
					time_name = "Dawn";
					break;
			}
			std::string time_string = fmt::format("Time: {} ({}, {})", region.time().value, time_of_day.value, time_name);
			sf::Text time_text{time_string, fonts.firamono, 20};
			time_text.setFillColor(sf::Color::White);
			time_text.setPosition({0, 50});
			target.draw(time_text, states);
		}
	}
}
