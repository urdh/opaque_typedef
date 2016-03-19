#ifndef OPAQUE_BINOP_FUNCTION_HPP
#define OPAQUE_BINOP_FUNCTION_HPP
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
#include "../utility.hpp"
#include <type_traits>

namespace opaque {
namespace binop {

/// \addtogroup miscellaneous
/// @{

//
// Functions implementing the ordinary operator@= variations
//

template <typename T, typename U>
constexpr auto multiply_equal(T& l, const U& r)
          noexcept(noexcept(l *= r))
                -> decltype(l *= r) {
                     return l *= r; }

template <typename T, typename U>
constexpr auto divide_equal(T& l, const U& r)
          noexcept(noexcept(l /= r))
                -> decltype(l /= r) {
                     return l /= r; }

template <typename T, typename U>
constexpr auto modulus_equal(T& l, const U& r)
          noexcept(noexcept(l %= r))
                -> decltype(l %= r) {
                     return l %= r; }

template <typename T, typename U>
constexpr auto add_equal(T& l, const U& r)
          noexcept(noexcept(l += r))
                -> decltype(l += r) {
                     return l += r; }

template <typename T, typename U>
constexpr auto subtract_equal(T& l, const U& r)
          noexcept(noexcept(l -= r))
                -> decltype(l -= r) {
                     return l -= r; }

template <typename T, typename U>
constexpr auto left_shift_equal(T& l, const U& r)
          noexcept(noexcept(l <<= r))
                -> decltype(l <<= r) {
                     return l <<= r; }

template <typename T, typename U>
constexpr auto right_shift_equal(T& l, const U& r)
          noexcept(noexcept(l >>= r))
                -> decltype(l >>= r) {
                     return l >>= r; }

template <typename T, typename U>
constexpr auto bitand_equal(T& l, const U& r)
          noexcept(noexcept(l &= r))
                -> decltype(l &= r) {
                     return l &= r; }

template <typename T, typename U>
constexpr auto bitxor_equal(T& l, const U& r)
          noexcept(noexcept(l ^= r))
                -> decltype(l ^= r) {
                     return l ^= r; }

template <typename T, typename U>
constexpr auto bitor_equal(T& l, const U& r)
          noexcept(noexcept(l |= r))
                -> decltype(l |= r) {
                     return l |= r; }

//
// These variants accept and modify temporary objects, and return by value.
// This allows them to be used in expressions without named variables, such
// as multiply_equal(T(l),r).
//

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto multiply_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l *= r))) -> typename
           std::decay<decltype(l *= r)>::type {
           return opaque::move(l *= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto divide_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l /= r))) -> typename
           std::decay<decltype(l /= r)>::type {
           return opaque::move(l /= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto modulus_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l %= r))) -> typename
           std::decay<decltype(l %= r)>::type {
           return opaque::move(l %= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto add_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l += r))) -> typename
           std::decay<decltype(l += r)>::type {
           return opaque::move(l += r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto subtract_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l -= r))) -> typename
           std::decay<decltype(l -= r)>::type {
           return opaque::move(l -= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto left_shift_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l <<= r))) -> typename
           std::decay<decltype(l <<= r)>::type {
           return opaque::move(l <<= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto right_shift_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l >>= r))) -> typename
           std::decay<decltype(l >>= r)>::type {
           return opaque::move(l >>= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitand_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l &= r))) -> typename
           std::decay<decltype(l &= r)>::type {
           return opaque::move(l &= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitxor_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l ^= r))) -> typename
           std::decay<decltype(l ^= r)>::type {
           return opaque::move(l ^= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitor_equal(T&& l, const U& r)
noexcept(noexcept(opaque::move(l |= r))) -> typename
           std::decay<decltype(l |= r)>::type {
           return opaque::move(l |= r); }

//
// Functor objects that forward to the appropriate function.
// Note that the function name is qualified to inhibit ADL.
//

#define OPAQUE_BINOP_FORWARD(F) \
template <typename T, typename U> \
constexpr auto operator()(T&& l, U&& r) const noexcept( \
  noexcept(opaque::binop::F(opaque::forward<T>(l), opaque::forward<U>(r)))) -> \
  decltype(opaque::binop::F(opaque::forward<T>(l), opaque::forward<U>(r))) { \
  return   opaque::binop::F(opaque::forward<T>(l), opaque::forward<U>(r)); } \

struct    multiply_equal_t { OPAQUE_BINOP_FORWARD(   multiply_equal) };
struct      divide_equal_t { OPAQUE_BINOP_FORWARD(     divide_equal) };
struct     modulus_equal_t { OPAQUE_BINOP_FORWARD(    modulus_equal) };
struct         add_equal_t { OPAQUE_BINOP_FORWARD(        add_equal) };
struct    subtract_equal_t { OPAQUE_BINOP_FORWARD(   subtract_equal) };
struct  left_shift_equal_t { OPAQUE_BINOP_FORWARD( left_shift_equal) };
struct right_shift_equal_t { OPAQUE_BINOP_FORWARD(right_shift_equal) };
struct      bitand_equal_t { OPAQUE_BINOP_FORWARD(     bitand_equal) };
struct      bitxor_equal_t { OPAQUE_BINOP_FORWARD(     bitxor_equal) };
struct       bitor_equal_t { OPAQUE_BINOP_FORWARD(      bitor_equal) };

#undef OPAQUE_BINOP_FORWARD

/// @}

}
}

#endif
