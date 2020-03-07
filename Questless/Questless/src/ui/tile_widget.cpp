//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "tile_widget.hpp"

#include "animation/still_shape.hpp"
#include "rsrc/tile.hpp"
#include "utility/unreachable.hpp"
#include "world/terrain.hpp"

namespace ql {
	tile_widget::tile_widget(reg& reg, rsrc::tile const& resources, world_view::tile_view const& tile_view)
		: _reg{&reg}
		, _rsrc{resources}
		, _tv{tile_view} //
	{
		_ani = [&] {
			auto const terrain = reg.get<ql::terrain>(_tv.id);
			sf::ConvexShape cs;
			{ // Add offset points.
				cs.setPointCount(6);
				std::size_t idx = 0;
				for (auto const& offset : tile_layout.corner_offsets()) {
					cs.setPoint(idx++, view::to_sfml(offset));
				}
			}
			cs.setOutlineColor(sf::Color::Black);
			cs.setOutlineThickness(1);
			auto shape = std::make_unique<still_shape>(std::make_unique<sf::ConvexShape>(cs));
			switch (terrain) {
				case terrain::dirt:
					shape->shape->setTexture(&_rsrc.txtr.dirt);
					break;
				case terrain::edge:
					// Leave texture null.
					break;
				case terrain::grass:
					shape->shape->setTexture(&_rsrc.txtr.grass);
					break;
				case terrain::sand:
					shape->shape->setTexture(&_rsrc.txtr.sand);
					break;
				case terrain::snow:
					shape->shape->setTexture(&_rsrc.txtr.snow);
					break;
				case terrain::stone:
					shape->shape->setTexture(&_rsrc.txtr.stone);
					break;
				case terrain::water:
					shape->shape->setTexture(&_rsrc.txtr.water);
					break;
				default:
					UNREACHABLE;
			}
			return shape;
		}();
	}

	auto tile_widget::get_size() const -> view::vector {
		//! @todo What is the most useful size to report?
		return {};
	}

	auto tile_widget::update(sec elapsed_time) -> void {
		if (_ani) { _ani->update(elapsed_time); }
	}

	auto tile_widget::set_position(view::point position) -> void {
		_position = position;
		if (_ani) { _ani->setPosition(to_sfml(_position)); }
	}

	auto tile_widget::get_position() const -> view::point {
		return _position;
	}

	auto tile_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		if (_ani) {
			//! @todo Use a shader to indicate perception.
			target.draw(*_ani, states);
		}
	}
}
