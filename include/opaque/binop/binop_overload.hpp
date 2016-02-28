#ifndef OPAQUE_BINOP_OVERLOAD_HPP
#define OPAQUE_BINOP_OVERLOAD_HPP
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
#include "../constexpr14.hpp"
#include "../type_traits.hpp"
#include "../utility.hpp"
#include "../convert.hpp"
#include <utility>

namespace opaque {
namespace binop {

/// \addtogroup internal
/// @{

///
/// Overloads of binary operators
///

template <typename OP, typename RT, bool apply_commutativity=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2>
struct overload;

template <typename OP, typename RT,
         typename P1, typename P2, typename I1, typename I2>
struct overload<OP,RT,false,const P1& ,const P2& ,I1,I2> {
  // No return type conversion - enable NRVO
  template <typename R=RT> static constexpr14
  typename std::enable_if<    std::is_same<RT,I1>::value, R>::type
  func(const P1&  p1, const P2&  p2, OP op=OP{}) noexcept(
      std::is_nothrow_constructible<I1,const P1&>::value and
      noexcept(op(std::declval<I1>(), convert<I2>(          p2 )))) {
    I1 temp(p1);
    op(temp, convert<I2>(          p2 ));
    return temp;
  }
  // Return type conversion
  template <typename R=RT> static constexpr14
  typename std::enable_if<not std::is_same<RT,I1>::value, R>::type
  func(const P1&  p1, const P2&  p2, OP op=OP{}) noexcept(
      std::is_nothrow_constructible<I1,const P1&>::value and
      std::is_nothrow_constructible<RT,I1&&>::value and
      noexcept(op(std::declval<I1>(), convert<I2>(          p2 )))) {
    I1 temp(p1);
    op(temp, convert<I2>(          p2 ));
    return static_cast<RT>(std::move(temp));
  }
};

template <typename OP, typename RT,
         typename P1, typename P2, typename I1, typename I2>
struct overload<OP,RT,false,const P1& ,      P2&&,I1,I2> {
  // No return type conversion - enable NRVO
  template <typename R=RT> static constexpr14
  typename std::enable_if<    std::is_same<RT,I1>::value, R>::type
  func(const P1&  p1,       P2&& p2, OP op=OP{}) noexcept(
      std::is_nothrow_constructible<I1,const P1&>::value and
      noexcept(op(std::declval<I1>(), convert<I2>(std::move(p2))))) {
    I1 temp(p1);
    op(temp, convert<I2>(std::move(p2)));
    return temp;
  }
  // Return type conversion
  template <typename R=RT> static constexpr14
  typename std::enable_if<not std::is_same<RT,I1>::value, R>::type
  func(const P1&  p1,       P2&& p2, OP op=OP{}) noexcept(
      std::is_nothrow_constructible<I1,const P1&>::value and
      std::is_nothrow_constructible<RT,I1&&>::value and
      noexcept(op(std::declval<I1>(), convert<I2>(std::move(p2))))) {
    I1 temp(p1);
    op(temp, convert<I2>(std::move(p2)));
    return static_cast<RT>(std::move(temp));
  }
};

template <typename OP, typename RT,
         typename P1, typename P2, typename I1, typename I2>
struct overload<OP,RT,false,      P1&&,const P2& ,I1,I2> {
  static constexpr RT func(      P1&& p1, const P2&  p2, OP op=OP{}) noexcept(
    noexcept(static_cast<RT>(
        op(convert_mutable<I1>(std::move(p1)), convert<I2>(          p2 ))))) {
    return   static_cast<RT>(
        op(convert_mutable<I1>(std::move(p1)), convert<I2>(          p2 ))); }
};

template <typename OP, typename RT,
         typename P1, typename P2, typename I1, typename I2>
struct overload<OP,RT,false,      P1&&,      P2&&,I1,I2> {
  static constexpr RT func(      P1&& p1,       P2&& p2, OP op=OP{}) noexcept(
    noexcept(static_cast<RT>(
        op(convert_mutable<I1>(std::move(p1)), convert<I2>(std::move(p2)))))) {
    return   static_cast<RT>(
        op(convert_mutable<I1>(std::move(p1)), convert<I2>(std::move(p2)))); }
};

template <typename OP, typename RT,
         typename P1, typename P2, typename I1, typename I2>
struct overload<OP,RT,true,P1,P2,I1,I2> {
  using overload_t = overload<OP,RT,false,P2,P1,I2,I1>;
  static constexpr RT func(P1&& p1, P2&& p2, OP op=OP{}) noexcept(noexcept(
           overload_t::func(
             opaque::forward<P2>(p2), opaque::forward<P1>(p1), op))) {
    return overload_t::func(
        opaque::forward<P2>(p2), opaque::forward<P1>(p1), op);
  }
};

//
// Four flavors of overload are required, but we cannot inherit from the same
// base class more than once (ISO/IEC 14882 §10.1/3).  Work around this by
// creating four distinct classes to inherit from.
//

template <typename OP, typename RT, bool apply_commutativity,
         typename P1, typename P2, typename I1, typename I2>
struct overload_1 : overload<OP,RT,apply_commutativity,P1,P2,I1,I2> { };

template <typename OP, typename RT, bool apply_commutativity,
         typename P1, typename P2, typename I1, typename I2>
struct overload_2 : overload<OP,RT,apply_commutativity,P1,P2,I1,I2> { };

template <typename OP, typename RT, bool apply_commutativity,
         typename P1, typename P2, typename I1, typename I2>
struct overload_3 : overload<OP,RT,apply_commutativity,P1,P2,I1,I2> { };

template <typename OP, typename RT, bool apply_commutativity,
         typename P1, typename P2, typename I1, typename I2>
struct overload_4 : overload<OP,RT,apply_commutativity,P1,P2,I1,I2> { };

///
/// Conversion costs associated with a binary operation
///
/// The parameter types must reflect the actual types passed to the binary
/// operation, e.g. const T& or T&&.  All the other types must be decayed
/// types.
///
template<typename return_type, typename result_type,
         typename inter1_type, typename inter2_type,
         typename param1_type, typename param2_type>
static constexpr unsigned binop_conversion_cost() noexcept {
  static_assert(is_decayed<return_type>::value, "");
  static_assert(is_decayed<result_type>::value, "");
  static_assert(is_decayed<inter1_type>::value, "");
  static_assert(is_decayed<inter2_type>::value, "");
  return converter<inter1_type,param1_type>::mutable_cost() +
         converter<inter2_type,param2_type>::cost() +
         converter<return_type,result_type>::cost();
}

///
/// Select between the regular overload and the commutative alternative
///
/// If the operator is not commutative, use the regular overload.
/// Otherwise, if only one option is well-formed, use that.
/// Otherwise, if both are well-formed, select the lower-cost one.
///

template <bool regular_well_formed, bool swapped_well_formed,
         typename OPN, typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2,
         typename OPS=OPN>
struct overload_selector {
  // overload_selector<false,false,OPN,RT,commutative,P1,P2,I1,I2,OPS>
  // --> No overload is well-formed
  static_assert(    swapped_well_formed, "Operation is not well-formed");
  // overload_selector<false,true ,OPN,RT,false,P1,P2,I1,I2,OPS>>
  // --> Only swapped overload is well-formed, but we can't use it
  static_assert(not swapped_well_formed, "Operation is not commutative");
};

template <typename OPN, typename RT,
         typename P1, typename P2, typename I1, typename I2, typename OPS>
struct overload_selector<false,true ,OPN,RT,true ,P1,P2,I1,I2,OPS> {
  // Only swapped overload is well-formed, and we can use it
  using type_1 = overload_1<OPS,RT,true ,const P1& , const P2& ,I1,I2>;
  using type_2 = overload_2<OPS,RT,true ,const P1& ,       P2&&,I1,I2>;
  using type_3 = overload_3<OPS,RT,true ,      P1&&, const P2& ,I1,I2>;
  using type_4 = overload_4<OPS,RT,true ,      P1&&,       P2&&,I1,I2>;
};

template <typename OPN, typename RT, bool commutative,
         typename P1, typename P2, typename I1, typename I2, typename OPS>
struct overload_selector<true ,false,OPN,RT,commutative,P1,P2,I1,I2,OPS> {
  // Only regular overload is well-formed
  using type_1 = overload_1<OPN,RT,false,const P1& , const P2& ,I1,I2>;
  using type_2 = overload_2<OPN,RT,false,const P1& ,       P2&&,I1,I2>;
  using type_3 = overload_3<OPN,RT,false,      P1&&, const P2& ,I1,I2>;
  using type_4 = overload_4<OPN,RT,false,      P1&&,       P2&&,I1,I2>;
};

template <typename OPN, typename RT,
         typename P1, typename P2, typename I1, typename I2, typename OPS>
struct overload_selector<true ,true ,OPN,RT,false,P1,P2,I1,I2,OPS> {
  // Both overloads are well-formed, but we must use the regular one
  using type_1 = overload_1<OPN,RT,false,const P1& , const P2& ,I1,I2>;
  using type_2 = overload_2<OPN,RT,false,const P1& ,       P2&&,I1,I2>;
  using type_3 = overload_3<OPN,RT,false,      P1&&, const P2& ,I1,I2>;
  using type_4 = overload_4<OPN,RT,false,      P1&&,       P2&&,I1,I2>;
};

template <typename OPN, typename RT,
         typename P1, typename P2, typename I1, typename I2, typename OPS>
struct overload_selector<true ,true ,OPN,RT,true ,P1,P2,I1,I2,OPS> {
  // Both overloads are well-formed, and we must choose between them

  template <typename result, typename param1, typename param2>
  static constexpr unsigned norm_cost() noexcept {
    return binop_conversion_cost<RT,result,I1,I2,param1,param2>();
  }
  template <typename result, typename param1, typename param2>
  static constexpr unsigned swap_cost() noexcept {
    return binop_conversion_cost<RT,result,I2,I1,param2,param1>();
  }

  using RN = typename std::decay<typename
    is_functor_call_well_formed<OPN,I1&,const I2&>::result_type>::type;
  using RS = typename std::decay<typename
    is_functor_call_well_formed<OPS,I2&,const I1&>::result_type>::type;

  using type_1 = typename std::conditional<
    swap_cost           <RS,const P1& ,const P2& >() <
    norm_cost           <RN,const P1& ,const P2& >(),
    overload_1<OPS,RT,true ,const P1& ,const P2& ,I1,I2>,
    overload_1<OPN,RT,false,const P1& ,const P2& ,I1,I2>>::type;
  using type_2 = typename std::conditional<
    swap_cost           <RS,const P1& ,      P2&&>() <
    norm_cost           <RN,const P1& ,      P2&&>(),
    overload_2<OPS,RT,true ,const P1& ,      P2&&,I1,I2>,
    overload_2<OPN,RT,false,const P1& ,      P2&&,I1,I2>>::type;
  using type_3 = typename std::conditional<
    swap_cost           <RS,      P1&&,const P2& >() <
    norm_cost           <RN,      P1&&,const P2& >(),
    overload_3<OPS,RT,true ,      P1&&,const P2& ,I1,I2>,
    overload_3<OPN,RT,false,      P1&&,const P2& ,I1,I2>>::type;
  using type_4 = typename std::conditional<
    swap_cost           <RS,      P1&&,      P2&&>() <
    norm_cost           <RN,      P1&&,      P2&&>(),
    overload_4<OPS,RT,true ,      P1&&,      P2&&,I1,I2>,
    overload_4<OPN,RT,false,      P1&&,      P2&&,I1,I2>>::type;
};

template <typename OPN, typename RT, bool commutative,
         typename P1, typename P2, typename I1, typename I2, typename OPS>
using overload_selector_t = overload_selector<
    is_functor_call_well_formed<OPN,I1&,I2>::value,
    is_functor_call_well_formed<OPS,I2&,I1>::value,
    OPN,RT,commutative,P1,P2,I1,I2,OPS>;

///
/// Generalized binary operator
///
template <typename OPN, typename RT, bool commutative=false,
         typename P1=RT, typename P2=RT, typename I1=P1, typename I2=P2,
         typename OPS=OPN>
struct binary_operator
  : overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_1
  , overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_2
  , overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_3
  , overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_4
{
  //
  // Inheriting a function with the same name from different base classes is
  // ambiguous (ISO/IEC 14882 §10.2/6).  Work around this by pulling the
  // declarations into the derived class.
  //
  using overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_1::func;
  using overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_2::func;
  using overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_3::func;
  using overload_selector_t<OPN,RT,commutative,P1,P2,I1,I2,OPS>::type_4::func;
};

/// @}

}
}

#endif
