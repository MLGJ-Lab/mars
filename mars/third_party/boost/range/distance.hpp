// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_RANGE_DISTANCE_HPP
#define BOOST_RANGE_DISTANCE_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <third_party/boost/range/begin.hpp>
#include <third_party/boost/range/end.hpp>
#include <third_party/boost/range/difference_type.hpp>

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost
{

    template< class T >
    inline BOOST_DEDUCED_TYPENAME range_difference<T>::type 
    distance( const T& r )
    {
        return std::distance( mars_boost::begin( r ), mars_boost::end( r ) );
    }

} // namespace 'boost'

#endif