//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body.hpp"

#include "entities/beings/being.hpp"
#include "entities/beings/body_part.hpp"
#include "game.hpp"
#include "utility/utility.hpp"

#include <numeric>
#include <queue>

using namespace vecx;

namespace ql {
	body::body(id<being> owner_id, body_part root)
		: blood{0.0_blood,
			  [] { return 0.0_blood; },
			  [owner_id] {
				  being const& owner = the_game().beings.cref(owner_id);
				  return owner.body.total_vitality() * body_part::blood_per_vitality;
			  }}
		, _owner_id{owner_id}
		, _root{std::move(root)}
		, _total_vitality{0.0_hp} //
	{
		// Walk the parts tree to build the parts lists and compute cumulative values.
		std::queue<ref<body_part>> work_list;
		work_list.push(_root);
		while (!work_list.empty()) {
			body_part& part = work_list.front();
			_parts.push_back(part);
			_c_parts.push_back(part);

			// Add part values to totals.
			_total_vitality += part.stats.a.vitality.value();

			// Remove current part from work list and add its children.
			work_list.pop();
			for (auto const& attachment : part.attachments) {
				if (attachment->part) { work_list.push(*attachment->part); }
			}
		}

		// Start with maximum blood.
		blood = _total_vitality * body_part::blood_per_vitality;
	}

	body_part const* body::find_part(id<body_part> id) const {
		auto const it = std::find_if(_parts.begin(), _parts.end(), [id](body_part const& p) { return p.id == id; });
		return it == _parts.end() ? &it->get() : nullptr;
	}
	body_part* body::find_part(id<body_part> id) {
		return const_cast<body_part*>(const_cast<body const&>(*this).find_part(id));
	}

	void body::update(tick elapsed) {
		being const& owner = the_game().beings.cref(_owner_id);

		// Update cumulative values.
		_total_vitality = std::accumulate(_parts.begin(), _parts.end(), 0.0_hp, [](health acc, body_part const& p) {
			return acc + p.stats.a.vitality.value();
		});

		// Bleed and regenerate blood.
		for (body_part const& part : _parts) {
			blood += (owner.stats.regen.value() * part.stats.a.vitality.value() * body_part::blood_per_vitality -
						 part.bleeding.value()) *
					 elapsed;
		}

		// Update parts.
		for (body_part& part : _parts) {
			part.update(elapsed);
		}
	}

	uptr<animation> get_animation(body const& body) {
		std::unordered_multimap<int, sf::ConvexShape> colored_hitboxes;

		auto get_color = [](ql::body_part const& part) {
			float const pct_health = static_cast<float>((part.health.value() / part.stats.a.vitality.value()).value);
			constexpr float threshold = 0.5f;
			bool const below_threshold = pct_health < threshold;
			sf::Uint8 const red = below_threshold ? 255 : to_uint8(1 - (pct_health - threshold) / (1.0f - threshold));
			sf::Uint8 const green = below_threshold ? to_uint8(pct_health / threshold) : 255;
			return sf::Color{red, green, 0};
		};

		std::function<void(ql::body_part const&, view::vector, radians)> traverse =
			[&](ql::body_part const& part, view::vector parent_offset, radians parent_rotation) {
				// Adjust hitbox and extend body bounds.
				auto hitbox = part.hitbox;
				sf::ConvexShape shape;
				shape.setFillColor(get_color(part));
				shape.setOutlineColor(sf::Color::Black);
				shape.setOutlineThickness(1);
				for (auto& vertex : hitbox.vertices) {
					vertex.rotate(view::point{0.0_view_length, 0.0_view_length}, parent_rotation);
					vertex += parent_offset;
				}

				// Color hitbox and add to layer-sorted map.
				colored_hitboxes.insert(std::make_pair(part.layer, hitbox));
				for (auto const& attachment : part.attachments) {
					if (attachment->part) {
						traverse(*attachment->part,
							attachment->offset.rotated(parent_rotation) + parent_offset,
							attachment->rotation + parent_rotation);
					}
				}
			};
		traverse(body.root(), view::vector::zero(), 0.0_rad);

		auto const texture_color = sf::Color{0, 255, 255, 128};
		auto result = umake<media::texture>(texture_size, texture_color);
		result->as_target([&] {
			auto const offset_to_center = view::point{0.0_view_length, 0.0_view_length} - data.bounds.position;
			for (auto& kv : data.colored_hitboxes) {
				auto [hitbox, color] = kv.second;
				for (auto& vertex : hitbox.vertices) {
					vertex += offset_to_center;
				}
				the_renderer().draw_polygon(hitbox, 2.0_view_length, sf::Color::Black, color);
			}
		});
	}
}
