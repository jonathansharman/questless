//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Agent.h"

namespace questless
{
	//! A basic AI, suitable for most NPCs.
	class BasicAI : public Agent, EffectVisitor
	{
	public:
		BasicAI(Being& being) : Agent{being} {}

		void act() override;

		void perceive(Effect::uptr const& effect) override { effect->accept(*this); }

		// Queries and messages

		Complete send_message
			( Message::uptr message
			, std::function<Complete()> cont
			) const override;

		Complete query_count
			( CountQuery::uptr query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Complete(std::optional<int>)> cont
			) const override;

		Complete query_magnitude
			( MagnitudeQuery::uptr query
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Complete(std::optional<double>)> cont
			) const override;

		Complete query_tile
			( TileQuery::uptr query
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Complete(std::optional<RegionTileCoords>)> cont
			) const override;

		Complete query_direction
			( DirectionQuery::uptr query
			, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
			) const override;

		Complete query_being
			( BeingQuery::uptr query
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Being*>)> cont
			) const override;

		Complete query_item
			( ItemQuery::uptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Item*>)> cont
			) const override;

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(EagleEyeEffect const&) override {} // Ignore.
		void visit(InjuryEffect const& effect) override
		{
			// Retaliate against injuries.
			if (effect.opt_source_id && effect.target_id == being.id) {
				_state = std::make_unique<AttackState>(*effect.opt_source_id);
			}
		}
		void visit(LightningBoltEffect const&) override {} // Ignore.
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
