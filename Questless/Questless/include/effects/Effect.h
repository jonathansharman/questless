/**
* @file    Effect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Represents an in-game effect perceivable by beings, such as lighting and sound effects.
*/

#pragma once

#include <memory>

#include "EffectVisitor.h"
#include "entities/beings/Damage.h"

namespace questless
{
	class Effect
	{
	public:
		using ptr = std::shared_ptr<Effect>;

		/// @param origin The coordinates of the effect's origin.
		Effect(RegionTileCoords origin) : _origin{origin} {}

		virtual ~Effect() = default;

		virtual void accept(EffectVisitor& visitor) const = 0;

		/// @return The maximum distance from the origin at which this effect may be perceived.
		virtual int range() const = 0;

		RegionTileCoords origin() const { return _origin; }
	private:
		RegionTileCoords _origin;
	};

	/// @todo Eventually move effect subtypes to individual header files.

	class LightningBoltEffect : public Effect
	{
	public:
		using ptr = std::shared_ptr<LightningBoltEffect>;

		using Effect::Effect;

		/// @param origin The coordinates of the effect's origin.
		static ptr make(RegionTileCoords origin) { return std::make_shared<LightningBoltEffect>(origin); }

		virtual void accept(EffectVisitor& visitor) const override { visitor.visit(*this); }

		int range() const override { return 10; }
	};

	class InjuryEffect : public Effect
	{
	public:
		using ptr = std::shared_ptr<InjuryEffect>;

		/// @param origin The coordinates of the effect's origin.
		/// @param damage The damage dealt.
		/// @param target_id The id of the injured being.
		/// @param opt_source_id The id of the being who caused the injury or nullopt if there is none.
		InjuryEffect(RegionTileCoords origin, Damage damage, BeingId target_id, boost::optional<BeingId> opt_source_id)
			: Effect{origin}, _damage{damage}, _target_id{target_id}, _opt_source_id{opt_source_id}
		{}

		virtual void accept(EffectVisitor& visitor) const override { visitor.visit(*this); }

		int range() const override { return 7; }

		Damage const& damage() const { return _damage; }
		BeingId target_id() const { return _target_id; }
		boost::optional<BeingId> opt_source_id() const { return _opt_source_id; }
	private:
		Damage _damage;
		BeingId _target_id;
		boost::optional<BeingId> _opt_source_id;
	};
}
