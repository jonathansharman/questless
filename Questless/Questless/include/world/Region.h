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
		enum class PeriodOfDay { morning, afternoon, dusk, evening, night, dawn };

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
		void spawn_player(uptr<Being> player_being);

		//! Adds @p being to the region, setting its coordinates to @p region_tile_coords.
		void add(Being& being, RegionTileCoords region_tile_coords);

		//! Adds @p object to the region, setting its coordinates to @p region_tile_coords.
		void add(Object& object, RegionTileCoords region_tile_coords);

		//! Spawns @p being in the region, setting its coordinates to @p region_tile_coords and adding it to the being cache.
		void spawn(uptr<Being> being, RegionTileCoords region_tile_coords);

		//! Spawns @p object in the region, setting its coordinates to @p region_tile_coords and adding it to the object cache.
		void spawn(uptr<Object> object, RegionTileCoords region_tile_coords);

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

		//! Adds @p light_source to the region's light sources.
		void add(LightSource const& light_source);

		//! Adds @p light_source from the region's light sources, if present.
		void remove(LightSource const& light_source);

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

		//! The total in-game time in this region.
		double time() const { return _time; }

		//! The time of day in this region.
		double time_of_day() const { return _time_of_day; }

		//! The period of day in this region.
		PeriodOfDay period_of_day() const { return _period_of_day; }

		//! The illuminance of the tile at @p region_tile_coords.
		double illuminance(RegionTileCoords region_tile_coords) const;

		//! The temperature of the tile at @p region_tile_coords.
		double temperature(RegionTileCoords region_tile_coords) const;

		//! Advances local time by one unit then updates everything contained in the region.
		void update();
	private:
		///////////////
		// Constants //
		///////////////

		// These define the maximum size of the sections rhomboid that should be loaded at any given time.

		static int const _loaded_sections_q_radius = 1;
		static int const _loaded_sections_r_radius = 1;

		// Time Constants

		static double constexpr _day_length = 1200.0;
		static double constexpr _twilight_percentage = 0.05;
		static double constexpr _end_of_morning = 0.25 * _day_length;
		static double constexpr _end_of_afternoon = 0.5 * _day_length;
		static double constexpr _end_of_dusk = (0.5 + _twilight_percentage) * _day_length;
		static double constexpr _end_of_evening = (2.0 / 3.0) * _day_length;
		static double constexpr _end_of_night = (1.0 - _twilight_percentage) * _day_length;

		/////////////////
		// Member Data //
		/////////////////

		std::string _name;
		std::map<RegionSectionCoords, std::unique_ptr<Section>> _section_map; //! @todo Replace unique_ptr with reference_wrapper (disallow null sections).
		RegionSectionCoords center_section_coords = RegionSectionCoords{0, 0};

		double _time;
		double _time_of_day;
		PeriodOfDay _period_of_day;

		std::set<ref<Being>, Being::ref_less_t> _turn_queue;
		double _ambient_illuminance;

		//////////////////////
		// Member Functions //
		//////////////////////

		double get_time_of_day() const { return fmod(_time, _day_length); }
		PeriodOfDay get_period_of_day() const;

		double get_ambient_illuminance();

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
