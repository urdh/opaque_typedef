#ifndef OPAQUE_DATA_HPP
#define OPAQUE_DATA_HPP
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
#include "constexpr14.hpp"
#include "utility.hpp"
#include <type_traits>

namespace opaque {

/// \addtogroup internal
/// @{

///
/// Data storage for opaque typedefs
///
template <typename U, typename O>
struct data {
  typedef U underlying_type;
  typedef O opaque_type;

  underlying_type value;

  /// Copy the underlying value
  explicit constexpr operator underlying_type() const &
    noexcept(std::is_nothrow_copy_constructible<underlying_type>::value) {
    return              value ;
  }

  /// Move the underlying value
  explicit constexpr operator underlying_type()       &&
    noexcept(std::is_nothrow_move_constructible<underlying_type>::value) {
    return opaque::move(value);
  }

  /// Construct
  template <typename... Args>
  explicit constexpr data(Args&&... args)
    noexcept(std::is_nothrow_constructible<underlying_type, Args&&...>::value)
    : value(opaque::forward<Args>(args)...) { }

  data() = default;
  data(const data& ) = default;
  data(      data&&) = default;
  data& operator=(const data& ) & = default;
  data& operator=(      data&&) & = default;
protected:
  ~data() = default;

  /// Downcast to the opaque_type
  constexpr14 opaque_type& downcast() noexcept {
    static_assert(std::is_base_of<data, opaque_type>::value, "Bad downcast");
    return *static_cast<opaque_type*>(this);
  }

};

/// @}

}

#endif
