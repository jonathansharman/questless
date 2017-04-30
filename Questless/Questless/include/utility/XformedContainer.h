//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <boost/iterator/transform_iterator.hpp>

namespace questless
{
	//! @todo Get rid of MutableViewType and ConstViewType and just use auto for mutable_xform and const_xform, whenever template parameter auto is supported.

	//! Stores a reference to a @p Container and allows iteration through a transformed view.
	//! @tparam Container A containter type supporting forward iteration
	//! @tparam MutableViewType The viewed type when using non-const iterators.
	//! @tparam ConstViewType The viewed type when using const iterators.
	//! @tparam mutable_xform Unary function for transforming elements of the container to type @p MutableViewType
	//! @tparam const_xform Unary function for transforming elements of the container to type @p ConstViewType
	template
		< typename Container
		, typename MutableViewType
		, typename ConstViewType
		, MutableViewType (*mutable_xform)(typename Container::value_type)
		, ConstViewType (*const_xform)(typename Container::value_type)
		>
	struct XformedContainer
	{
		//! The underlying container.
		Container& container;

		//! @param container The underlying container.
		XformedContainer(Container& container) : container{container} {}

		//! Iterator to the beginning of the transformed container.
		auto begin()
		{
			return boost::make_transform_iterator(container.begin(), mutable_xform);
		}
		//! Iterator to the end of the transformed container.
		auto end()
		{
			return boost::make_transform_iterator(container.end(), mutable_xform);
		}

		//! Const iterator to the beginning of the transformed container.
		auto begin() const
		{
			return boost::make_transform_iterator(container.begin(), const_xform);
		}
		//! Const iterator to the end of the transformed container.
		auto end() const
		{
			return boost::make_transform_iterator(container.end(), const_xform);
		}
	};
}
