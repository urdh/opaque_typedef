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
#include "opaque/binop/binop_function.hpp"
#include "opaque/binop/binop_overload.hpp"
#include "arrtest/arrtest.hpp"
#include "tracing_base.hpp"
#include <iostream>

using namespace opaque;

std::vector<tracing_base::operation> tracing_base::trace;
using trace_type = decltype(tracing_base::trace);
using operation = tracing_base::operation;

UNIT_TEST_MAIN

struct foo : tracing_base {
  foo& operator*=(const foo&) noexcept {
    mark();
    return *this;
  }
  foo() { } // User-provided for const instances without initializer
};

struct bar : foo {
  explicit bar(const foo& ) noexcept {
    trace.push_back(operation::copy_constructor);
  }
  explicit bar(      foo&&) noexcept {
    trace.push_back(operation::move_constructor);
  }
};

namespace {

//
// Non-converting implementations that converting overloads must match in
// the non-converting case.
//

foo func_norm(const foo&  p1, const foo&  p2) {
  foo tmp(p1);
  tmp *= p2;
  return tmp;
}

foo func_norm(const foo&  p1,       foo&& p2) {
  foo tmp(p1);
  tmp *= p2;
  return tmp;
}

foo func_norm(      foo&& p1, const foo&  p2) {
  p1 *= p2;
  return std::move(p1);
}

foo func_norm(      foo&& p1,       foo&& p2) {
  p1 *= p2;
  return std::move(p1);
}

foo func_swap(const foo&  p1, const foo&  p2) {
  foo tmp(p2);
  tmp *= p1;
  return tmp;
}

foo func_swap(const foo&  p1,       foo&& p2) {
  p2 *= p1;
  return std::move(p2);
}

foo func_swap(      foo&& p1, const foo&  p2) {
  foo tmp(p2);
  tmp *= p1;
  return tmp;
}

foo func_swap(      foo&& p1,       foo&& p2) {
  p2 *= p1;
  return std::move(p2);
}

}

TEST(norm_ll_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l, r;
      auto x = func_norm(l, r);
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,false,const T& ,const T& ,T,T>;
      const T l, r;
      auto x = over::func(l, r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_lr_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l;
      auto x = func_norm(l, foo());
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,false,const T& ,      T&&,T,T>;
      const T l;
      auto x = over::func(l, foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_rl_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T r;
      auto x = func_norm(foo(), r);
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,false,      T&&,const T& ,T,T>;
      const T r;
      auto x = over::func(foo(), r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_rr_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      auto x = func_norm(foo(), foo());
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,false,      T&&,      T&&,T,T>;
      auto x = over::func(foo(), foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_ll_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l, r;
      auto x = func_swap(l, r);
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,true ,const T& ,const T& ,T,T>;
      const T l, r;
      auto x = over::func(l, r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_lr_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l;
      auto x = func_swap(l, foo());
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,true ,const T& ,      T&&,T,T>;
      const T l;
      auto x = over::func(l, foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_rl_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T r;
      auto x = func_swap(foo(), r);
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,true ,      T&&,const T& ,T,T>;
      const T r;
      auto x = over::func(foo(), r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_rr_no_conversion) {
  using T = foo;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      auto x = func_swap(foo(), foo());
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,T,true ,      T&&,      T&&,T,T>;
      auto x = over::func(foo(), foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}


TEST(norm_ll_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l, r;
      auto x = static_cast<R>(func_norm(l, r));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,false,const T& ,const T& ,T,T>;
      const T l, r;
      auto x = over::func(l, r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_lr_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l;
      auto x = static_cast<R>(func_norm(l, foo()));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,false,const T& ,      T&&,T,T>;
      const T l;
      auto x = over::func(l, foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_rl_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T r;
      auto x = static_cast<R>(func_norm(foo(), r));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,false,      T&&,const T& ,T,T>;
      const T r;
      auto x = over::func(foo(), r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(norm_rr_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      auto x = static_cast<R>(func_norm(foo(), foo()));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,false,      T&&,      T&&,T,T>;
      auto x = over::func(foo(), foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_ll_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l, r;
      auto x = static_cast<R>(func_swap(l, r));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,true ,const T& ,const T& ,T,T>;
      const T l, r;
      auto x = over::func(l, r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_lr_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T l;
      auto x = static_cast<R>(func_swap(l, foo()));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,true ,const T& ,      T&&,T,T>;
      const T l;
      auto x = over::func(l, foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_rl_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      const T r;
      auto x = static_cast<R>(func_swap(foo(), r));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,true ,      T&&,const T& ,T,T>;
      const T r;
      auto x = over::func(foo(), r);
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(swap_rr_ret_conversion) {
  using T = foo;
  using R = bar;
  using kind = binop::multiply_equal_t;
  using trace_t = decltype(tracing_base::trace);
  {
    trace_t expected;
    trace_t actual;
    { tracing_base::scope_printer P(std::cout);
      auto x = static_cast<R>(func_swap(foo(), foo()));
      expected = tracing_base::trace;
    }
    { tracing_base::scope_printer P(std::cout);
      using over = binop::overload<kind,R,true ,      T&&,      T&&,T,T>;
      auto x = over::func(foo(), foo());
      actual = tracing_base::trace;
    }
    CHECK_EQUAL(expected.size(), actual.size());
    CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size());
  }
}

TEST(costs) {
  using binop::binop_conversion_cost;
  using T = int;
  using U = char;
  CHECK_EQUAL(2u, binop_conversion_cost<T,T,T,T,const T& ,const T& >());
  CHECK_EQUAL(2u, binop_conversion_cost<T,T,T,T,const T& ,      T&&>());
  CHECK_EQUAL(0u, binop_conversion_cost<T,T,T,T,      T&&,const T& >());
  CHECK_EQUAL(0u, binop_conversion_cost<T,T,T,T,      T&&,      T&&>());
  CHECK_EQUAL(3u, binop_conversion_cost<U,T,T,T,const T& ,const T& >());
  CHECK_EQUAL(3u, binop_conversion_cost<U,T,T,T,const T& ,      T&&>());
  CHECK_EQUAL(1u, binop_conversion_cost<U,T,T,T,      T&&,const T& >());
  CHECK_EQUAL(1u, binop_conversion_cost<U,T,T,T,      T&&,      T&&>());
  CHECK_EQUAL(5u, binop_conversion_cost<U,T,T,U,const T& ,const T& >());
  CHECK_EQUAL(4u, binop_conversion_cost<U,T,T,U,const T& ,      T&&>());
  CHECK_EQUAL(3u, binop_conversion_cost<U,T,T,U,      T&&,const T& >());
  CHECK_EQUAL(2u, binop_conversion_cost<U,T,T,U,      T&&,      T&&>());
  CHECK_EQUAL(3u, binop_conversion_cost<T,U,U,T,const T& ,const T& >());
  CHECK_EQUAL(3u, binop_conversion_cost<T,U,U,T,const T& ,      T&&>());
  CHECK_EQUAL(2u, binop_conversion_cost<T,U,U,T,      T&&,const T& >());
  CHECK_EQUAL(2u, binop_conversion_cost<T,U,U,T,      T&&,      T&&>());
}
