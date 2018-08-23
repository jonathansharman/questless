//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "space.hpp"

namespace units {
	namespace detail {
		using color_buffer_base_t = buffer_base<float, 4>;
		class color_buffer : public color_buffer_base_t {
		public:
			float& red() & { return _elements[0]; }
			constexpr float const& red() const& { return _elements[0]; }

			float& green() & { return _elements[1]; }
			constexpr float const& green() const& { return _elements[1]; }

			float& blue() & { return _elements[2]; }
			constexpr float const& blue() const& { return _elements[2]; }

			float& alpha() & { return _elements[3]; }
			constexpr float const& alpha() const& { return _elements[3]; }
		protected:
			using color_buffer_base_t::color_buffer_base_t;
			using color_buffer_base_t::operator =;
		};
	}

	using color_space = space<struct color_space_tag, float, 4, detail::color_buffer>;
}
