//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_MINUS_HPP_INCLUDED
#define BOOST_TT_HAS_MINUS_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_minus
#define BOOST_TT_TRAIT_OP -
#define BOOST_TT_FORBIDDEN_IF\
   (\
      /* Lhs==pointer and Rhs==fundamental and Rhs!=integral */\
      (\
         ::mars_boost::is_pointer< Lhs_noref >::value && \
         ::mars_boost::is_fundamental< Rhs_nocv >::value && \
         (!  ::mars_boost::is_integral< Rhs_noref >::value )\
      ) || \
      /* Lhs==void* and (Rhs==fundamental or Rhs==pointer) */\
      (\
         ::mars_boost::is_pointer< Lhs_noref >::value && \
         ::mars_boost::is_void< Lhs_noptr >::value && \
         ( \
            ::mars_boost::is_fundamental< Rhs_nocv >::value || \
            ::mars_boost::is_pointer< Rhs_noref >::value\
          )\
      ) || \
      /* Rhs==void* and (Lhs==fundamental or Lhs==pointer) */\
      (\
         ::mars_boost::is_pointer< Rhs_noref >::value && \
         ::mars_boost::is_void< Rhs_noptr >::value && \
         (\
            ::mars_boost::is_fundamental< Lhs_nocv >::value || \
            ::mars_boost::is_pointer< Lhs_noref >::value\
          )\
      ) ||\
      /* Lhs=fundamental and Rhs=pointer */\
      (\
         ::mars_boost::is_fundamental< Lhs_nocv >::value && \
         ::mars_boost::is_pointer< Rhs_noref >::value\
      ) ||\
      /* two different pointers */\
      (\
         ::mars_boost::is_pointer< Lhs_noref >::value && \
         ::mars_boost::is_pointer< Rhs_noref >::value && \
         (!  ::mars_boost::is_same< Lhs_nocv, Rhs_nocv >::value )\
      )\
      )


#include <third_party/boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif