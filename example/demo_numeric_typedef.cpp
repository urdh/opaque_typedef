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
#include "opaque/numeric_typedef.hpp"
#include <iostream>

using namespace opaque;

template <typename... TP>
std::ostream&
operator<<(std::ostream& stream, const numeric_typedef_base<TP...>& v) {
  return stream << v.value;
}

//
// Realistic usage test
//
// Define template "address" and "offset" types with the following behavior:
//   A = A + A  error
//   A = A + O  legal
//   A = O + A  legal
//   A = O + O  error
//   O = A + A  error
//   O = A + O  error
//   O = O + A  error
//   O = O + O  legal
//
//   A = A - A  error
//   A = A - O  legal
//   A = O - A  error
//   A = O - O  error
//   O = A - A  legal
//   O = A - O  error
//   O = O - A  error
//   O = O - O  legal
//

template <typename T> struct offset : numeric_typedef<T, offset<T>> {
  using base = numeric_typedef<T, offset<T>>;
  using base::base;
};

template <typename T> struct address : numeric_typedef_base<T, address<T>>
    // A = A + O and A = A - O become supported by operator+= and -= below
  , binop::addable     <address<T>, true , address<T>,  offset<T>>
  , binop::subtractable<address<T>, false, address<T>,  offset<T>>
    // A = O + A is supported by operator+= because + is commutative
  , binop::addable     <address<T>, true ,  offset<T>, address<T>>
    // O = A - A requires conversion to T because we delete -= below
  , binop::subtractable<offset<T> , false, address<T>, address<T>, T, T>
  , binop::bitorable   <address<T>>
{
  using base = numeric_typedef_base<T, address<T>>;
  using base::base;

  // Remove unwanted operations
  address& operator+=(const address&) = delete;
  address& operator-=(const address&) = delete;

  // Add desired operations
  constexpr14 address& operator+=(const offset<T>& o) noexcept(noexcept(
    std::declval<T&>() += o.value )) {
           this->value += o.value;
    return *this;
  }
  constexpr14 address& operator-=(const offset<T>& o) noexcept(noexcept(
    std::declval<T&>() -= o.value )) {
           this->value -= o.value;
    return *this;
  }

};

void do_add();
void do_sub();
void do_commutative_flavors();
void do_other();
unsigned verify_asm_builtin(unsigned, unsigned);
unsigned verify_asm_opaque(unsigned, unsigned);

void do_add() {
  using N = int;
  using A = address<N>;
  using O =  offset<N>;
  A a(0);
  O o(0);
//a = a + a; // error
  a = a + o; // legal
  a = o + a; // legal
//a = o + o; // error
//o = a + a; // error
//o = a + o; // error
//o = o + a; // error
  o = o + o; // legal
  a += o; // legal
}

void do_sub() {
  using N = int;
  using A = address<N>;
  using O =  offset<N>;
  A a(0);
  O o(0);
//a = a - a; // error
  a = a - o; // legal
//a = o - a; // error
//a = o - o; // error
  o = a - a; // legal
//o = a - o; // error
//o = o - a; // error
  o = o - o; // legal
  a -= o; // legal
}

void do_commutative_flavors() {
  using N = int;
  using A = address<N>;
  using O =  offset<N>;
  A a(0);
  O o(0);
  a =   a  +   o ;
  a =   a  + O(0);
  a = A(0) +   o ;
  a = A(0) + O(0);
  a =   o  +   a ;
  a =   o  + A(0);
  a = O(0) +   a ;
  a = O(0) + A(0);
  a =   a  -   o ;
  a =   a  - O(0);
  a = A(0) -   o ;
  a = A(0) - O(0);
}

void do_other() {
  using N = int;
  using A = address<N>;
  using O =  offset<N>;
  N n(0);
  A a(0);
  O o(0);
  n = n | n;
  a = a | a;
  o = o | o;
}

unsigned verify_asm_builtin(unsigned m, unsigned n) {
  using N = unsigned;
  asm(";# builtin begin");
  N a = m;
  N b = n;
  N c = a - b;
  asm(";# builtin end");
  return c;
}

unsigned verify_asm_opaque(unsigned m, unsigned n) {
  using N = unsigned;
  using A = address<N>;
  using O =  offset<N>;
  asm(";# opaque begin");
  A x{m};
  A y{n};
  O z = x - y;
  asm(";# opaque end");
  return static_cast<N>(z);
}

int main() {
  do_add();
  do_sub();
  do_commutative_flavors();
  do_other();
  auto x = verify_asm_builtin(2,1);
  auto y = verify_asm_opaque(2,1);
  static_cast<void>(x+y);

  using address_t = address<unsigned>;
  using offset_t  = offset<unsigned>;
  address_t a(40u);
  offset_t  o(2u);
  std::cout << a+o << "\n";
  constexpr auto A = address_t(5u);
  constexpr auto O = offset_t(5u);
  constexpr14 auto B = A+O;
  std::cout << B << "\n";
}
