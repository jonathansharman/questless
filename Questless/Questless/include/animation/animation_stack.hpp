//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <vector>

#include "animation.hpp"
#include "utility/reference.hpp"

namespace ql
{
	//! A stack of associated animations. Useful for multi-animation effects, such as a sprite animation combined with a particle animation.
	//!
	//! @note Animations are updated and drawn in the same order they were added to the set.
	//!
	class animation_stack : public animation
	{
	public:
		//! Adds @p animation to the set.
		//! @return A reference to @p animation.
		animation& add(uptr<animation> animation)
		{
			_animations.push_back(std::move(animation));
			return *_animations.back();
		}

		void draw(units::screen_space::point position) const
		{
			for (auto& animation : _animations) {
				animation->draw(position);
			}
		}

		void draw(units::game_space::point position, camera const& camera, units::colors::color_factor color_factor = units::colors::white_factor()) const
		{
			for (auto& animation : _animations) {
				animation->draw(position, camera, color_factor);
			}
		}
	private:
		std::vector<uptr<animation>> _animations;

		void animation_subupdate() final
		{
			for (auto& animation : _animations) {
				animation->update();
			}
		}
	};
}
