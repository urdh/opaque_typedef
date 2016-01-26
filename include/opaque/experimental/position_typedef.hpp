#ifndef OPAQUE_EXPERIMENTAL_POSITION_TYPEDEF_HPP
#define OPAQUE_EXPERIMENTAL_POSITION_TYPEDEF_HPP
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
/// Numeric opaque typedef to represent a position
///
/// This is a numeric typedef base class providing interoperability between
/// two types for the + and - operations as follows:
///  - position + position --> type error
///  - position + distance --> position
///  - distance + position --> position
///  - distance + distance --> distance
///  - position - position --> distance
///  - position - distance --> position
///  - distance - position --> type error
///  - distance - distance --> distance
///
/// Template arguments for position_typedef:
///  -# Distance : The distance type, which must be a numeric opaque typedef
///  -# O : The opaque type, your subclass
///
template <typename Distance, typename O>
struct position_typedef
  : numeric_typedef_base<typename Distance::underlying_type, O,
                         typename Distance::shift_type>
  , binop::addable     <O       , true , O       , Distance>
  , binop::addable     <O       , true , Distance, O       >
  , binop::subtractable<O       , false, O       , Distance>
  , binop::subtractable<Distance, false, O       , O       ,
                        typename Distance::underlying_type,
                        typename Distance::underlying_type>
{
private:
  using base =
    numeric_typedef_base<typename Distance::underlying_type, O,
                         typename Distance::shift_type>;
public:
  using typename base::underlying_type;
  using typename base::opaque_type;
  using base::value;

  opaque_type& operator+=(const opaque_type&) = delete;
  opaque_type& operator-=(const opaque_type&) = delete;

  constexpr14 opaque_type& operator+=(const Distance& s)
    noexcept(noexcept( value += s.value )) {
                       value += s.value;
    return downcast(); }

  constexpr14 opaque_type& operator-=(const Distance& s)
    noexcept(noexcept( value -= s.value )) {
                       value -= s.value;
    return downcast(); }

  using base::base;
  explicit position_typedef() = default;
  position_typedef(const position_typedef& ) = default;
  position_typedef(      position_typedef&&) = default;
  position_typedef& operator=(const position_typedef& ) = default;
  position_typedef& operator=(      position_typedef&&) = default;
protected:
  ~position_typedef() = default;
  using base::downcast;
};

/// @}

}
}

#endif
