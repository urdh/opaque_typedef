#ifndef OPAQUE_INCONVERTIBOOL_HPP
#define OPAQUE_INCONVERTIBOOL_HPP
//
// Copyright (c) 2015, 2016
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
#include "numeric_typedef.hpp"

namespace opaque {

/// \addtogroup typedefs
/// @{

///
/// Safer boolean type.  May be implicitly created from a bool, but not
/// from any other type, and which is not implicitly convertible to any type.
///
/// This is a safer substitute for built-in bool if you are concerned about
/// implicit conversions of bool to and from other types (like int) that may
/// be bugs.  Inconvertibool prevents those implicit conversions.
///
/// By request of Christoph Weiss.
///
struct inconvertibool : opaque::numeric_typedef<bool, inconvertibool> {
  using base = opaque::numeric_typedef<bool, inconvertibool>;
  inconvertibool() = default;

  // Constructor is not explicit, but is hidden for types other than bool.
  template <typename T, typename = typename std::enable_if<
    std::is_same<bool, typename std::decay<T>::type>::value>::type>
  constexpr inconvertibool(T initial) noexcept : base(initial) { }
};

constexpr inline bool operator==(bool b, const inconvertibool& i) noexcept {
  return b == i.value;
}
constexpr inline bool operator!=(bool b, const inconvertibool& i) noexcept {
  return b != i.value;
}

/// @}

}

#endif
