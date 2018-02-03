//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

namespace ql
{
	//! A basic AI, suitable for most NPCs.
	class basic_ai : public agent, public effect_const_visitor
	{
	public:
		basic_ai(ql::being& being) : agent{being} {}

		void act() override;

		void perceive(sptr<effect> const& effect) override { effect->accept(*this); }

		// Queries and messages

		complete send_message
			( uptr<message> message
			, std::function<complete()> cont
			) const override;

		complete query_count
			( uptr<count_query> query
			, int default_value
			, std::optional<int> min
			, std::optional<int> max
			, std::function<complete(std::optional<int>)> cont
			) const override;

		complete query_magnitude
			( uptr<magnitude_query> query
			, double default_value
			, std::optional<double> min
			, std::optional<double> max
			, std::function<complete(std::optional<double>)> cont
			) const override;

		complete query_tile
			( uptr<tile_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const override;

		complete query_direction
			( uptr<direction_query> query
			, std::function<complete(std::optional<region_tile::direction>)> cont
			) const override;

		complete query_vector
			( uptr<vector_query> query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const override;

		complete query_being
			( uptr<being_query> query
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<ql::being*>)> cont
			) const override;

		complete query_item
			( uptr<item_query> query
			, ql::being& source
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<item*>)> cont
			) const override;

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(eagle_eye_effect const&) override {} // Ignore.
		void visit(injury_effect const& effect) override;
		void visit(lightning_bolt_effect const&) override {} // Ignore.
	private:
		friend class state;
		struct state
		{
			virtual ~state() = default;
			virtual void act(basic_ai& ai) = 0;
		};
		struct idle_state : public state { void act(basic_ai& ai) override; };
		struct walk_state : public state { void act(basic_ai& ai) override; };
		struct attack_state : public state
		{
			id<ql::being> target_id;
			attack_state(id<ql::being> target_id) : target_id{target_id} {}
			void act(basic_ai& ai) override;
		};

		uptr<state> _state = umake<idle_state>();
	};
}
