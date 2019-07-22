//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "section.hpp"

#include "tile.hpp"

#include "entities/beings/being.hpp"
#include "magic/spell.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

#include <fstream>

namespace ql {
	section::section(region_section::point coords, std::istream& data_stream) : _coords{coords} {
		for (auto& slice : _tile_ids) {
			for (ent tile_id : slice) {
				int c;
				temperature temperature_offset;

				data_stream >> c >> temperature_offset.value;

				switch (static_cast<tile_subtype>(c)) {
					case tile_subtype::dirt:
						tile = umake<dirt_tile>(temperature_offset);
						break;
					case tile_subtype::edge:
						tile = umake<edge_tile>(temperature_offset);
						break;
					case tile_subtype::grass:
						tile = umake<grass_tile>(temperature_offset);
						break;
					case tile_subtype::sand:
						tile = umake<sand_tile>(temperature_offset);
						break;
					case tile_subtype::snow:
						tile = umake<snow_tile>(temperature_offset);
						break;
					case tile_subtype::stone:
						tile = umake<stone_tile>(temperature_offset);
						break;
					case tile_subtype::water:
						tile = umake<water_tile>(temperature_offset);
						break;
					default:
						assert(false && "Unrecognized tile type.");
				}
			}
		}
	}

	section::~section() {}

	void section::save(char const* filename) {
		std::ofstream fout{filename};
		for (auto& r_row : _tile_ids) {
			for (auto& tile : r_row) {
				fout << static_cast<char>(tile->subtype()) << ' ' << tile->temperature_offset.value << ' ';
			}
		}
	}

	std::optional<ent> section::entity_id_at(region_tile::point tile_coords) const {
		auto it = entity_id_map.find(tile_coords);
		return it != entity_id_map.end() ? std::make_optional(it->second) : std::nullopt;
	}

	bool section::try_add(ent entity_id) {
		auto result = entity_id_map.insert({reg.get<location>(entity_id).coords, entity_id});
		return result.second;
	}

	typename void section::remove_at(region_tile::point coords) {
		auto it = entity_id_map.find(coords);
		if (it != entity_id_map.end()) { entity_id_map.erase(it); }
	}

	void section::remove(ent entity_id) {
		remove_at(reg.get<location>(entity_id).coords);
	}
}
