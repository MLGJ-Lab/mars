//-----------------------------------------------------------------------------
// boost variant/detail/enable_recursive_fwd.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_FWD_HPP
#define BOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_FWD_HPP

#include "third_party/boost/mpl/aux_/config/ctps.hpp"

#include "third_party/boost/mpl/bool_fwd.hpp"

#   include "third_party/boost/mpl/bool.hpp"

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {
namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) tag recursive_flag
//
// Signifies that the variant should perform recursive substituion.
//


template <typename T>
struct recursive_flag
{
    typedef T type;
};


///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction is_recursive_flag
//
// Signifies that the variant should perform recursive substituion.
//


template <typename T>
struct is_recursive_flag
    : mpl::false_
{
};

template <typename T>
struct is_recursive_flag< recursive_flag<T> >
    : mpl::true_
{
};


///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction enable_recursive
//
// Attempts recursive_variant_ tag substitution, wrapping with
// mars_boost::recursive_wrapper if substituion occurs w/ non-indirect result
// (i.e., not a reference or pointer) *and* NoWrapper is false_.
//
template <
      typename T
    , typename RecursiveVariant
    , typename NoWrapper = mpl::false_
    >
struct enable_recursive;

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction class quoted_enable_recursive
//
// Same behavior as enable_recursive metafunction (see above).
//
template <
      typename RecursiveVariant
    , typename NoWrapper = mpl::false_
    >
struct quoted_enable_recursive;

}} // namespace detail::variant
} // namespace mars_boost

#endif // BOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_FWD_HPP