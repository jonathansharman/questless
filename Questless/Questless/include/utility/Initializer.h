//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

//! @brief Enables calling a static initialization method automatically.
//!
//! To initialize type T automatically, follow these steps.
//!
//! 1. Define a static initialize() method for type T.
//! 2. If initialize() needs to be private (recommended) or protected, make Initializer<T> a friend class of T.
//! 3. Declare a static member of type Initializer<T>.
//!
//! Example:
//! @code
//! class Foo
//! {
//! public:
//!		// Public members
//!	private:
//! 	friend class Initializer<T>;
//! 	static Initializer<T> _initializer;
//! 	static void initialize();
//!
//!		// Other private members, including static data.
//! };
//! @endcode
template <typename T>
class Initializer
{
public:
	Initializer()
	{
		T::initialize();
	}
};
