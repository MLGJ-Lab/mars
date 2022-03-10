
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONTEXT_DETAIL_INVOKE_H
#define BOOST_CONTEXT_DETAIL_INVOKE_H

#include <functional>
#include <type_traits>
#include <utility>

#include <third_party/boost/config.hpp>

#include <third_party/boost/context/detail/config.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
# include BOOST_ABI_PREFIX
#endif

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {
namespace context {
namespace detail {

#if _MSC_VER > 1800 
using std::invoke;
#else
template< typename Fn, typename ... Args >
typename std::enable_if<
    std::is_member_pointer< typename std::decay< Fn >::type >::value,
    typename std::result_of< Fn &&( Args && ... ) >::type
>::type
invoke( Fn && fn, Args && ... args) {
    return std::mem_fn( fn)( std::forward< Args >( args) ... );   
}

template< typename Fn, typename ... Args >
typename std::enable_if<
    ! std::is_member_pointer< typename std::decay< Fn >::type >::value,
    typename std::result_of< Fn &&( Args && ... ) >::type
>::type
invoke( Fn && fn, Args && ... args) {
    return std::forward< Fn >( fn)( std::forward< Args >( args) ... );
}
#endif

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_CONTEXT_DETAIL_INVOKE_H