//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	struct MessageArrowMiss;
	struct MessageCannotEquip;
	struct MessageArrowMiss;
	struct MessageCannotEquip;
	struct MessageEntityInTheWay;
	struct MessageIncantFailedMute;
	struct MessageIncantGatestoneNotEnoughMana;
	struct MessageMeleeMiss;
	struct MessageNotEnoughAmmo;
	struct MessageNotEnoughMana;

	using MessageSubtypeList = type_list::of_t
		< MessageArrowMiss
		, MessageCannotEquip
		, MessageArrowMiss
		, MessageCannotEquip
		, MessageEntityInTheWay
		, MessageIncantFailedMute
		, MessageIncantGatestoneNotEnoughMana
		, MessageMeleeMiss
		, MessageNotEnoughAmmo
		, MessageNotEnoughMana
		>;

	DEFINE_VISITORS(Message, MessageSubtypeList)

	//! A message that can be sent to an agent.
	struct Message : public Element<MessageSubtypeList>
	{
		virtual ~Message() = default;
	};

	DEFINE_ELEMENT_BASE(Message, Message)

	struct MessageArrowMiss : MessageBase<MessageArrowMiss> {};
	struct MessageCannotEquip : MessageBase<MessageCannotEquip> {};
	struct MessageEntityInTheWay : MessageBase<MessageEntityInTheWay> {};
	struct MessageIncantFailedMute : MessageBase<MessageIncantFailedMute> {};
	struct MessageIncantGatestoneNotEnoughMana : MessageBase<MessageIncantGatestoneNotEnoughMana>
	{
		double mana_deficit;
		MessageIncantGatestoneNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
	struct MessageMeleeMiss : MessageBase<MessageMeleeMiss> {};
	struct MessageNotEnoughAmmo : MessageBase<MessageNotEnoughAmmo> {};
	struct MessageNotEnoughMana : MessageBase<MessageNotEnoughMana>
	{
		double mana_deficit;
		MessageNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
	};
}
