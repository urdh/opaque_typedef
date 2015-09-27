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
#include "../include/binop/binop_function.hpp"
#include "../include/test/arrtest.hpp"
#include "tracing_base.hpp"
#include <iostream>

using namespace opaque;

std::vector<tracing_base::operation> tracing_base::trace;
using trace_type = decltype(tracing_base::trace);
using operation = tracing_base::operation;

UNIT_TEST_MAIN

TEST(lvalues) {
  int x = 7;
  const int y = 3;
  binop::multiply_equal(x,y);
  CHECK_EQUAL( 7*3, x);
  binop::divide_equal(x,y);
  CHECK_EQUAL(21/3, x);
  binop::modulus_equal(x,y);
  CHECK_EQUAL( 7%3, x);
  binop::add_equal(x,y);
  CHECK_EQUAL( 1+3, x);
  binop::subtract_equal(x,y);
  CHECK_EQUAL( 4-3, x);
  binop::left_shift_equal(x,y);
  CHECK_EQUAL(1<<3, x);
  binop::right_shift_equal(x,y);
  CHECK_EQUAL(8>>3, x);
  binop::bitand_equal(x,y);
  CHECK_EQUAL( 1&3, x);
  binop::bitxor_equal(x,y);
  CHECK_EQUAL( 1^3, x);
  binop::bitor_equal(x,y);
  CHECK_EQUAL( 2|3, x);
}

TEST(rvalues) {
  CHECK_EQUAL( 7*3, binop::multiply_equal(7,3));
  CHECK_EQUAL(21/3, binop::divide_equal(21,3));
  CHECK_EQUAL( 7%3, binop::modulus_equal(7,3));
  CHECK_EQUAL( 1+3, binop::add_equal(1,3));
  CHECK_EQUAL( 4-3, binop::subtract_equal(4,3));
  CHECK_EQUAL(1<<3, binop::left_shift_equal(1,3));
  CHECK_EQUAL(8>>3, binop::right_shift_equal(8,3));
  CHECK_EQUAL( 1&3, binop::bitand_equal(1,3));
  CHECK_EQUAL( 1^3, binop::bitxor_equal(1,3));
  CHECK_EQUAL( 2|3, binop::bitor_equal(2,3));
}

struct foo : tracing_base {
  foo& operator*=(const foo&) noexcept {
    mark();
    return *this;
  }
};

TEST(trace_l) {
  const foo r;
  foo l;
  CHECK(std::is_same<foo&,
      decltype(binop::multiply_equal(l, r))>::value);
  {
    tracing_base::scope_printer P(std::cout);
    binop::multiply_equal(l, r);
    trace_type t = { operation::mark };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    tracing_base::scope_printer P(std::cout);
    foo x = binop::multiply_equal(l, r);
    trace_type t = { operation::mark, operation::copy_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(trace_r) {
  const foo r;
  CHECK(std::is_same<foo,
      decltype(binop::multiply_equal(foo(), r))>::value);
  {
    tracing_base::scope_printer P(std::cout);
    binop::multiply_equal(foo(), r);
    foo::mark();
    trace_type t = { operation::default_constructor, operation::mark,
      operation::move_constructor, operation::destructor,
      operation::destructor, operation::mark };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    tracing_base::scope_printer P(std::cout);
    foo x = binop::multiply_equal(foo(), r);
    foo::mark();
    trace_type t = { operation::default_constructor, operation::mark,
      operation::move_constructor, operation::destructor, operation::mark };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    tracing_base::scope_printer P(std::cout);
    auto&& x = binop::multiply_equal(foo(), r);
    foo::mark();
    trace_type t = { operation::default_constructor, operation::mark,
      operation::move_constructor, operation::destructor, operation::mark };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(functor_int) {
  binop::multiply_equal_t f;
  int x = 7;
  const int y = 3;
  CHECK(std::is_same<int&, decltype(f(x,y))>::value);
  f(x,y);
  CHECK_EQUAL( 7*3, x);
  CHECK_EQUAL( 7*3, f(7,3));
}

TEST(functor_foo) {
  binop::multiply_equal_t f;
  const foo r;
  CHECK(std::is_same<foo, decltype(f(foo(),r))>::value);
  tracing_base::scope_printer P(std::cout);
  foo x = f(foo(),r);
  foo::mark();
  trace_type t = { operation::default_constructor, operation::mark,
    operation::move_constructor, operation::destructor, operation::mark };
  CHECK_EQUAL(t.size(), tracing_base::trace.size());
  CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
}

TEST(oddball) {
  static int x = 3;
  struct odd {
    int& operator*=(const odd&) noexcept { return x; }
  };
  odd l, r;
  binop::multiply_equal_t f;
  CHECK(std::is_same<int&, decltype(f(l    ,r))>::value);
  CHECK(std::is_same<int , decltype(f(odd(),r))>::value);
  auto&& r1 = f(l    ,r);
  auto&& r2 = f(odd(),r);
  CHECK(&x == &r1);
  CHECK(&x != &r2);
  CHECK_EQUAL(3, r1);
  CHECK_EQUAL(3, r2);
  x = 4;
  CHECK_EQUAL(4, r1);
  CHECK_EQUAL(3, r2);
}
