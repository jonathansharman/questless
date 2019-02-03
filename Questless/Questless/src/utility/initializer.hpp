//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

//! @brief Enables calling a static initialization method automatically.
//!
//! To initialize type T automatically, follow these steps.
//!
//! 1. Define a static initialize() method for type T.
//! 2. If initialize() needs to be private (recommended) or protected, make initializer<T> a friend class of T.
//! 3. Declare a static member of type initializer<T> after all other static data members.
//!
//! Example:
//! @code
//! struct foo {
//!		static int bar;
//! private:
//! 	friend class initializer<T>;
//! 	static initializer<T> _initializer;
//! 	static void initialize(); // Initializes bar.
//! };
//! @endcode
//!
//! @warning It is critical to define the initializer member after all other static members in order to guarantee it is
//! initialized last.
//!

namespace ql {
	template <typename T>
	struct initializer {
		initializer() {
			T::initialize();
		}
	};
}
