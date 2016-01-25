#ifndef OPAQUE_EXPERIMENTAL_STRING_TYPEDEF_HPP
#define OPAQUE_EXPERIMENTAL_STRING_TYPEDEF_HPP
//
// Copyright (c) 2016
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
#include "safer_string_typedef.hpp"
#include <memory>
#include <string>

namespace opaque {
namespace experimental {

/// \addtogroup typedefs
/// @{

///
/// String opaque typedef base type
///
/// This is an opaque typedef base class for standard strings.
///
/// Template arguments:
///  -# S : The string type (e.g. std::string)
///  -# R : The result type, your subclass
///
template <typename S, typename R>
struct string_typedef : safer_string_typedef<S,R> {
private:
  using base = safer_string_typedef<S,R>;
public:
  using typename base::underlying_type;
  using typename base::opaque_type;
  using base::value;

  using typename base::allocator_type;
  using typename base::size_type;
  using base::npos;

private:
  typedef          allocator_type Allocator;
  typedef typename std::allocator_traits<Allocator>::value_type charT;
public:

  using base::base;

  opaque_type& operator=(const charT* s) {
    value = s;
    return downcast();
  }

  opaque_type& operator+=(const charT* s) {
    value += s;
    return downcast();
  }
  opaque_type& append(const charT* s, size_type n) {
    value.append(s, n);
    return downcast();
  }
  opaque_type& append(const charT* s) {
    value.append(s);
    return downcast();
  }

  opaque_type& assign(const charT* s, size_type n) {
    value.assign(s, n);
    return downcast();
  }
  opaque_type& assign(const charT* s) {
    value.assign(s);
    return downcast();
  }

  opaque_type& insert(size_type pos, const charT* s, size_type n) {
    value.insert(pos, s, n);
    return downcast();
  }
  opaque_type& insert(size_type pos, const charT* s) {
    value.insert(pos, s);
    return downcast();
  }

  opaque_type& replace(size_type pos, size_type n1, const charT* s,
      size_type n2) {
    value.replace(pos, n1, s, n2);
    return downcast();
  }
  opaque_type& replace(size_type pos, size_type n1, const charT* s) {
    value.replace(pos, n1, s);
    return downcast();
  }

  friend opaque_type operator+(const charT* lhs, const opaque_type&  rhs) {
    return lhs +           rhs.value ;
  }
  friend opaque_type operator+(const charT* lhs,       opaque_type&& rhs) {
    return lhs + std::move(rhs.value);
  }
  friend opaque_type operator+(const opaque_type&  lhs, const charT* rhs) {
    return           lhs.value  + rhs;
  }
  friend opaque_type operator+(      opaque_type&& lhs, const charT* rhs) {
    return std::move(lhs.value) + rhs;
  }

  string_typedef() = default;
  string_typedef(const string_typedef& ) = default;
  string_typedef(      string_typedef&&) = default;
  string_typedef& operator=(const string_typedef& ) = default;
  string_typedef& operator=(      string_typedef&&) = default;
protected:
  ~string_typedef() = default;
  using base::downcast;
};

/// @}

}
}

#endif
