/**
* @file    PlayerActionDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the PlayerActionDialog class.
*/

#pragma once

#include <boost/optional.hpp>

#include "Dialog.h"
#include "HUDController.h"
#include "utility/utility.h"

namespace questless
{
	class PlayerActionDialog : public Dialog
	{
	public:
		struct Choice
		{
			enum class Type { idle, walk, use };

			Type type;
			int data;

			Choice(Type type = Type::idle, int data = 0) : type{type}, data{data} {}
		};

		PlayerActionDialog(HUDController& hud, std::function<void(Choice)> cont) : _hud{hud}, _cont{std::move(cont)} {}

		State update() override;

		void draw() const override
		{}
	private:
		HUDController& _hud;
		Continuation<Choice> _cont;

		void refresh() override {}
	};
}
