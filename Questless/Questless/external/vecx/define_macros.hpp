//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Defines convenience macros for defining space types.

#define DEFINE_VECTOR_INDEX_NAME(vector_type, index, name) \
	constexpr auto& name(vector_type& v) { return v[index]; } \
	constexpr auto name(vector_type const& v) { return v[index]; }

#define DEFINE_BOX_SIZE_NAME(box_type, index, name) \
	constexpr auto& name(box_type& box) { return box.size[index]; } \
	constexpr auto name(box_type const& box) { return box.size[index]; }

#define DEFINE_BOX_EXTREMES_NAMES(box_type, index, near_name, far_name) \
	constexpr auto& near_name(box_type& box) { return box.position[index]; } \
	constexpr auto near_name(box_type const& box) { return box.position[index]; } \
	constexpr auto far_name(box_type const& box) { return box.position[index] + box.size[index]; }
