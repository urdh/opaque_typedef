#ifndef ARR_TEST_TRAIT_OSTREAMABLE_HPP
#define ARR_TEST_TRAIT_OSTREAMABLE_HPP
//
// Copyright (c) 2013, 2015
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
#include <utility>
#include <ostream>

/// \file

///
/// Isolated namespace for arrtest traits
///
/// This prevents the problem of ADL finding an operator match in an enclosing
/// namespace, which would prevent a search of the global namespace.
///
namespace arrtest_trait {

template <typename T>
using is_ostreamable_t =
  decltype(std::declval<std::ostream&>() << std::declval<const T&>());

#if 1

// CWG 1558
// template <typename...> using void_t = void;
template <typename...> struct voider { using type = void; };
template <typename... T> using void_t = typename voider<T...>::type;

template <typename T, template <typename> class TE, typename = void>
struct is_well_formed                       : std::false_type { };
template <typename T, template <typename> class TE>
struct is_well_formed<T, TE, void_t<TE<T>>> : std::true_type  { };

///
/// \ingroup test
/// Trait expressing the ability to format a type through std::ostream
///
template <typename T>
using is_ostreamable = is_well_formed<T, is_ostreamable_t>;

#else

//
// Alternate implementation avoiding void_t
//

template <typename T, template <typename> class Trait>
struct trait_check {
  template <typename U                     > static std::false_type check(...);
  template <typename U, typename = Trait<U>> static std::true_type  check(int);
  using type = decltype(check<T>(0));
};

///
/// \ingroup test
/// Trait expressing the ability to format a type through std::ostream
///
template <typename T>
using is_ostreamable = typename trait_check<T, is_ostreamable_t>::type;

#endif

}

#endif
