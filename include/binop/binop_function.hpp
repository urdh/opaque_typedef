#ifndef OPAQUE_BINOP_FUNCTION_HPP
#define OPAQUE_BINOP_FUNCTION_HPP
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
#include <utility>

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
noexcept(noexcept(std::move(l *= r))) -> typename
        std::decay<decltype(l *= r)>::type {
           return std::move(l *= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto divide_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l /= r))) -> typename
        std::decay<decltype(l /= r)>::type {
           return std::move(l /= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto modulus_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l %= r))) -> typename
        std::decay<decltype(l %= r)>::type {
           return std::move(l %= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto add_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l += r))) -> typename
        std::decay<decltype(l += r)>::type {
           return std::move(l += r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto subtract_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l -= r))) -> typename
        std::decay<decltype(l -= r)>::type {
           return std::move(l -= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto left_shift_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l <<= r))) -> typename
        std::decay<decltype(l <<= r)>::type {
           return std::move(l <<= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto right_shift_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l >>= r))) -> typename
        std::decay<decltype(l >>= r)>::type {
           return std::move(l >>= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitand_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l &= r))) -> typename
        std::decay<decltype(l &= r)>::type {
           return std::move(l &= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitxor_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l ^= r))) -> typename
        std::decay<decltype(l ^= r)>::type {
           return std::move(l ^= r); }

template <typename T, typename U, typename = typename
  std::enable_if<not std::is_lvalue_reference<T>::value>::type>
constexpr auto bitor_equal(T&& l, const U& r)
noexcept(noexcept(std::move(l |= r))) -> typename
        std::decay<decltype(l |= r)>::type {
           return std::move(l |= r); }

//
// Functor objects that forward to the appropriate function
//

struct multiply_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(multiply_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(multiply_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   multiply_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct divide_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(divide_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(divide_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   divide_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct modulus_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(modulus_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(modulus_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   modulus_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct add_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(add_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(add_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   add_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct subtract_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(subtract_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(subtract_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   subtract_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct left_shift_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(left_shift_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(left_shift_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   left_shift_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct right_shift_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(right_shift_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(right_shift_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   right_shift_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct bitand_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(bitand_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(bitand_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   bitand_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct bitxor_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(bitxor_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(bitxor_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   bitxor_equal(std::forward<T>(l), std::forward<U>(r)); }
};

struct bitor_equal_t {
  template <typename T, typename U>
  constexpr auto operator()(T&& l, U&& r) noexcept(
    noexcept(bitor_equal(std::forward<T>(l), std::forward<U>(r)))) ->
    decltype(bitor_equal(std::forward<T>(l), std::forward<U>(r))) {
    return   bitor_equal(std::forward<T>(l), std::forward<U>(r)); }
};

/// @}

}
}

#endif
