//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "Section.h"
#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "utility/utility.h"

namespace questless
{
	class Game;

	//! A large set of connected sections of hexagonal tiles.
	class Region
	{
	public:
		//! Pseudo-randomly generates a new region.
		//! @param name The name of the new region.
		Region(std::string region_name);

		//! Loads the region with filename @p save_name from disk.
		Region(char const* save_name, std::string region_name);

		//! The region's name, as it appears in game and on disk.
		std::string name() const { return _name; }

		//! Saves the region to disk.
		//! @param save_name The name of the region's save file.
		void save(char const* save_name);

		//! Removes the given being from the turn queue if it's there.
		//! @param being A being.
		void remove_from_turn_queue(Being& being) { if (being.busy_time <= 0.0) _turn_queue.erase(being); }

		//! Adds the given being to the turn queue if it's not busy.
		//! @param being A being.
		void add_to_turn_queue(Being& being) { if (being.busy_time <= 0.0) _turn_queue.insert(being); }

		//! The being whose turn it is to act or null if none are ready.
		Being* next_ready_being();

		//! The ID of the being at @p region_tile_coords or nullopt if none.
		std::optional<Id<Being>> being_id(RegionTileCoords region_tile_coords) const;

		//! The ID of the object at @p region_tile_coords or nullopt if none.
		std::optional<Id<Object>> object_id(RegionTileCoords region_tile_coords) const;

		//! The being at @p region_tile_coords or nullptr if none.
		Being const* being(RegionTileCoords region_tile_coords) const { return being_helper(region_tile_coords); }
		//! The being at @p region_tile_coords or nullptr if none.
		Being* being(RegionTileCoords region_tile_coords) { return being_helper(region_tile_coords); }

		//! The object at @p region_tile_coords or nullptr if none.
		Object const* object(RegionTileCoords region_tile_coords) const { return object_helper(region_tile_coords); }
		//! The object at @p region_tile_coords or nullptr if none.
		Object* object(RegionTileCoords region_tile_coords) { return object_helper(region_tile_coords); }

		//! Spawns the player-controlled being @p player_being in the region at an arbitrary location and adds it to the being cache.
		void spawn_player(Being::uptr player_being);

		//! Adds @p being to the region, setting its coordinates to @p region_tile_coords.
		void add(Being& being, RegionTileCoords region_tile_coords);

		//! Adds @p object to the region, setting its coordinates to @p region_tile_coords.
		void add(Object& object, RegionTileCoords region_tile_coords);

		//! Spawns @p being in the region, setting its coordinates to @p region_tile_coords and adding it to the being cache.
		void spawn(Being::uptr being, RegionTileCoords region_tile_coords);

		//! Spawns @p object in the region, setting its coordinates to @p region_tile_coords and adding it to the object cache.
		void spawn(Object::uptr object, RegionTileCoords region_tile_coords);

		//! Moves @p being to the tile at @p region_tile_coords.
		//! @return Whether @p being was successfully moved.
		bool move(Being& being, RegionTileCoords region_tile_coords);

		//! Moves @p object to the tile at @p region_tile_coords.
		//! @return Whether @p object was successfully moved.
		bool move(Object& object, RegionTileCoords region_tile_coords);

		//! Removes @p being from the region, if present.
		void remove(Being& being);

		//! Removes @p object from the region, if present.
		void remove(Object& object);

		//! The tile at @p region_tile_coords or nullptr if none.
		Tile* tile(RegionTileCoords region_tile_coords) { return tile_helper(region_tile_coords); }

		//! The tile at @p region_tile_coords or nullptr if none.
		Tile const* tile(RegionTileCoords region_tile_coords) const { return tile_helper(region_tile_coords); }

		//! The section at @p region_section_coords.
		Section* section(RegionSectionCoords region_section_coords) { return section_helper(region_section_coords); }
		//! The section at @p region_section_coords.
		Section const* section(RegionSectionCoords region_section_coords) const { return section_helper(region_section_coords); }

		//! The section that contains @p region_tile_coords or nullptr if none.
		Section* containing_section(RegionTileCoords region_tile_coords) { return containing_section_helper(region_tile_coords); }
		//! The section that contains @p region_tile_coords or nullptr if none.
		Section const* containing_section(RegionTileCoords region_tile_coords) const { return containing_section_helper(region_tile_coords); }

		//! Updates everything contained in the region.
		void update();
	private:
		// Define the maximum size of the sections rhomboid that should be loaded at any given time.
		static const int _loaded_sections_q_radius = 1;
		static const int _loaded_sections_r_radius = 1;

		std::string _name;
		std::map<RegionSectionCoords, std::unique_ptr<Section>> _section_map; //! @todo Replace unique_ptr with reference_wrapper (disallow null sections).
		RegionSectionCoords center_section_coords = RegionSectionCoords{0, 0};

		std::set<Being::ref, Being::ref_less_t> _turn_queue;

		//! Performs some operation on each section in the loaded rhomboid of sections.
		//! @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(Section&)> f);

		// Helper methods to avoid duplicating code for const and non-const versions of methods.

		Being* being_helper(RegionTileCoords region_tile_coords) const;
		Object* object_helper(RegionTileCoords region_tile_coords) const;

		Tile* tile_helper(RegionTileCoords region_tile_coords) const;

		Section* section_helper(RegionSectionCoords region_section_coords) const
		{
			auto it = _section_map.find(region_section_coords);
			return it == _section_map.end() ? nullptr : it->second.get();
		}

		Section* containing_section_helper(RegionTileCoords region_tile_coords) const
		{
			auto it = _section_map.find(Section::region_section_coords(region_tile_coords));
			return it == _section_map.end() ? nullptr : it->second.get();
		}
	};
}
