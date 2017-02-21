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
	struct MessageMeleeMiss;
	struct MessageNotEnoughMana;
	struct MessageSpellOutOfCharges;
	struct MessageSpellOnCooldown;
	struct MessageIncantFailedMute;

	struct MessageVisitor
	{
		virtual ~MessageVisitor() = default;

		virtual void visit(MessageMeleeMiss const&) = 0;
		virtual void visit(MessageNotEnoughMana const&) = 0;
		virtual void visit(MessageSpellOutOfCharges const&) = 0;
		virtual void visit(MessageSpellOnCooldown const&) = 0;
		virtual void visit(MessageIncantFailedMute const&) = 0;
	};

	struct Message
	{
		using ptr = std::unique_ptr<Message>;
		virtual ~Message() = default;
		virtual void accept(MessageVisitor& visitor) = 0;
	};
	struct MessageMeleeMiss : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageNotEnoughMana : Message
	{
		double mana_deficit;
		MessageNotEnoughMana(double mana_deficit) : mana_deficit{mana_deficit} {}
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageSpellOutOfCharges : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageSpellOnCooldown : Message
	{
		double active_cooldown;
		MessageSpellOnCooldown(double active_cooldown) : active_cooldown{active_cooldown} {}
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MessageIncantFailedMute : Message
	{
		void accept(MessageVisitor& visitor) override { visitor.visit(*this); }
	};
}
