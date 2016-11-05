/**
* @file    Effect.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Represents an in-game effect perceivable by beings, such as lighting and sound effects.
*/

#ifndef EFFECT_H
#define EFFECT_H

#include <memory>

#include "EffectVisitor.h"

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
}

#endif
