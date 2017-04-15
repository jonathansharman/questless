/**
* @file    PlayerActionDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the PlayerActionDialog class.
*/

#pragma once

#include <optional>

#include "Dialog.h"
#include "HUD.h"
#include "utility/utility.h"

namespace questless
{
	class PlayerActionDialog : public Dialog
	{
	public:
		struct Choice
		{
			enum class Type { idle, move, shift_move, use };

			Type type;
			int data;

			Choice(Type type = Type::idle, int data = 0) : type{type}, data{data} {}
		};

		PlayerActionDialog(HUD& hud, std::function<void(Choice)> cont) : _hud{hud}, _cont{std::move(cont)} {}

		State update() override;

		void draw() const override
		{}
	private:
		HUD& _hud;
		Continuation<Choice> _cont;
		static int _last_used;

		void refresh() override {}
	};
}
