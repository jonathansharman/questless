//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../Dialog.h"
#include "animation/Camera.h"
#include "spell/Spell.h"
#include "units/GameSpace.h"
#include "units/GameSeconds.h"
#include "units/GameVelocity.h"

namespace questless
{
	class Gatestone;
}

namespace questless::qte
{
	//! Quick time event for incanting a spell.
	class Incant : public Dialog
	{
	public:
		//! @param gatestone The gatestone used for incantation.
		//! @param cont The dialog continuation function.
		Incant(Gatestone& gatestone, std::function<void(uptr<spell::Spell>)> cont);

		State update() final;

		void draw() const final;
	private:
		enum class Note { left, right, up, down };
		enum class Side { left, right };

		static constexpr double time_factor = 4.0;

		Gatestone& _gatestone;
		Continuation<uptr<spell::Spell>> _cont;

		Side _side = Side::right;

		bool _tick = false;

		bool _begun = false;

		units::GameSeconds _elapsed_time = 0.0s;

		std::vector<Note> _notes;

		uptr<sdl::Texture> _txt_title;
		uptr<sdl::Texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
