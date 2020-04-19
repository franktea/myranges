/*
 * more_ranges.h
 *
 *  Created on: Apr 17, 2020
 *      Author: franktang
 */

#ifndef MORE_RANGES_H_
#define MORE_RANGES_H_

#include <ranges>
#include <memory>
#include <iostream>

namespace std {
namespace ranges {

template <input_range _Vp>
class cycle_view : public view_interface<cycle_view<_Vp>>
{
private:
	struct _Sentinel;

	struct _Iterator
	{
	private:
		friend _Sentinel;
		using _Vp_iter = iterator_t<_Vp>;

		_Vp_iter _M_current = _Vp_iter();
		cycle_view* _M_parent = nullptr;
	public:
		using iterator_category = typename iterator_traits<_Vp_iter>::iterator_category;
		using value_type = range_value_t<_Vp>;
		using difference_type = range_difference_t<_Vp>;

		_Iterator() = default;

		constexpr _Iterator(cycle_view& __parent, _Vp_iter __current)
			: _M_current(std::move(__current)),
			  _M_parent(std::__addressof(__parent)) {}

		constexpr range_reference_t<_Vp> operator*() const
		{ return *_M_current; }

		constexpr _Vp_iter operator->() const
			requires __detail::__has_arrow<_Vp_iter> && copyable<_Vp_iter>
		{ return _M_current; }

		constexpr _Iterator& operator++()
		{
			++_M_current;
			if(_M_current == ranges::end(_M_parent->_M_base))
				_M_current = ranges::begin(_M_parent->_M_base);

			return *this;
		}

		constexpr void operator++(int)
		{
			++*this;
		}

		constexpr _Iterator operator++(int) requires forward_range<_Vp>
		{
			auto __tmp = *this;
			++*this;
			return __tmp;
		}

		friend constexpr bool operator==(const _Iterator& __x, const _Iterator& __y)
			requires equality_comparable<_Vp_iter>
		{ return __x._M_current == __y._M_current; }
	};

	struct _Sentinel
	{
	private:
		sentinel_t<_Vp> _M_end = sentinel_t<_Vp>();

		constexpr bool __equal(const _Iterator& __i) const
		{
			return __i._M_current == _M_end;
		}
	public:
		_Sentinel() = default;

		constexpr explicit _Sentinel(cycle_view& __parent) :
			_M_end(ranges::end(__parent._M_base)) {}

		friend constexpr bool operator==(const _Iterator& __x, const _Sentinel& __y)
		{ return __y.__equal(__x); }
	};

	_Vp _M_base = _Vp();
public:
    cycle_view() = default;

    constexpr cycle_view(_Vp __base): _M_base(std::move(__base)) {}

    constexpr _Iterator begin()
    {
        return {*this, ranges::begin(_M_base)};
    }

    constexpr auto end()
    {
    	if constexpr (common_range<_Vp>)
    		return _Iterator{*this, ranges::end(_M_base)};
    	else
    		return _Sentinel{*this};
    }
};

template <input_range _Range>
cycle_view(_Range &&) -> cycle_view<views::all_t<_Range>>;

namespace views {
	inline constexpr __adaptor::_RangeAdaptorClosure cycle
	= [] <viewable_range _Range> (_Range&& __r)
	{
		return cycle_view { std::forward<_Range>(__r) };
	};
} // namespace views

template <input_range _Vp1, input_range _Vp2>
class zip_view : public view_interface<zip_view<_Vp1, _Vp2>>
{
private:
	struct _Sentinel;

	struct _Iterator
	{
	private:
		friend zip_view;
		friend _Sentinel;
		using _Vp1_iter = iterator_t<_Vp1>;
		using _Vp2_iter = iterator_t<_Vp2>;

		_Vp1_iter _M_current1 = _Vp1_iter();
		_Vp2_iter _M_current2 = _Vp2_iter();
		zip_view* _M_parent = nullptr;
	public:
		using iterator_category = typename iterator_traits<_Vp1_iter>::iterator_category;
		using value_type = std::pair<range_value_t<_Vp1>, range_value_t<_Vp2>>;
		using difference_type = range_difference_t<_Vp1>;
		
		_Iterator() = default;

		constexpr _Iterator(zip_view& __parent,
			_Vp1_iter __current1, _Vp2_iter __current2) :
			_M_current1(std::move(__current1)),
			_M_current2(std::move(__current2)),
			_M_parent(std::__addressof(__parent)) {}

		constexpr std::pair<range_reference_t<_Vp1>, range_reference_t<_Vp2>> 
			operator*() const
		{ return std::make_pair(std::ref(*_M_current1), std::ref(*_M_current2)); }

		constexpr std::pair<_Vp1_iter, _Vp2_iter> operator->() const
			requires __detail::__has_arrow<_Vp1_iter> &&
					__detail::__has_arrow<_Vp2_iter>
		{ return std::make_pair(_M_current1, _M_current2); }

		constexpr _Iterator& operator++()
		{
			++_M_current1;
			++_M_current2;
			//if(_M_current1 == ranges::end(_M_parent->_M_base1)
			//	|| _M_current2 == ranges::end(_M_parent->_M_base2))
			return *this;
		}

		constexpr void operator++(int)
		{ ++*this; }

		constexpr _Iterator operator++(int) requires forward_range<_Vp1> && forward_range<_Vp2>
		{
			auto __tmp = *this;
			++*this;
			return __tmp;
		}

		friend constexpr bool operator==(const _Iterator& __x, const _Iterator& __y)
			requires equality_comparable<_Vp1_iter> && equality_comparable<_Vp2_iter>
		{ return __x._M_current1 == __y._M_current1 && __x._M_current2 == __y._M_current2; }
	};

	struct _Sentinel
	{
	private:
		std::pair<sentinel_t<_Vp1>, sentinel_t<_Vp2>> _M_end = 
			std::make_pair(sentinel_t<_Vp1>(), sentinel_t<_Vp2>());

		constexpr bool __equal(const _Iterator& __i) const
		{
			return __i._M_current1 == _M_end.first || __i._M_current2 == _M_end.second;
		}
	public:
		_Sentinel() = default;

		constexpr explicit _Sentinel(zip_view& __parent): _M_end(std::make_pair(
			ranges::end(__parent._M_base1), ranges::end(__parent._M_base2))) {}

		friend constexpr bool operator==(const _Iterator& __x, const _Sentinel& __y)
		{ return __y.__equal(__x); }
	};

	_Vp1 _M_base1 = _Vp1();
	_Vp2 _M_base2 = _Vp2();
public:
	zip_view() = default;

	constexpr zip_view(_Vp1 __base1, _Vp2 __base2): _M_base1(std::move(__base1)),
		_M_base2(std::move(__base2)) {}

	constexpr _Iterator begin()
	{ return {*this, ranges::begin(_M_base1), ranges::begin(_M_base2)}; }

	constexpr auto end()
	{
		//if constexpr (common_range<_Vp1> && common_range<_Vp2>)
		//	return _Iterator{*this, ranges::end(_M_base1), ranges::end(_M_base2)};
		//else
			return _Sentinel{*this};
	}
};


template <input_range _Vp1, input_range _Vp2>
inline constexpr bool enable_borrowed_range<zip_view<_Vp1, _Vp2>> = true;

template <input_range _Range1, input_range _Range2>
zip_view(_Range1&&, _Range2&&) -> zip_view<views::all_t<_Range1>, views::all_t<_Range2>>;

namespace views {
	inline constexpr __adaptor::_RangeAdaptor zip
	= [] <viewable_range _Range1, viewable_range _Range2> (_Range1&& __r1, _Range2&& __r2)
	{
		return zip_view { std::forward<_Range1>(__r1), std::forward<_Range2>(__r2) };
	};
} // namespace views

} // namespace ranges
} // mamespace std

#endif /* MORE_RANGES_H_ */
