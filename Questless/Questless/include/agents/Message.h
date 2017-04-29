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

	using MessageMutableVisitor = Visitor
		< MessageArrowMiss
		, MessageCannotEquip
		, MessageArrowMiss
		, MessageCannotEquip
		, MessageEntityInTheWay
		, MessageIncantFailedMute
		, MessageMeleeMiss
		, MessageNotEnoughAmmo
		, MessageNotEnoughMana
		, MessageSpellNotEnoughCharges
		, MessageSpellOnCooldown
		>;

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
	struct Message : public Element<MessageMutableVisitor, MessageConstVisitor>
	{
		virtual ~Message() = default;
	};

	DEFINE_ELEMENT_BASE(Message, Message)

	struct MessageArrowMiss : MessageBase<MessageArrowMiss> {};
	struct MessageCannotEquip : MessageBase<MessageCannotEquip> {};
	struct MessageEntityInTheWay : MessageBase<MessageEntityInTheWay> {};
	struct MessageIncantFailedMute : MessageBase<MessageIncantFailedMute> {};
	struct MessageMeleeMiss : MessageBase<MessageMeleeMiss> {};
	struct MessageNotEnoughAmmo : MessageBase<MessageNotEnoughAmmo> {};
	struct MessageNotEnoughMana : MessageBase<MessageNotEnoughMana>
	{
		double mana_deficit;
		MessageNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
	struct MessageSpellNotEnoughCharges : MessageBase<MessageSpellNotEnoughCharges> {};
	struct MessageSpellOnCooldown : MessageBase<MessageSpellOnCooldown>
	{
		double active_cooldown;
		MessageSpellOnCooldown(double active_cooldown) : active_cooldown{active_cooldown} {}
	};
}
