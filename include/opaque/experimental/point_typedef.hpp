#ifndef OPAQUE_EXPERIMENTAL_POINT_TYPEDEF_HPP
#define OPAQUE_EXPERIMENTAL_POINT_TYPEDEF_HPP
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
#include "../numeric_typedef.hpp"

namespace opaque {
namespace experimental {

/// \addtogroup typedefs
/// @{

///
/// Point-like numeric opaque typedef
///
/// This is a numeric typedef base class providing interoperability between
/// two types for the + and - operations as follows:
///  - point + point --> type error
///  - point + span  --> point
///  - span  + point --> point
///  - span  + span  --> span
///  - point - point --> span
///  - point - span  --> point
///  - span  - point --> type error
///  - span  - span  --> span
///
/// Template arguments for point_typedef:
///  -# Span : The span type, which must be a numeric opaque typedef
///  -# R : The result type, your subclass
///
template <typename Span, typename R>
struct point_typedef
  : numeric_typedef_base<typename Span::value_t, R, typename Span::shift_t>
  , binop::addable     <R   , true , R   , Span>
  , binop::addable     <R   , true , Span, R   >
  , binop::subtractable<R   , false, R   , Span>
  , binop::subtractable<Span, false, R   , R   ,
                        typename Span::value_t, typename Span::value_t>
{
  using base =
    numeric_typedef_base<typename Span::value_t, R, typename Span::shift_t>;
  using base::value;
  using base::downcast;
  typedef R result_t;

  result_t& operator+=(const result_t&) = delete;
  result_t& operator-=(const result_t&) = delete;

  constexpr14 result_t& operator+=(const Span& s)
    noexcept(noexcept( value += s.value )) {
                       value += s.value;
    return downcast(); }

  constexpr14 result_t& operator-=(const Span& s)
    noexcept(noexcept( value -= s.value )) {
                       value -= s.value;
    return downcast(); }

  using base::base;
  explicit point_typedef() = default;
  point_typedef(const point_typedef& ) = default;
  point_typedef(      point_typedef&&) = default;
  point_typedef& operator=(const point_typedef& ) = default;
  point_typedef& operator=(      point_typedef&&) = default;
protected:
  ~point_typedef() = default;
};

/// @}

}
}

#endif
