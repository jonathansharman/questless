/**
* @file    Section.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Section class, which represents a hexagonal collection of hexes in the world.
*/

#ifndef SECTION_H
#define SECTION_H

#include <string>
#include <array>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "entities/beings/Being.h"
#include "entities/objects/Object.h"
#include "utility/constants.h"
#include "utility/hex-utilities.h"
#include "world/Tile.h"
#include "animation/Camera.h"

namespace questless
{
	class Section
	{
	public:
		/// @param region Coordinates relative to the entire region.
		/// @return Positive coordinates relative to the section containing the given region coordinates.
		static HexCoords section_coords(HexCoords region_coords);

		/// Constructs an empty section.
		/// @param coords The positive axial coordinates of the section.
		Section(HexCoords coords);

		/// Creates a new section from the provided data string.
		/// @param data A string representing section data.
		void create(const std::string& data);

		/// Loads the section from the specified file.
		/// @param filename The section's filename.
		void open(const std::string& filename);

		/// Saves the section to the specified file.
		/// @param filename The section's filename.
		void save(const std::string& filename);

		/// Closes the section by setting its hexes to null, releasing its memory.
		/// @note Call save() before close() to write the section data back to file.
		void close();

		/// @return The hex coordinates of the section within the region.
		HexCoords coords() const { return _coords; }

		/// @return The list of beings in this section.
		const std::vector<Being::ptr>& beings() const { return _beings; }

		/// @return The list of objects in this section.
		const std::vector<Object::ptr>& objects() const { return _objects; }

		/// Adds the given entity to the section.
		/// @param being An entity to add.
		/// @tparam EntityType The type of entity to add. Possible values are Being and Object.
		template <typename EntityType>
		void add(typename EntityType::ptr entity) { entities<EntityType>().push_back(std::move(entity)); }

		/// Removes the entity at the given index from the section, if it is of the given subtype.
		/// @param index The index of the entity to be removed.
		/// @return The unique pointer to the removed entity.
		/// @tparam EntityType The type of entity to remove. Possible values are Being and Object.
		template <typename EntityType>
		typename EntityType::ptr remove(size_t index)
		{
			EntityType::ptr removed_entity = std::move(entities<EntityType>()[index]);
			entities<EntityType>()[index] = std::move(entities<EntityType>().back());
			entities<EntityType>().pop_back();
			return removed_entity;
		}

		/// Removes the given entity from the section.
		/// @param entity A reference to the entity to remove.
		/// @return The unique pointer to the removed entity or nullptr if not present.
		/// @tparam EntityType The type of entity to remove. Possible values are Being and Object.
		template <typename EntityType>
		typename EntityType::ptr remove(typename const EntityType& entity)
		{
			for (size_t i = 0; i < entities<EntityType>().size(); ++i) {
				if (entities<EntityType>()[i].get() == &entity) {
					return remove<EntityType>(i);
				}
			}
			return nullptr;
		}

		/// Removes from the section the first entity of the given type for which the given predicate is true.
		/// @param predicate The predicate for determining whether to remove an entity.
		/// @return The unique pointer to the removed entity or nullptr if none matched.
		/// @tparam EntityType The type of entity to remove. Possible values are Being and Object.
		template <typename EntityType>
		typename EntityType::ptr remove(std::function<bool(typename const EntityType::ptr& entity)> predicate)
		{
			for (size_t i = 0; i < entities<EntityType>().size(); ++i) {
				if (predicate(entities<EntityType>()[i])) {
					return remove<EntityType>(i);
				}
			}
			return nullptr;
		}

		/// @return The tile at the given coordinates.
		/// @param tile_coords The tile's hex coordinates, relative to the center of the section.
		const Tile& tile(HexCoords tile_coords) const { return *_tiles[tile_coords.r + section_radius][tile_coords.q + section_radius]; }
	private:
		std::array<std::array<std::unique_ptr<Tile>, section_diameter>, section_diameter> _tiles; ///< An r-major array of tiles, representing a rhomboid section of world data centered around the section's hex coordinates.
		HexCoords _coords; ///< The hex coordinates of the section within the region. The section's center's world coordinates are _coords * section_diameter.

		std::vector<Being::ptr> _beings;
		std::vector<Object::ptr> _objects;

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Being>::value>>
		std::vector<Being::ptr>& entities() { return _beings; }

		template <typename EntityType, typename = typename std::enable_if_t<std::is_same<EntityType, Object>::value>>
		std::vector<Object::ptr>& entities() { return _objects; }

		/// Reads section data from a stream.
		/// @param in The stream from which to read the data.
		void read(std::istream& in);
	};
}

#endif
