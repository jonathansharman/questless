/**
* @file    ModalWeapon.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the ModalWeapon abstract base class.
*/

#include "items/weapons/ModalWeapon.h"
#include "entities/beings/Being.h"
#include "Game.h"

namespace questless
{
	Action::Complete ModalWeapon::SwitchForm::perform(Being& /*actor*/, cont_t cont)
	{
		/// @todo This.

		/// @todo Add list querying to agent queries.
		//actor.agent().query_list();
		//actor.gain_busy_time(_weapon.switch_time());
		return cont(Result::success);
	}
}
