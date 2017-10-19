//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

namespace ql
{
	//! The agent representing the player's control over his or her character.
	class player : public agent
	{
	public:
		player(ql::being& being)
			: agent{being}
			, _world_view{nullptr}
		{}

		//! The player's current view of the world.
		world_view const& world_view() const { return *_world_view; }

		void update_world_view() { _world_view = std::make_unique<ql::world_view>(being, true); }

		void act() final;

		void perceive(sptr<effect> const& effect) final;

		//! Gets a list of perceived effects and removes them from the player agent.
		//! @return All the effects the player has perceived since the last call to poll_perceived_effects().
		std::vector<sptr<effect>> poll_perceived_effects();

		// Queries and messages

		complete send_message
			( uptr<message> message
			, std::function<complete()> cont
			) const final;

		complete query_count
			( uptr<count_query> query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<complete(std::optional<int>)> cont
			) const final;

		complete query_magnitude
			( uptr<magnitude_query> query
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<complete(std::optional<double>)> cont
			) const final;

		complete query_tile
			( uptr<tile_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const final;

		complete query_direction
			( uptr<direction_query> query
			, std::function<complete(std::optional<region_tile::direction>)> cont
			) const final;

		complete query_vector
			( uptr<vector_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const final;

		complete query_being
			( uptr<being_query> query
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<ql::being*>)> cont
			) const final;

		complete query_item
			( uptr<item_query> query
			, ql::being& source
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<item*>)> cont
			) const final;

		// Quick Time Events

		complete get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) const final;

		complete incant(gatestone& gatestone, std::function<complete(uptr<magic::spell>)> cont) const final;
	private:
		uptr<ql::world_view> _world_view;

		std::vector<sptr<effect>> _perceived_effects;
	};
}
