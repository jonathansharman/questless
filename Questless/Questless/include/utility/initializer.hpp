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
//! class foo
//! {
//! public:
//!		// Public members
//!	private:
//!		// Static data members
//!
//! 	friend class initializer<T>;
//! 	static initializer<T> _initializer;
//! 	static void initialize();
//!
//!		// Other private members
//! };
//! @endcode
//!
//! @warning It is critical to define the initializer member after all other static members in order to guarantee it is initialized last.
//!
template <typename T>
class initializer
{
public:
	initializer()
	{
		T::initialize();
	}
};
