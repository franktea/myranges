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

template <range _Range>
class view_add_constant : public ranges::view_base
{
private:
    /* data members == "the state" */
    struct data_members_t
    {
    	_Range urange;
    };
    std::shared_ptr<data_members_t> data_members;

    /* the iterator type */
    struct iterator_type : iterator_t<_Range>
    {
        using base = iterator_t<_Range>;
        using reference = uint64_t;

        iterator_type() = default;
        iterator_type(base const & b) : base{b} {}

        iterator_type operator++(int)
        {
            return static_cast<base&>(*this)++;
        }

        iterator_type & operator++()
        {
            ++static_cast<base&>(*this);
            return (*this);
        }

        reference operator*() const
        {
            return *static_cast<base>(*this) + 42;
        }
    };

public:
    /* constructors and deconstructors */
    view_add_constant() = default;
/*    constexpr view_add_constant(view_add_constant const & rhs) = default;
    constexpr view_add_constant(view_add_constant && rhs) = default;
    constexpr view_add_constant & operator=(view_add_constant const & rhs) = default;
    constexpr view_add_constant & operator=(view_add_constant && rhs) = default;
    ~view_add_constant() = default;
*/
    view_add_constant(_Range && urange)
        //: data_members{new data_members_t{std::forward<urng_t>(urange)}}
    	:data_members(std::make_shared<data_members_t>(std::forward<_Range>(urange)))
    {}

    /* begin and end */
    iterator_type begin() const
    {
        return std::begin(data_members->urange);
    }

    iterator_type cbegin() const
    {
        return begin();
    }

    auto end() const
    {
        return std::end(data_members->urange);
    }

    auto cend() const
    {
        return end();
    }
};

template <input_range _Range>
view_add_constant(_Range &&) -> view_add_constant<_Range>;

namespace views {
	inline constexpr __adaptor::_RangeAdaptorClosure add_constant
	= [] <viewable_range _Range> (_Range&& __r)
	{
		return view_add_constant { std::forward<_Range>(__r) };
	};
} // namespace views


template <range _Range>
class view_add_constant : public ranges::view_base
{
private:
    /* data members == "the state" */
    struct data_members_t
    {
    	_Range urange;
    };
    std::shared_ptr<data_members_t> data_members;

    /* the iterator type */
    struct iterator_type : iterator_t<_Range>
    {
        using base = iterator_t<_Range>;
        using reference = uint64_t;

        iterator_type() = default;
        iterator_type(base const & b) : base{b} {}

        iterator_type operator++(int)
        {
            return static_cast<base&>(*this)++;
        }

        iterator_type & operator++()
        {
            ++static_cast<base&>(*this);
            return (*this);
        }

        reference operator*() const
        {
            return *static_cast<base>(*this) + 42;
        }
    };

public:
    /* constructors and deconstructors */
    view_add_constant() = default;
/*    constexpr view_add_constant(view_add_constant const & rhs) = default;
    constexpr view_add_constant(view_add_constant && rhs) = default;
    constexpr view_add_constant & operator=(view_add_constant const & rhs) = default;
    constexpr view_add_constant & operator=(view_add_constant && rhs) = default;
    ~view_add_constant() = default;
*/
    view_add_constant(_Range && urange)
        //: data_members{new data_members_t{std::forward<urng_t>(urange)}}
    	:data_members(std::make_shared<data_members_t>(std::forward<_Range>(urange)))
    {}

    /* begin and end */
    iterator_type begin() const
    {
        return std::begin(data_members->urange);
    }

    iterator_type cbegin() const
    {
        return begin();
    }

    auto end() const
    {
        return std::end(data_members->urange);
    }

    auto cend() const
    {
        return end();
    }
};

template <input_range _Range>
view_add_constant(_Range &&) -> view_add_constant<_Range>;

namespace views {
	inline constexpr __adaptor::_RangeAdaptorClosure add_constant
	= [] <viewable_range _Range> (_Range&& __r)
	{
		return view_add_constant { std::forward<_Range>(__r) };
	};
} // namespace views

} // namespace ranges
} // mamespace std



#endif /* MORE_RANGES_H_ */
