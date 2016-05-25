/**
* @file    Stream.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* The Stream class, which implements a lazy linked list.
* Inspired by http://bartoszmilewski.com/2014/04/21/getting-lazy-with-c/.
* Iterator adapted from http://codereview.stackexchange.com/questions/74609/custom-iterator-for-a-linked-list-class.
*/

#ifndef STREAM_H
#define STREAM_H

#include <functional>
#include <memory>

template <typename T>
class Suspension
{
public:
	Suspension(std::function<T()> f) : _f{f}, _thunk{&force_thunk} {}

	const T& get() { return _thunk(*this); }
	const T& operator *() { return _thunk(*this); }
	const T* operator ->() { return &_thunk(*this); }
private:
	mutable const T& (*_thunk)(Suspension&);
	mutable T _memo;

	std::function<T()> _f;

	static const T& force_thunk(Suspension& s) { return s.set_memo(); }
	static const T& get_thunk(Suspension& s) { return s.get_memo(); }

	const T& get_memo() const { return _memo; }
	const T& set_memo() const
	{
		_memo = _f();
		_thunk = &get_thunk;
		return _memo;
	}
};

template <typename T>
class Stream;

template <typename T>
class Cell
{
public:
	Cell() {}
	Cell(T head) : _head{head} {}
	Cell(T head, const Stream<T>& tail) : _head{head}, _tail{tail} {}

	T head() const { return _head; }

	Stream<T> tail() const { return _tail; }
private:
	T _head;
	Stream<T> _tail;
};

template <typename T>
class Stream
{
public:
	template <typename Type, typename UnqualifiedType = std::remove_cv_t<Type>>
	class ForwardIterator : public std::iterator<std::forward_iterator_tag, UnqualifiedType, std::ptrdiff_t, Type*, Type&>
	{
	public:
		/// @return End iterator.
		ForwardIterator() : _itr{Stream{}} {}

		void swap(ForwardIterator& other) noexcept { std::swap(_itr, other._itr); }

		ForwardIterator& operator ++()
		{
			if (_itr.empty()) {
				throw std::out_of_range{"Stream iterator out of range."};
			}
			_itr = _itr.tail();
			return *this;
		}

		ForwardIterator operator ++(int)
		{
			if (_itr.empty()) {
				throw std::out_of_range{"Stream iterator out of range."};
			}
			ForwardIterator ret = *this;
			_itr = _itr.tail();
			return ret;
		}

		template <typename OtherType>
		bool operator ==(const ForwardIterator<OtherType>& other) const { return _itr == other._itr; }

		template <typename OtherType>
		bool operator !=(const ForwardIterator<OtherType>& other) const { return _itr != other._itr; }

		Type operator *() const
		{
			if (_itr.empty()) {
				throw std::logic_error{"Invalid stream iterator dereference."};
			}
			return _itr.head();
		}

		Type operator ->() const
		{
			if (_itr.empty()) {
				throw std::logic_error{"Invalid stream iterator dereference."};
			}
			return &_itr.head();
		}

		/// @return Constant iterator from this iterator.
		operator ForwardIterator<const Type>() const { return ForwardIterator<const Type>{itr}; }
	private:
		friend class Stream<UnqualifiedType>;

		Stream<UnqualifiedType> _itr;

		explicit ForwardIterator(Stream<UnqualifiedType> s) : _itr{std::move(s)} {}
	};

	using iterator = ForwardIterator<T>;
	using const_iterator = ForwardIterator<const T>;

	Stream() {}
	explicit Stream(std::function<Cell<T>()> f) : _lazy_cell{std::make_shared<Suspension<Cell<T>>>(f)} {}

	bool operator ==(const Stream& other) const { return _lazy_cell == other._lazy_cell; }
	bool operator !=(const Stream& other) const { return _lazy_cell != other._lazy_cell; }

	bool empty() const { return !_lazy_cell; }

	T head() const { return _lazy_cell->get().head(); }

	Stream<T> tail() const { return _lazy_cell->get().tail(); }

	// Const iterators are the same as non-const iterators since streams are functional.

	iterator begin() const { return iterator{*this}; }
	const_iterator cbegin() const { return const_iterator{*this}; }
	iterator end() const { return iterator{}; }
	iterator cend() const { return iterator{}; }

	Stream take(int n)
	{
		if (n <= 0 || empty()) {
			return Stream{};
		}
		return Stream<T>{[_lazy_cell, n] {
			auto x = _lazy_cell->get();
			return Cell<T>{_lazy_cell->get().value(), _lazy_cell->get().tail()};
		}};
	}
private:
	std::shared_ptr<Suspension<Cell<T>>> _lazy_cell;
};

template <typename T, typename F>
inline void for_each(Stream<T> s, F f)
{
	while (!s.empty()) {
		f(s.head());
		s = s.tail();
	}
}

template <typename T, typename F>
inline auto smap(Stream<T> s, F f) -> Stream<decltype(f(s.head()))>
{
	using U = decltype(f(s.head()));
	static_assert(std::is_convertible<F, std::function<U(T)>>::value, "f must be of type U(T).");

	if (s.empty()) {
		return Stream<U>{};
	}
	return Stream<U>([s, f]() {
		return Cell<U>{f(s.head()), smap(s.tail(), f)};
	});
}

template <typename T, typename F>
inline auto smapv(Stream<T> s, F f) -> Stream<decltype(f())>
{
	using U = decltype(f());
	static_assert(std::is_convertible<F, std::function<U()>>::value, "f must be of type U().");

	if (s.isEmpty()) {
		return Stream<U>{};
	}
	return Stream<U>{[s, f]() {
		return Cell<U>{f(), smapv(s.tail(), f)};
	}};
}

template <typename T>
inline Stream<T> sconcat(Stream<T> left, Stream<T> right)
{
	if (left.empty()) {
		return right;
	}
	return Stream<T>{[=]() {
		return Cell<T>{left.head(), sconcat<T>(left.tail(), right)};
	}};
}

template <typename T>
inline Stream<T> sjoin(Stream<Stream<T>> s)
{
	while (!s.empty() && s.head().empty()) { // Eliminate leading empty streams.
		s = s.tail();
	}
	if (s.empty()) {
		return Stream<T>{};
	}
	return Stream<T>{[s]() {
		Stream<T> head = s.head();
		return Cell<T>{head.head(), concat(head.tail(), sjoin(s.tail()))};
	}};
}

template <typename T, typename F>
inline auto sbind(Stream<T> s, F f) -> decltype(f(s.get()))
{
	return sjoin(map(s, f));
}

template <typename T, typename F>
inline auto sthen(Stream<T> s, F f) -> decltype(f())
{
	return sjoin(mapv(s, f));
}

inline Stream<void*> sguard(bool b)
{
	return b ? Stream<void*>{nullptr} : Stream<void*>{};
}

template <typename T>
inline static Stream<T> singleton(T v)
{
	return Stream<T>{[v]() {
		return Cell<T>{v};
	}};
}

/// @todo This whole thing is still a bit of a mess.

#endif
