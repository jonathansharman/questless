//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace units
{
	namespace detail
	{
		class texture_space_buffer : public buffer_base<int, 2>
		{
		public:
			int& u() & { return _elements[0]; }
			constexpr int const& u() const& { return _elements[0]; }

			int& v() & { return _elements[1]; }
			constexpr int const& v() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	struct texture_space : space<struct texture_space_tag, int, 2, detail::texture_space_buffer>
	{
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;
	};

	inline int& width(texture_space::box& box) { return box.size.u(); }
	inline int width(texture_space::box const& box) { return box.size.u(); }

	inline int& height(texture_space::box& box) { return box.size.v(); }
	inline int height(texture_space::box const& box) { return box.size.v(); }

	inline int& left(texture_space::box& box) { return box.position.u(); }
	inline int left(texture_space::box const& box) { return box.position.u(); }

	inline int& top(texture_space::box& box) { return box.position.v(); }
	inline int top(texture_space::box const& box) { return box.position.v(); }

	inline int right(texture_space::box const& box) { return box.position.u() + box.size.u(); }

	inline int bottom(texture_space::box const& box) { return box.position.v() + box.size.v(); }

	inline texture_space::point top_left(texture_space::box const& box) { return box.position; }
	inline texture_space::point top_right(texture_space::box const& box) { return texture_space::point{box.position.u() + box.size.u(), box.position.v()}; }
	inline texture_space::point bottom_left(texture_space::box const& box) { return texture_space::point{box.position.u(), box.position.v() + box.size.v()}; }
	inline texture_space::point bottom_right(texture_space::box const& box) { return box.position + box.size; }

	inline texture_space::point center(texture_space::box const& box) { return box.position + box.size / 2; }

	inline texture_space::scalar area(texture_space::box const& box) { return width(box) * height(box); }
}
