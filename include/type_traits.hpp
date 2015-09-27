#ifndef OPAQUE_TYPE_TRAITS_HPP
#define OPAQUE_TYPE_TRAITS_HPP
//
// Copyright (c) 2015
// Kyle Markley.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of the author nor the names of any contributors may be
//    used to endorse or promote products derived from this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
#include <type_traits>

namespace opaque {

/// \addtogroup internal
/// @{

#ifdef __cpp_lib_void_t
using std::void_t;
#else
// CWG 1558
template <typename...> struct voider { using type = void; };
template <typename... T> using void_t = typename voider<T...>::type;
#endif

///
/// Determine whether a type is decayed
///
template <typename T>
struct is_decayed : std::conditional<
  std::is_same<T, typename std::decay<T>::type>::value,
  std::true_type, std::false_type>::type { };

///
/// Determine whether two types are related by inheritance
///
template <typename T, typename U>
struct is_related : std::conditional<
  std::is_base_of<T,U>::value or std::is_base_of<U,T>::value,
  std::true_type, std::false_type>::type { };

///
/// Determine whether casting to T from U is free, or creates a new object
///
template <typename T, typename U>
struct is_cast_free : std::conditional<
  std::is_same<T,U>::value or is_related<T,U>::value,
  std::true_type, std::false_type>::type { };

/// @}

}

#endif
