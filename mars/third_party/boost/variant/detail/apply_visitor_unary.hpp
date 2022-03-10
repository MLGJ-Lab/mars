//-----------------------------------------------------------------------------
// boost variant/detail/apply_visitor_unary.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003 Eric Friedman
// Copyright (c) 2014 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP
#define BOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP

#include "third_party/boost/config.hpp"
#include "third_party/boost/detail/workaround.hpp"
#include "third_party/boost/variant/detail/generic_result_type.hpp"

#if BOOST_WORKAROUND(__EDG__, BOOST_TESTED_AT(302))
#include "third_party/boost/core/enable_if.hpp"
#include "third_party/boost/mpl/not.hpp"
#include "third_party/boost/type_traits/is_const.hpp"
#endif

#if !defined(BOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(BOOST_NO_CXX11_DECLTYPE_N3276)
#   include <third_party/boost/mpl/distance.hpp>
#   include <third_party/boost/mpl/advance.hpp>
#   include <third_party/boost/mpl/deref.hpp>
#   include <third_party/boost/mpl/size.hpp>
#   include <third_party/boost/utility/declval.hpp>
#   include <third_party/boost/core/enable_if.hpp>
#   include "third_party/boost/variant/detail/has_result_type.hpp"
#endif

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {

//////////////////////////////////////////////////////////////////////////
// function template apply_visitor(visitor, visitable)
//
// Visits visitable with visitor.
//

//
// nonconst-visitor version:
//

#if !BOOST_WORKAROUND(__EDG__, BOOST_TESTED_AT(302))

#   define BOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(V) \
    BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename V::result_type) \
    /**/

#else // EDG-based compilers

#   define BOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(V) \
    typename enable_if< \
          mpl::not_< is_const< V > > \
        , BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename V::result_type) \
        >::type \
    /**/

#endif // EDG-based compilers workaround

template <typename Visitor, typename Visitable>
inline
    BOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE(Visitor)
apply_visitor(Visitor& visitor, Visitable& visitable)
{
    return visitable.apply_visitor(visitor);
}

#undef BOOST_VARIANT_AUX_APPLY_VISITOR_NON_CONST_RESULT_TYPE

//
// const-visitor version:
//

template <typename Visitor, typename Visitable>
inline
    BOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
apply_visitor(const Visitor& visitor, Visitable& visitable)
{
    return visitable.apply_visitor(visitor);
}


#if !defined(BOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(BOOST_NO_CXX11_DECLTYPE_N3276)

// C++14
namespace detail { namespace variant {

// This class serves only metaprogramming purposes. none of its methods must be called at runtime!
template <class Visitor, class Variant>
struct result_multideduce1 {
    typedef typename Variant::types                 types;
    typedef typename mars_boost::mpl::begin<types>::type begin_it;
    typedef typename mars_boost::mpl::advance<
        begin_it, mars_boost::mpl::int_<mars_boost::mpl::size<types>::type::value - 1>
    >::type                                         last_it;

    // For metaprogramming purposes ONLY! Do not use this method (and class) at runtime!
    static Visitor& vis() BOOST_NOEXCEPT {
        // Functions that work with lambdas must be defined in same translation unit.
        // Because of that, we can not use `mars_boost::decval<Visitor&>()` here.
        Visitor&(*f)() = 0; // pointer to function
        return f();
    }

    static decltype(auto) deduce_impl(last_it, unsigned /*helper*/) {
        typedef typename mars_boost::mpl::deref<last_it>::type value_t;
        return vis()( mars_boost::declval< value_t& >() );
    }

    template <class It>
    static decltype(auto) deduce_impl(It, unsigned helper) {
        typedef typename mars_boost::mpl::next<It>::type next_t;
        typedef typename mars_boost::mpl::deref<It>::type value_t;
        if (helper == mars_boost::mpl::distance<begin_it, It>::type::value) {
            return deduce_impl(next_t(), ++helper);
        }

        return vis()( mars_boost::declval< value_t& >() );
    }

    static decltype(auto) deduce() {
        return deduce_impl(begin_it(), 0);
    }
};

template <class Visitor, class Variant>
struct result_wrapper1
{
    typedef decltype(result_multideduce1<Visitor, Variant>::deduce()) result_type;

    Visitor& visitor_;
    explicit result_wrapper1(Visitor& visitor) BOOST_NOEXCEPT
        : visitor_(visitor)
    {}

    template <class T>
    result_type operator()(T& val) const {
        return visitor_(val);
    }
};

}} // namespace detail::variant

template <typename Visitor, typename Visitable>
inline decltype(auto) apply_visitor(Visitor& visitor, Visitable& visitable,
    typename mars_boost::disable_if<
        mars_boost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    mars_boost::detail::variant::result_wrapper1<Visitor, Visitable> cpp14_vis(visitor);
    return visitable.apply_visitor(cpp14_vis);
}

template <typename Visitor, typename Visitable>
inline decltype(auto) apply_visitor(const Visitor& visitor, Visitable& visitable,
    typename mars_boost::disable_if<
        mars_boost::detail::variant::has_result_type<Visitor>
    >::type* = 0)
{
    mars_boost::detail::variant::result_wrapper1<const Visitor, Visitable> cpp14_vis(visitor);
    return visitable.apply_visitor(cpp14_vis);
}

#endif // !defined(BOOST_NO_CXX14_DECLTYPE_AUTO) && !defined(BOOST_NO_CXX11_DECLTYPE_N3276)

} // namespace mars_boost

#endif // BOOST_VARIANT_DETAIL_APPLY_VISITOR_UNARY_HPP