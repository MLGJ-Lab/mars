
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_COROUTINES_DETAIL_TRAMPOLINE_PUSH_H
#define BOOST_COROUTINES_DETAIL_TRAMPOLINE_PUSH_H

#include <cstddef>

#include <third_party/boost/assert.hpp>
#include <third_party/boost/config.hpp>
#include <third_party/boost/context/detail/fcontext.hpp>
#include <third_party/boost/cstdint.hpp>
#include <third_party/boost/exception_ptr.hpp>
#include <third_party/boost/move/move.hpp>

#include <third_party/boost/coroutine/detail/config.hpp>
#include <third_party/boost/coroutine/detail/data.hpp>
#include <third_party/boost/coroutine/detail/flags.hpp>
#include <third_party/boost/coroutine/detail/parameters.hpp>
#include <third_party/boost/coroutine/detail/setup.hpp>
#include <third_party/boost/coroutine/detail/setup.hpp>
#include <third_party/boost/coroutine/exceptions.hpp>
#include <third_party/boost/coroutine/flags.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {
namespace coroutines {
namespace detail {

template< typename Coro >
void trampoline_push( context::detail::transfer_t t)
{
    typedef typename Coro::param_type   param_type;

    data_t * data = static_cast< data_t * >( t.data);
    data->from->ctx_ = t.fctx;
    param_type * param(
        static_cast< param_type * >( data->data) );
    BOOST_ASSERT( 0 != param);
    BOOST_ASSERT( 0 != param->data);

    Coro * coro(
        static_cast< Coro * >( param->coro) );
    BOOST_ASSERT( 0 != coro);

    coro->run( param->data);
}

template< typename Coro >
void trampoline_push_void( context::detail::transfer_t t)
{
    typedef typename Coro::param_type   param_type;

    data_t * data = static_cast< data_t * >( t.data);
    data->from->ctx_ = t.fctx;
    param_type * param(
        static_cast< param_type * >( data->data) );
    BOOST_ASSERT( 0 != param);

    Coro * coro(
        static_cast< Coro * >( param->coro) );
    BOOST_ASSERT( 0 != coro);

    coro->run();
}

}}}

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_COROUTINES_DETAIL_TRAMPOLINE_PUSH_H