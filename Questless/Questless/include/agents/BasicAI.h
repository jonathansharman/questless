/**
* @file    BasicAI.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BasicAI class, an AI suitable for most NPCs.
*/

#pragma once

#include "Agent.h"

namespace questless
{
	class BasicAI : public Agent, EffectVisitor
	{
	public:
		BasicAI(Being& being) : Agent{being} {}

		void act() override;

		void perceive(Effect::ptr const& effect) override { effect->accept(*this); }

		// Queries and messages

		Action::Complete send_message
			( Message::ptr message
			, std::function<Action::Complete()> cont
			) const override;

		Action::Complete query_count
			( CountQuery::ptr query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Action::Complete(std::optional<int>)> cont
			) const override;

		Action::Complete query_magnitude
			( MagnitudeQuery::ptr query
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Action::Complete(std::optional<double>)> cont
			) const override;

		Action::Complete query_tile
			( TileQuery::ptr query
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(std::optional<RegionTileCoords>)> cont
			) const override;

		Action::Complete query_being
			( BeingQuery::ptr query
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(std::optional<Being*>)> cont
			) const override;

		Action::Complete query_item
			( ItemQuery::ptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(std::optional<Item*>)> cont
			) const override;

		// Effect Visits

		virtual void visit(LightningBoltEffect const&) override {} // Ignore.
		virtual void visit(InjuryEffect const& effect) override
		{
			// Retaliate against injuries.
			if (effect.opt_source_id() && effect.target_id() == being.id) {
				_state = std::make_unique<AttackState>(*effect.opt_source_id());
			}
		}
	private:
		friend class State;
		struct State
		{
			virtual ~State() = default;
			virtual void act(BasicAI& ai) = 0;
		};
		struct IdleState : public State { void act(BasicAI& ai) override; };
		struct WalkState : public State { void act(BasicAI& ai) override; };
		struct AttackState : public State
		{
			Id<Being> target_id;
			AttackState(Id<Being> target_id) : target_id{target_id} {}
			void act(BasicAI& ai) override;
		};

		std::unique_ptr<State> _state = std::make_unique<IdleState>();
	};
}
