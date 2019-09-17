//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "animation.hpp"

#include "utility/reference.hpp"

#include <deque>

namespace ql {
	//! An animation with attached child animations. A scene node's transform is composed with its children's transforms.
	struct scene_node : animation {
		//! The animation to draw at this node.
		uptr<animation> node_animation;

		//! Children drawn behind this node. Drawn back-to-front, so back_children.back() is the back-most node.
		std::deque<uptr<animation>> back_children{};

		//! Children drawn in front of this node. Drawn back-to-front, so front_children.front() is the front-most node.
		std::deque<uptr<animation>> front_children{};

		//! @param node_animation The animation to draw at this node.
		scene_node(uptr<animation> node_animation);

	private:
		auto animation_subupdate(sec elapsed_time) -> void final;

		auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
