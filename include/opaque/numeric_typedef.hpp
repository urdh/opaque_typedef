#ifndef OPAQUE_NUMERIC_TYPEDEF_HPP
#define OPAQUE_NUMERIC_TYPEDEF_HPP
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
#include "binop/binop_inherit.hpp"
#include "data.hpp"
#include <type_traits>
#include <utility>

namespace opaque {

/// \addtogroup typedefs
/// @{

///
/// Numeric opaque typedef base type
///
/// Same as numeric_typedef, but without providing operator@ in terms of
/// operator@= by default.  (Deriving from this rather than numeric_typedef
/// can avoid the need to remove a provided operator@ via a template
/// specialization.)
///
/// This is a suitable base class when you want to control the interface of
/// your type, including its interoperability with itself and other types.
/// Delete operations that you do not want, and add operations that you do
/// want.
///
/// You may define the behavior of mixed-type operator@ in your subclass by
/// inheriting from the opaque::binop::opname classes and providing the type
/// details.  The list of template arguments for these classes is:
///  -# return type
///  -# commutative (bool)
///  -# type of left operand
///  -# type of right operand
///  -# type to convert left operand to
///  -# type to convert right operand to
///
/// If you do not desire certain standard numeric operations, simply delete
/// them in your subclass.  (Note that it is simpler to delete an unwanted
/// operation than to supply a missing one that is desired.)
///
template <typename V, typename R, typename S = unsigned>
struct numeric_typedef_base : data<V,R> {
  using base = data<V,R>;
  using base::value;
  typedef V value_t;
  typedef R result_t;
  typedef S shift_t;
  typedef numeric_typedef_base<V,R,S> self_t;

  constexpr14 result_t& operator*=(const result_t& peer)
    noexcept(noexcept( value *= peer.value )) {
                       value *= peer.value;
    return downcast(); }

  constexpr14 result_t& operator/=(const result_t& peer)
    noexcept(noexcept( value /= peer.value )) {
                       value /= peer.value;
    return downcast(); }

  constexpr14 result_t& operator%=(const result_t& peer)
    noexcept(noexcept( value %= peer.value )) {
                       value %= peer.value;
    return downcast(); }

  constexpr14 result_t& operator+=(const result_t& peer)
    noexcept(noexcept( value += peer.value )) {
                       value += peer.value;
    return downcast(); }

  constexpr14 result_t& operator-=(const result_t& peer)
    noexcept(noexcept( value -= peer.value )) {
                       value -= peer.value;
    return downcast(); }

  constexpr14 result_t& operator<<=(const shift_t& count)
    noexcept(noexcept( value <<= count )) {
                       value <<= count;
    return downcast(); }

  constexpr14 result_t& operator>>=(const shift_t& count)
    noexcept(noexcept( value >>= count )) {
                       value >>= count;
    return downcast(); }

  constexpr14 result_t& operator&=(const result_t& peer)
    noexcept(noexcept( value &= peer.value )) {
                       value &= peer.value;
    return downcast(); }

  constexpr14 result_t& operator^=(const result_t& peer)
    noexcept(noexcept( value ^= peer.value )) {
                       value ^= peer.value;
    return downcast(); }

  constexpr14 result_t& operator|=(const result_t& peer)
    noexcept(noexcept( value |= peer.value )) {
                       value |= peer.value;
    return downcast(); }


  constexpr14 result_t& operator++()
    noexcept(noexcept( ++value )) {
                       ++value;
    return downcast(); }

  constexpr14 result_t& operator--()
    noexcept(noexcept( --value )) {
                       --value;
    return downcast(); }

  constexpr14 result_t operator++(int) noexcept(noexcept(
        std::declval<self_t&>().operator++()) and
        std::is_nothrow_constructible<result_t, value_t>::value) {
    result_t r(value); operator++(); return r; }

  constexpr14 result_t operator--(int) noexcept(noexcept(
        std::declval<self_t&>().operator--()) and
        std::is_nothrow_constructible<result_t, value_t>::value) {
    result_t r(value); operator--(); return r; }


  constexpr result_t operator+() const
    noexcept(noexcept( result_t(+value) )) {
    return             result_t(+value); }

  constexpr result_t operator-() const
    noexcept(noexcept( result_t(-value) )) {
    return             result_t(-value); }

  constexpr result_t operator~() const
    noexcept(noexcept( result_t(~value) )) {
    return             result_t(~value); }

  constexpr bool     operator!() const
    noexcept(noexcept( !value  )) {
    return             !value;  }

  constexpr bool operator==(const result_t& peer) const
    noexcept(noexcept( value == peer.value )) {
                return value == peer.value; }
  constexpr bool operator!=(const result_t& peer) const
    noexcept(noexcept( value != peer.value )) {
                return value != peer.value; }
  constexpr bool operator< (const result_t& peer) const
    noexcept(noexcept( value <  peer.value )) {
                return value <  peer.value; }
  constexpr bool operator> (const result_t& peer) const
    noexcept(noexcept( value >  peer.value )) {
                return value >  peer.value; }
  constexpr bool operator<=(const result_t& peer) const
    noexcept(noexcept( value <= peer.value )) {
                return value <= peer.value; }
  constexpr bool operator>=(const result_t& peer) const
    noexcept(noexcept( value >= peer.value )) {
                return value >= peer.value; }

  /// Check whether the underlying value is nonzero
  explicit constexpr operator bool() const
    noexcept(noexcept( static_cast<bool>(value) )) {
    return             static_cast<bool>(value); }


  using base::base;
  explicit numeric_typedef_base() = default;
  numeric_typedef_base(const numeric_typedef_base& ) = default;
  numeric_typedef_base(      numeric_typedef_base&&) = default;
  numeric_typedef_base& operator=(const numeric_typedef_base& ) = default;
  numeric_typedef_base& operator=(      numeric_typedef_base&&) = default;
protected:
  ~numeric_typedef_base() = default;
  using base::downcast;
};

///
/// Numeric opaque typedef
///
/// This is a base class providing wrappers for numeric operations on a
/// user-supplied type, which may be user-defined or built-in.  The purpose of
/// this type is to easily enable the creation of several rigorously separate
/// numeric types so that they cannot be accidentally mixed in expressions,
/// implicitly converted, and also to enable overloading.
///
/// For example, you could create separate types for coordinates in each of
/// several dimensions, and it would be a compile-time error to mix those types
/// in a single expression, even if the the same underlying type was used to
/// represent all of them.
///
/// Template arguments for numeric_typedef:
///  -# V : The underlying type holding the value
///  -# R : The result type, your subclass
///  -# S : The right-hand operand type for shift operations
///
template <typename V, typename R, typename S = unsigned>
struct numeric_typedef : numeric_typedef_base<V,R,S>
  , binop::multipliable   <R>
  , binop::dividable      <R>
  , binop::modulable      <R>
  , binop::addable        <R>
  , binop::subtractable   <R>
  , binop::left_shiftable <R, false, R, S>
  , binop::right_shiftable<R, false, R, S>
  , binop::bitandable     <R>
  , binop::bitxorable     <R>
  , binop::bitorable      <R>
{
  using base = numeric_typedef_base<V,R,S>;
  using base::base;
  explicit numeric_typedef() = default;
  numeric_typedef(const numeric_typedef& ) = default;
  numeric_typedef(      numeric_typedef&&) = default;
  numeric_typedef& operator=(const numeric_typedef& ) = default;
  numeric_typedef& operator=(      numeric_typedef&&) = default;
protected:
  ~numeric_typedef() = default;
};

/// @}

}

#endif
