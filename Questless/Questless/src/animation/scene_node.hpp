//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "utility/reference.hpp"

#include <range/v3/view/reverse.hpp>

#include <deque>

namespace ql {
	//! An animation with attached child animations. A scene node's transform is composed with its children's transforms.
	struct scene_node : animation {
		//! The animation to draw at this node.
		uptr<animation> animation;

		//! Children drawn behind this node. Drawn back-to-front, so back_children.back() is the back-most node.
		std::deque<uptr<ql::animation>> back_children{};

		//! Children drawn in front of this node. Drawn back-to-front, so front_children.front() is the front-most node.
		std::deque<uptr<ql::animation>> front_children{};

		//! @param animation The animation to draw at this node.
		scene_node(uptr<ql::animation> animation) : animation{std::move(animation)} {}

	private:
		void animation_subupdate(sec elapsed_time) final {
			animation->update(elapsed_time);
			if (animation->stopped()) {
				stop();
				return;
			}
			auto update_children = [&](auto& children) {
				for (auto it = children.begin(); it != children.end();) {
					(*it)->update(elapsed_time * animation->time_scale.value());
					if ((*it)->stopped()) {
						it = children.erase(it);
					} else {
						++it;
					}
				}
			};
			update_children(back_children);
			update_children(front_children);
		}

		void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final {
			for (auto& child : back_children | ranges::view::reverse) {
				target.draw(*child, states);
			}
			if (animation) { target.draw(*animation, states); }
			for (auto& child : front_children | ranges::view::reverse) {
				target.draw(*child, states);
			}
		}
	};
}
