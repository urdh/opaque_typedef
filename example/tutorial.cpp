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
#include "opaque/ostream.hpp"
#include <iostream>

struct myint : opaque::numeric_typedef<int, myint> {
  using base = opaque::numeric_typedef<int, myint>;
  using base::base;
};

template <typename T>
struct offset : opaque::numeric_typedef<T, offset<T>> {
  using base  = opaque::numeric_typedef<T, offset<T>>;
  using base::base;
};

template <typename T>
struct address : opaque::numeric_typedef_base<T, address<T>>
  , opaque::binop::addable     <address<T>, true , address<T>,  offset<T>>
  , opaque::binop::addable     <address<T>, true ,  offset<T>, address<T>>
  , opaque::binop::subtractable<address<T>, false, address<T>,  offset<T>>
  , opaque::binop::subtractable<offset<T> , false, address<T>, address<T>, T, T>
{
  using base   = opaque::numeric_typedef_base<T, address<T>>;
  using base::base;
  address& operator*=(const address&) = delete;
  address& operator+=(const address&) = delete;
  address& operator-=(const address&) = delete;
  address& operator+=(const offset<T>& o) {
    this->value += o.value;
    return *this;
  }
  address& operator-=(const offset<T>& o) {
    this->value -= o.value;
    return *this;
  }
};

//
// Same as address, but using opaque::experimental::point_typedef to define
// the + and - operators, and get the desired flavors of += and -=.
//
#include "opaque/experimental/point_typedef.hpp"
template <typename T>
struct alt_address
  : opaque::experimental::point_typedef<offset<T>, alt_address<T>>
{
  using base = opaque::experimental::point_typedef<offset<T>, alt_address<T>>;
  using base::base;
  alt_address& operator*=(const alt_address&) = delete;
};

int main() {
  myint x(1);
  std::cout << "myint(1) << 4: " << (x<<4) << "\n";
  constexpr offset<unsigned> o(3u);
  address<unsigned> a(2u);
  constexpr alt_address<unsigned> l(7u);
  auto sum1 = a + o;
  constexpr14 auto sum2 = l + o;
  std::cout << "address(2) + offset(3): " << sum1 << "\n";
  std::cout << "address(7) + offset(3): " << sum2 << "\n";
}
