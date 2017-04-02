/**
* @file    Region.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Region class, which represents a set of connected hexagonal sections.
*/

#pragma once

#include <string>
#include <set>
#include <map>
#include <memory>
#include <functional>

#include "Section.h"
#include "utility/utility.h"

#include "entities/AllEntities.h"

namespace questless
{
	class Game;

	class Region
	{
	public:
		/// Pseudo-randomly generates a new region.
		/// @param name The name of the new region.
		Region(std::string region_name);

		/// Loads a region from disk.
		/// @param save_name The name of the region's save file.
		/// @param region_name The name of the region.
		Region(char const* save_name, std::string region_name);

		/// @return The region's name, as it appears in game and on disk.
		std::string name() const { return _name; }

		/// Saves the region to disk.
		/// @param save_name The name of the region's save file.
		void save(char const* save_name);

		/// Removes the given being from the turn queue if it's there.
		/// @param being A being.
		void remove_from_turn_queue(Being& being) { if (being.busy_time <= 0.0) _turn_queue.erase(being); }

		/// Adds the given being to the turn queue if it's not busy.
		/// @param being A being.
		void add_to_turn_queue(Being& being) { if (being.busy_time <= 0.0) _turn_queue.insert(being); }

		/// @return The being whose turn it is to act or null if none are ready.
		Being* next_ready_being();

		/// @return The list of beings in the section at the given section coordinates.
		////
		std::vector<Being::ref> beings(RegionSectionCoords section_coords) const
		{
			return _section_map.find(section_coords)->second->beings();
		}

		/// @return The list of objects in the section at the given section coordinates.
		////
		std::vector<Object::ref> objects(RegionSectionCoords section_coords) const
		{
			return _section_map.find(section_coords)->second->objects();
		}

		/// @return The ID of the being at the given tile coordinates or nullopt if none.
		////
		std::optional<Id<Being>> being_id(RegionTileCoords tile_coords) const;

		/// @return The ID of the object at the given tile coordinates or nullopt if none.
		////
		std::optional<Id<Object>> object_id(RegionTileCoords tile_coords) const;

		/// @return The being at the given tile coordinates or nullptr if none.
		////
		Being* being(RegionTileCoords tile_coords) const;

		/// @return The object at the given tile coordinates or nullptr if none.
		////
		Object* object(RegionTileCoords tile_coords) const;

		/// Spawns the given player-controlled being in the region at an arbitrary location and adds it to the being cache.
		////
		void spawn_player(Being::uptr player_being);

		/// Adds the given existing being to the region, setting its coordinates to those given.
		////
		void add(Being& being, RegionTileCoords coords);

		/// Adds the given existing object to the region, setting its coordinates to those given.
		////
		void add(Object& object, RegionTileCoords coords);

		/// Spawns the given being in the region, setting its coordinates to those given and adding it to the being cache.
		////
		void spawn(Being::uptr being, RegionTileCoords coords);

		/// Spawns the given object in the region, setting its coordinates to those given and adding it to the object cache.
		////
		void spawn(Object::uptr object, RegionTileCoords coords);

		/// Moves the given being to the given coordinates.
		/// @param being The being to move.
		/// @param coords The being's new coordinates.
		/// @return Whether the being successfully moved.
		bool move(Being& being, RegionTileCoords coords);

		/// Moves the given entity to the given coordinates.
		/// @param object The object to move.
		/// @param coords The object's new coordinates.
		/// @return Whether the object successfully moved.
		bool move(Object& object, RegionTileCoords coords);

		/// Removes the given being from the region, if present.
		////
		void remove(Being& being);

		/// Removes the given object from the region, if present.
		////
		void remove(Object& object);

		/// @todo I think section_exists() may be unnecessary. Can probably just use section() everywhere.

		/// @return Whether there is a section at the given section coordinates.
		bool section_exists(RegionSectionCoords section_coords) const;

		/// @return The section at the given section coordinates.
		Section* section(RegionSectionCoords section_coords);
		/// @return The section at the given section coordinates.
		Section const* section(RegionSectionCoords section_coords) const;

		/// Gets the coordinates of the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return Hex coordinates of the containing section.
		RegionSectionCoords containing_section_coords(RegionTileCoords tile_coords) const
		{
			/// @todo Figure out how to do this without the ugly casting to/from floating-point.
			return RegionSectionCoords{lround(1.0 * tile_coords.q / Section::diameter), lround(1.0 * tile_coords.r / Section::diameter)};
		}

		/// Gets the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return The containing section.
		Section& containing_section(RegionTileCoords tile_coords) { return *_section_map[containing_section_coords(tile_coords)]; }
		/// Gets the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return The containing section.
		Section const& containing_section(RegionTileCoords tile_coords) const { return *_section_map.find(containing_section_coords(tile_coords))->second; }

		/// Gets the light level at the given hex coordinates.
		/// @param tile_coords Hex coordinates in the region.
		/// @return The light level at those coordinates.
		double light_level(RegionTileCoords tile_coords) const { return containing_section(tile_coords).light_level(tile_coords); }

		/// Gets the temperature at the given hex coordinates.
		/// @param tile_coords Hex coordinates in the region.
		/// @return The temperature at those coordinates.
		double temperature(RegionTileCoords tile_coords) const { return containing_section(tile_coords).temperature(tile_coords); }

		/// Updates everything contained in the region.
		void update();
	private:
		// Define the maximum size of the sections rhomboid that should be loaded at any given time.
		static const int _loaded_sections_q_radius = 1;
		static const int _loaded_sections_r_radius = 1;

		std::string _name;
		std::map<RegionSectionCoords, std::unique_ptr<Section>> _section_map; /// @todo Replace unique_ptr with reference_wrapper (disallow null sections).
		RegionSectionCoords center_section_coords = RegionSectionCoords{0, 0};

		std::set<Being::ref, Being::ref_less_t> _turn_queue;

		/// Performs some operation on each section in the loaded rhomboid of sections.
		/// @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(Section&)> f);
	};
}
