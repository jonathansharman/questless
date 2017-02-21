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
	Action::Complete ModalWeapon::SwitchForm::perform(Being& actor, cont_t cont)
	{
		/// @todo This.

		/// @todo How to query for the desired form in a general manner?
		//actor.agent().query_list();
		actor.busy_time += _weapon.switch_time();
		return cont(Result::success);
	}
}
