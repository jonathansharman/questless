//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"
#include "effects/effect_visitor.hpp"
#include "utility/id.hpp"

namespace ql {
	//! A basic AI, suitable for most NPCs.
	class basic_ai : public agent, public effect_const_visitor {
	public:
		basic_ai(ql::being& being) : agent{being} {}

		complete act() override;

		void perceive(sptr<effect> const& effect) override;

		// Queries and messages

		complete send_message
			( queries::message::any message
			, std::function<complete()> cont
			) const override;

		complete query_count
			( queries::count::any query
			, int default_value
			, std::optional<int> min
			, std::optional<int> max
			, std::function<complete(std::optional<int>)> cont
			) const override;

		complete query_magnitude
			( queries::magnitude::any query
			, double default_value
			, std::optional<double> min
			, std::optional<double> max
			, std::function<complete(std::optional<double>)> cont
			) const override;

		complete query_tile
			( queries::tile::any query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::point)> predicate
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const override;

		//complete query_direction
		//	( queries::direction::any query
		//	, std::function<complete(std::optional<region_tile::direction>)> cont
		//	) const override;

		complete query_vector
			( queries::vector::any query
			, std::optional<region_tile::point> origin
			, std::function<bool(region_tile::vector)> predicate
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const override;

		complete query_being
			( queries::being::any query
			, std::function<bool(ql::being&)> predicate
			, std::function<complete(std::optional<ql::being*>)> cont
			) const override;

		//complete query_item
		//	( queries::item::any query
		//	, ql::being& source
		//	, std::function<bool(ql::being&)> predicate
		//	, std::function<complete(std::optional<item*>)> cont
		//	) const override;

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(arrow_attack_effect const&) override {} // Ignore.
		void visit(eagle_eye_effect const&) override {} // Ignore.
		void visit(injury_effect const& effect) override;
		void visit(lightning_bolt_effect const&) override {} // Ignore.
	private:
		friend class state;
		struct state {
			virtual ~state() = default;
			virtual complete act(basic_ai& ai) = 0;
		};
		struct idle_state : public state { complete act(basic_ai& ai) override; };
		struct walk_state : public state { complete act(basic_ai& ai) override; };
		struct attack_state : public state {
			id<ql::being> target_id;
			attack_state(id<ql::being> target_id) : target_id{target_id} {}
			complete act(basic_ai& ai) override;
		};

		uptr<state> _state = umake<idle_state>();
	};
}
