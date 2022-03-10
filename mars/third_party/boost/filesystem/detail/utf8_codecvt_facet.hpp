// Copyright (c) 2001 Ronald Garcia, Indiana University (garcia@osl.iu.edu)
// Andrew Lumsdaine, Indiana University (lums@osl.iu.edu).

// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FILESYSTEM_UTF8_CODECVT_FACET_HPP
#define BOOST_FILESYSTEM_UTF8_CODECVT_FACET_HPP

#include <third_party/boost/filesystem/config.hpp>

#define BOOST_UTF8_BEGIN_NAMESPACE \
     namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost { namespace filesystem { namespace detail {

#define BOOST_UTF8_END_NAMESPACE }}}
#define BOOST_UTF8_DECL BOOST_FILESYSTEM_DECL

#include <third_party/boost/detail/utf8_codecvt_facet.hpp>

#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL

#endif