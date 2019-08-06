//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "section.hpp"

#include "tile.hpp"

#include "entities/beings/being.hpp"
#include "magic/spell.hpp"
#include "utility/random.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

namespace ql {
	section::section(id region_id, region_section::point coords) : _coords{coords} {
		// Create a section with random tiles.
		for (span q = 0_span; q < section_diameter; ++q) {
			for (span r = 0_span; r < section_diameter; ++r) {
				auto const terrain = static_cast<ql::terrain>(uniform(0, static_cast<int>(terrain::terrain_count)));
				auto const rtc = region_tile_coords(coords, section_tile::point{q, r});
				location location{region_id, rtc};
				id const tile_id = reg.create();
				make_tile(tile_id, terrain, location, 0_temp, 0_lum);
				_tile_ids[q.value][r.value] = tile_id;
			}
		}
	}

	std::optional<id> section::entity_id_at(region_tile::point tile_coords) const {
		auto it = entity_id_map.find(tile_coords);
		return it != entity_id_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	bool section::try_add(id entity_id) {
		auto result = entity_id_map.insert({reg.get<location>(entity_id).coords, entity_id});
		return result.second;
	}

	typename void section::remove_at(region_tile::point coords) {
		auto it = entity_id_map.find(coords);
		if (it != entity_id_map.end()) { entity_id_map.erase(it); }
	}

	void section::remove(id entity_id) {
		remove_at(reg.get<location>(entity_id).coords);
	}
}
