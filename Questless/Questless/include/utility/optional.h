/**
* @file    optional.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A partial implemenation of the upcoming std::optional type.
*/

#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <algorithm>
#include <utility>
#include <stdexcept>

/// @todo Replace this class with std::optional once it's available.

class nullopt_t {} const nullopt;
class bad_optional_access : public std::logic_error
{
public:
	bad_optional_access() : logic_error("") {}
};

template <typename T>
class optional
{
public:
	using value_type = T;

	constexpr optional() : _value{}, _engaged { false } {}
	constexpr optional(nullopt_t) : _value{}, _engaged { false } {}
	optional(const optional& other) : _value(other._value), _engaged{other._engaged} {}
	optional(optional&& other) : _value(std::move(other._value)), _engaged{other._engaged} {}
	constexpr optional(const T& value) : _value(value), _engaged{true} {}
	constexpr optional(T&& value) : _value(std::move(value)), _engaged{true} {}

	optional& operator =(nullopt_t)
	{
		_engaged = false;
		return *this;
	}
	optional& operator =(const optional& other)
	{
		_value = other._value;
		_engaged = other._engaged;
	}
	optional& operator =(optional&& other)
	{
		swap(other);
		return *this;
	}
	template <typename U>
	optional& operator =(U&& value)
	{
		return *this = optional{value};
	}
		
	const T& operator *() const
	{
		if (_engaged) {
			return _value;
		} else {
			throw bad_optional_access();
		}
	}
	const T* operator ->() const
	{
		if (_engaged) {
			return &_value;
		} else {
			throw bad_optional_access();
		}
	}
	T& operator *()
	{
		if (_engaged) {
			return _value;
		} else {
			throw bad_optional_access();
		}
	}
	T* operator ->()
	{
		if (_engaged) {
			return &_value;
		} else {
			throw bad_optional_access();
		}
	}

	explicit operator bool() const { return _engaged; }

	const T& value() const { return _value; }
	T& value() { return _value; }

	template <class U>
	const T& value_or(U&& value) const { return _engaged ? _value : value; }
	template <class U>
	T& value_or(U&& value) { return _engaged ? _value : value; }

	void swap(optional& other)
	{
		if (_engaged && !other._engaged) {
			other._value = std::move(_value);
			_engaged = false;
			other._engaged = true;
		} else if (!_engaged && other._engaged) {
			_value = std::move(other._value);
			_engaged = true;
			other._engaged = false;
		} else if (_engaged && other._engaged) {
			std::swap(**this, *other);
		}
	}

	/// @todo Implement emplace here if desired.

	/// @todo Friend functions for comparison operators.
private:
	T _value;
	bool _engaged;
};

template <typename T>
constexpr optional<typename std::decay<T>::type> make_optional(T&& value)
{
	return optional<typename std::decay<T>::type>{std::forward<T>(value)};
}

#endif
