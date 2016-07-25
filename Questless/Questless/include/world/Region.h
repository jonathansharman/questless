/**
* @file    Region.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Region class, which represents a set of connected hexagonal sections.
*/

#ifndef REGION_H
#define REGION_H

#include <string>
#include <set>
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
		/// @param game The game object.
		/// @param name The name of the new region.
		Region(Game& game, std::string region_name);

		/// Loads a region from disk.
		/// @param game The game object.
		/// @param save_name The name of the region's save file.
		/// @param region_name The name of the region.
		Region(Game& game, const std::string& save_name, const std::string& region_name);

		/// @return The region's name, as it appears in game and on disk.
		std::string name() const { return _name; }

		/// Saves the region to disk.
		/// @param save_name The name of the region's save file.
		void save(const std::string& save_name);

		/// Removes the given being from the turn queue if it's there.
		/// @param being A being.
		void remove_from_turn_queue(Being& being) { if (being.busy_time() <= 0.0) _turn_queue.erase(being); }

		/// Adds the given being to the turn queue if it's not busy.
		/// @param being A being.
		void add_to_turn_queue(Being& being) { if (being.busy_time() <= 0.0) _turn_queue.insert(being); }

		/// @return The being whose turn it is to act or null if none are ready.
		Being* next_ready_being();

		/// @param section_coords Section coordinates within the region.
		/// @return The list of beings in the section at the given section coordinates.
		const std::vector<Being::ptr>& beings(RegionSectionCoords section_coords) const { return _section_map.find(section_coords)->second->beings(); }

		/// @param section_coords Section coordinates within the region.
		/// @return The list of objects at the given section coordinates.
		const std::vector<Object::ptr>& objects(RegionSectionCoords section_coords) const { return _section_map.find(section_coords)->second->objects(); }

		/// @param tile_coords Tile coordinates within the region.
		/// @return The list of beings at the given tile coordinates.
		std::vector<Being::ref> beings(RegionTileCoords tile_coords) const;

		/// @param tile_coords Tile coordinates within the region.
		/// @return The list of objects at the given tile coordinates.
		std::vector<Object::ref> objects(RegionTileCoords tile_coords) const;

		/// Inserts the given player being into the region at an arbitrary location, as its spawn point at the start of the game.
		/// @param player_being The being to insert into the region.
		void spawn_player(Being::ptr player_being);

		/// Adds an entity of the given type to the appropriate section of the region, based on the given coordinates, and set its internal coordinates to those given.
		/// @param entity An entity to be added.
		/// @param coords The entity's coordinates in the region.
		/// @tparam EntityType The type of entity to add. Legal values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity, RegionTileCoords coords)
		{
			RegionSectionCoords section_coords = containing_section_coords(coords);
			auto it = _section_map.find(section_coords);
			if (it != _section_map.end()) {
				Section* section = it->second.get();
				entity->region(this);
				entity->section(section);
				entity->coords(coords);

				/// @todo Find a more permanent solution to this switch on type. (Probably need to write different add methods for beings and objects unless I change how the turn queue works and add generic accessors for game.beings() and game.objects().)
				if (Being* being = dynamic_cast<Being*>(entity.get())) {
					add_to_turn_queue(dynamic_cast<Being&>(*entity));
					_game.add_being(being, GlobalCoords{_name, section->coords()});
				} else {
					Object* object = dynamic_cast<Object*>(entity.get());
					_game.add_object(object, GlobalCoords{_name, section->coords()});
				}

				section->add<EntityType>(std::move(entity));
			} else {
				throw std::out_of_range("No section at given coordinates in region.");
			}
		}

		/// Adds an entity to the appropriate section of the region, based on its internal coordinates.
		/// @param entity An entity to be added.
		/// @tparam EntityType The type of entity to add. Possible values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity) { add<EntityType>(std::move(entity), entity->coords()); }

		/// Moves the given being to the given coordinates.
		/// @param being The being to move.
		/// @param coords The being's new coordinates.
		void move(Being& being, RegionTileCoords coords);

		/// Moves the given entity to the given coordinates.
		/// @param object The object to move.
		/// @param coords The object's new coordinates.
		void move(Object& object, RegionTileCoords coords);

		/// Removes a being from the region and the game.
		/// @param being A being to be removed.
		/// @return The removed being.
		Being::ptr remove(Being& being);

		/// Removes an object from the region and the game.
		/// @param object An object to be removed.
		/// @return The removed object.
		Object::ptr remove(Object& object);

		/// @return Whether there is a section at the given section coordinates.
		bool section_exists(RegionSectionCoords section_coords) const;

		/// @return The section at the given section coordinates.
		const Section& section(RegionSectionCoords section_coords) const;

		/// Gets the coordinates of the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return Hex coordinates of the containing section.
		RegionSectionCoords containing_section_coords(RegionTileCoords tile_coords) const
		{
			/// @todo Figure out how to do this without the ugly casting to/from floating-point.
			return RegionSectionCoords{{lround(tile_coords.hex.q / double{section_diameter}), lround(tile_coords.hex.r / double{section_diameter})}};
		}

		/// Gets the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return The containing section.
		Section& containing_section(RegionTileCoords tile_coords) { return *_section_map[containing_section_coords(tile_coords)]; }
		/// Gets the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return The containing section.
		const Section& containing_section(RegionTileCoords tile_coords) const { return *_section_map.find(containing_section_coords(tile_coords))->second; }

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

		Game& _game;
		std::string _name;
		std::map<RegionSectionCoords, std::unique_ptr<Section>> _section_map;
		RegionSectionCoords center_section_coords = RegionSectionCoords{{0, 0}};

		std::set<Being::ref, Being::ref_less_t> _turn_queue;

		/// Performs some operation on each section in the loaded rhomboid of sections.
		/// @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(Section&)> f);
	};
}

#endif
