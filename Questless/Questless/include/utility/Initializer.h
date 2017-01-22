/**
* @file    Initializer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Initializer class, which enables calling a static initialization method automatically.
*
* @section USAGE
*
* To initialize type T automatically, follow these steps.
*
* 1) Define a static initialize() method for type T.
* 2) If initialize() needs to be private (recommended) or protected, make Initializer<T> a friend class of T.
* 3) Declare a static member of type Initializer<T>.
*
* E.g., for class Foo:
*
*	private:
* 		friend class Initializer<Foo>;
* 		static Initializer<Foo> _initializer;
* 		static void initialize();
*/

#pragma once

template <typename T>
class Initializer
{
public:
	Initializer()
	{
		T::initialize();
	}
};
