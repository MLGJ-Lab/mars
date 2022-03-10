//  GetCurrentProcess.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba
//  Copyright 2015 Andrey Semashev

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_DETAIL_WINAPI_GETCURRENTPROCESS_HPP
#define BOOST_DETAIL_WINAPI_GETCURRENTPROCESS_HPP

#include <third_party/boost/detail/winapi/basic_types.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

// Windows CE define GetCurrentProcess as an inline function in kfuncs.h
#if !defined( BOOST_USE_WINDOWS_H ) && !defined( UNDER_CE )
extern "C" {
BOOST_SYMBOL_IMPORT mars_boost::detail::winapi::HANDLE_ WINAPI GetCurrentProcess(BOOST_DETAIL_WINAPI_VOID);
}
#endif

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {
namespace detail {
namespace winapi {
using ::GetCurrentProcess;
}
}
}

#endif // BOOST_DETAIL_WINAPI_GETCURRENTPROCESS_HPP