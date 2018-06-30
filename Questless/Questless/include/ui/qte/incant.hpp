//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"
#include "units/game_space.hpp"

namespace ql
{
	class gatestone;
	namespace magic
	{
		class spell;
	}
}

namespace ql::qte
{
	//! Quick time event for incanting a spell.
	class incant : public dialog
	{
	public:
		//! @param gatestone The gatestone used for incantation.
		//! @param cont The dialog continuation function.
		incant(gatestone& gatestone, std::function<void(uptr<magic::spell>)> cont);

		state update() final;

		void draw() const final;
	private:
		enum class note { left, right, up, down };
		enum class side { left, right };

		static constexpr double time_factor = 4.0;

		gatestone& _gatestone;
		continuation<uptr<magic::spell>> _cont;

		side _side = side::right;

		bool _tick = false;

		bool _begun = false;

		units::game_space::seconds _elapsed_time = 0.0s;

		std::vector<note> _notes;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void load_textures();
	};
}
