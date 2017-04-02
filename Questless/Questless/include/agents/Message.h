/**
* @file    Message.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Messages that can be sent to an agent.
*/

#pragma once

#include <memory>

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

	struct MessageVisitor
	{
		virtual ~MessageVisitor() = default;

		virtual void visit(MessageArrowMiss const&) = 0;
		virtual void visit(MessageCannotEquip const&) = 0;
		virtual void visit(MessageEntityInTheWay const&) = 0;
		virtual void visit(MessageIncantFailedMute const&) = 0;
		virtual void visit(MessageMeleeMiss const&) = 0;
		virtual void visit(MessageNotEnoughAmmo const&) = 0;
		virtual void visit(MessageNotEnoughMana const&) = 0;
		virtual void visit(MessageSpellNotEnoughCharges const&) = 0;
		virtual void visit(MessageSpellOnCooldown const&) = 0;
	};

	struct Message
	{
		using uptr = std::unique_ptr<Message>;
		virtual ~Message() = default;
		virtual void accept(MessageVisitor& visitor) = 0;
	};
	struct MessageArrowMiss : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageCannotEquip : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageEntityInTheWay : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageIncantFailedMute : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageMeleeMiss : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageNotEnoughAmmo : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageNotEnoughMana : Message
	{
		double mana_deficit;
		MessageNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageSpellNotEnoughCharges : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageSpellOnCooldown : Message
	{
		double active_cooldown;
		MessageSpellOnCooldown(double active_cooldown) : active_cooldown{active_cooldown} {}
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
}
