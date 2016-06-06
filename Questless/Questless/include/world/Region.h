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
#include <filesystem>
namespace fs = std::tr2::sys; /// @todo Replace this with proper using statements if/when TR2 comes around.

#include "Section.h"
#include "utility/utility.h"

#include "entities/AllEntities.h"

namespace questless
{
	class Region
	{
	public:
		/// Pseudo-randomly generates a new region.
		/// @param name The name of the new region.
		Region(std::string region_name);

		/// Loads a region from disk.
		/// @param save_name The name of the region's save file.
		/// @param region_name The name of the region.
		Region(const std::string& save_name, const std::string& region_name);

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
		/// @return The list of beings in the section at the given coordinates.
		const std::vector<Being::ptr>& beings(HexCoords section_coords) const { return _section_map.find(section_coords)->second->beings(); }

		/// @param section_coords Section coordinates within the region.
		/// @return The list of objects at the given coordinates.
		const std::vector<Object::ptr>& objects(HexCoords section_coords) const { return _section_map.find(section_coords)->second->objects(); }

		/// Inserts the given player being into the region at an arbitrary location, as its spawn point at the start of the game.
		/// @param player_being The being to insert into the region.
		void spawn_player(Being::ptr player_being);

		/// @todo Remove draw_terrain() and cache_background() once all the functionality (caching in particular) is moved into the world renderer.

		/// Draws the visible portion of the terrain.
		void draw_terrain(const Camera& camera);

		/// Draws the region's hexes to a single cached background texture for faster drawing.
		/// @param renderer The renderer object to create and render the background.
		void cache_background(sdl::Renderer& renderer);

		/// Adds an entity of the given type to the appropriate section of the region, based on the given coordinates, and set its internal coordinates to those given.
		/// @param entity An entity to be added.
		/// @param coords The entity's coordinates in the region.
		/// @tparam EntityType The type of entity to add. Possible values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity, HexCoords coords)
		{
			HexCoords section_coords = containing_section_coords(coords);
			auto it = _section_map.find(section_coords);
			if (it != _section_map.end()) { /// @todo Deal with case where section not found? Probably not good to silently do nothing.
				entity->region(this);
				entity->section(it->second.get());
				entity->coords(coords);
				it->second->add<EntityType>(std::move(entity));
				/// @todo Find a more permanent solution to this switch on type. (Probably need to write different add methods for beings and objects unless I change how the turn queue works.)
				if (dynamic_cast<Being*>(entity.get())) {
					add_to_turn_queue(dynamic_cast<Being&>(*entity));
				}
			}
		}

		/// Adds an entity to the appropriate section of the region, based on its internal coordinates.
		/// @param entity An entity to be added.
		/// @tparam EntityType The type of entity to add. Possible values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity) { add<EntityType>(std::move(entity), entity->coords()); }

		/// Moves the given entity to the given coordinates, updating the corresponding section entity list as needed.
		/// @param entity The entity to move.
		/// @param coords The entity's new coordinates.
		/// @tparam EntityType The type of entity to move. Possible values are Being and Object.
		template <typename EntityType>
		void move(typename EntityType& entity, HexCoords coords)
		{
			HexCoords src_section_coords = containing_section_coords(entity.coords());
			const unique_ptr<Section>& src_section = _section_map[src_section_coords];
			if (src_section != nullptr) { /// @todo What's up with null sections?
				HexCoords dst_section_coords = containing_section_coords(coords);

				if (dst_section_coords != src_section_coords) {
					const unique_ptr<Section>& dst_section = _section_map[dst_section_coords];
					if (dst_section != nullptr) {
						EntityType::ptr moving_entity = src_section->remove<EntityType>(entity);

						moving_entity->coords(coords);
						dst_section->add<EntityType>(std::move(moving_entity));
					} else {
						/// @todo Need to deal with null destination case.
						return;
					}
				} else {
					entity.coords(coords);
				}
			}
		}

		/// @return Whether there is a section at the given section coordinates.
		bool section_exists(HexCoords section_coords) const;

		/// @return The section at the given section coordinates.
		const Section& section(HexCoords section_coords) const;

		/// @return The entity at the given tile coordinates, or nullptr if there is none.
		Entity* entity(HexCoords tile_coords) const;

		/// Gets the coordinates of the section that contains the tile with the given coordinates.
		/// @param tile_coords The tile's hex coordinates in the region.
		/// @return Hex coordinates of the containing section.
		HexCoords containing_section_coords(HexCoords tile_coords) const
		{
			/// @todo Figure out how to do this without the ugly casting to/from floating-point.
			return HexCoords{lround(tile_coords.q / double{section_diameter}), lround(tile_coords.r / double{section_diameter})};
		}

		/// Gets the light level at the given hex coordinates. If there is no tile at the given coordinates, the base light level for the region is returned.
		/// @param tile_coords Hex coordinates in the region.
		/// @return The light level at those coordinates.
		double light_level(HexCoords tile_coords) const { return 100.0; } /// @todo Implement this.

		void update();
	private:
		// Define the maximum size of the sections rhomboid that should be loaded at any given time.
		static const int _loaded_sections_q_radius = 1;
		static const int _loaded_sections_r_radius = 1;

		std::string _name;
		std::map<HexCoords, std::unique_ptr<Section>> _section_map;
		HexCoords center_section_coords = HexCoords{0, 0};

		std::set<Being::ref, Being::ref_less_t> _turn_queue;

		/// @todo Remove _background once world renderer has all the caching functionality.
		sdl::Texture::ptr _background;

		/// Performs some operation on each section in the loaded rhomboid of sections.
		/// @param f The operation to perform on each section.
		void for_each_loaded_section(std::function<void(Section&)> f);
	};
}

#endif
