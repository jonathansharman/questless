//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	struct MessageArrowMiss;
	struct MessageCannotEquip;
	struct MessageEntityInTheWay;
	struct MessageIncantFailedMute;
	struct MessageMeleeMiss;
	struct MessageNotEnoughAmmo;
	struct MessageNotEnoughMana;
	struct MessageSpellNotEnoughCharges;
	struct MessageSpellOnCooldown;

	using MessageConstVisitor = Visitor
		< MessageArrowMiss const
		, MessageCannotEquip const
		, MessageArrowMiss const
		, MessageCannotEquip const
		, MessageEntityInTheWay const
		, MessageIncantFailedMute const
		, MessageMeleeMiss const
		, MessageNotEnoughAmmo const
		, MessageNotEnoughMana const
		, MessageSpellNotEnoughCharges const
		, MessageSpellOnCooldown const
		>;

	//! A message that can be sent to an agent.
	struct Message : public ConstElement<MessageConstVisitor>
	{
		virtual ~Message() = default;
	};

	DEFINE_CONST_ELEMENT_BASE(Message, Message)

	struct MessageArrowMiss : MessageConstBase<MessageArrowMiss> {};
	struct MessageCannotEquip : MessageConstBase<MessageCannotEquip> {};
	struct MessageEntityInTheWay : MessageConstBase<MessageEntityInTheWay> {};
	struct MessageIncantFailedMute : MessageConstBase<MessageIncantFailedMute> {};
	struct MessageMeleeMiss : MessageConstBase<MessageMeleeMiss> {};
	struct MessageNotEnoughAmmo : MessageConstBase<MessageNotEnoughAmmo> {};
	struct MessageNotEnoughMana : MessageConstBase<MessageNotEnoughMana>
	{
		double mana_deficit;
		MessageNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
	struct MessageSpellNotEnoughCharges : MessageConstBase<MessageSpellNotEnoughCharges> {};
	struct MessageSpellOnCooldown : MessageConstBase<MessageSpellOnCooldown>
	{
		double active_cooldown;
		MessageSpellOnCooldown(double active_cooldown) : active_cooldown{active_cooldown} {}
	};
}
