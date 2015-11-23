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
#include "opaque/convert.hpp"
#include "arrtest/arrtest.hpp"
#include "tracing_base.hpp"
#include <iostream>

using namespace opaque;

std::vector<tracing_base::operation> tracing_base::trace;
using trace_type = decltype(tracing_base::trace);
using operation = tracing_base::operation;

UNIT_TEST_MAIN

struct foo : tracing_base { };
struct bar : tracing_base {
  bar(const foo& ) noexcept(false) { }
  bar(      foo& ) noexcept(false) { }
  bar(      foo&&) noexcept(false) { }
};
struct base : tracing_base { };
struct derived : base {
  derived() = default;
  derived(const derived& ) = default;
  derived(      derived&&) = default;
  derived(const base& ) noexcept { }
  derived(      base& ) noexcept { }
  derived(      base&&) noexcept { }
};

SUITE(test_convert_mutable) {

  TEST(mutable_foo_const_lvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::copy_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_bar_const_lvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(false, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_up_const_lvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::copy_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_down_const_lvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_constexpr_const_lvalue) {
    using T = int;
    using U = char;
    using C = converter<T,const U&>;
    constexpr U a = 1;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    constexpr auto r = C::convert_mutable(a);
    CHECK_EQUAL(a, r);
  }


  TEST(mutable_foo_lvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_bar_lvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(false, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_up_lvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_down_lvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert_mutable(a));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  // No test mutable_constexpr_lvalue


  TEST(mutable_foo_rvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert_mutable(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_bar_rvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert_mutable(std::move(a)));
    CHECK_EQUAL(false, noexcept(C::convert_mutable(std::move(a))));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_up_rvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert_mutable(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(mutable_down_rvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert_mutable(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert_mutable(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert_mutable(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  // No test mutable_constexpr_rvalue

}

SUITE(test_convert) {

  TEST(foo_const_lvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,const T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(bar_const_lvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(false, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(up_const_lvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,const T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(down_const_lvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,const U&>;
    const U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,const T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(constexpr_const_lvalue) {
    using T = int;
    using U = char;
    using C = converter<T,const U&>;
    constexpr U a = 1;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T>::value);
    constexpr auto r = C::convert(a);
    CHECK_EQUAL(a, r);
  }


  TEST(foo_lvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(bar_lvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(false, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(up_lvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(down_lvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,U&>;
    U a;
    using R = decltype(C::convert(a));
    CHECK_EQUAL(true, noexcept(C::convert(a)));
    CHECK(std::is_same<R,T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  // No test constexpr_lvalue


  TEST(foo_rvalue) {
    using T = foo;
    using U = foo;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(bar_rvalue) {
    using T = bar;
    using U = foo;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert(std::move(a)));
    CHECK_EQUAL(false, noexcept(C::convert(std::move(a))));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::default_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(up_rvalue) {
    using T = base;
    using U = derived;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(down_rvalue) {
    using T = derived;
    using U = base;
    using C = converter<T,U&&>;
    U a;
    using R = decltype(C::convert(std::move(a)));
    CHECK_EQUAL(true, noexcept(C::convert(std::move(a))));
    CHECK(std::is_same<R,T&&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = C::convert(std::move(a));
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  // No test constexpr_rvalue

}

SUITE(test_functions) {

  TEST(convert_mutable) {
    using T = foo;
    using U = foo;
    const U a;
    using R = decltype(convert_mutable<T>(a));
    CHECK_EQUAL(true, noexcept(convert_mutable<T>(a)));
    CHECK(std::is_same<R,T>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = convert_mutable<T>(a);
    CHECK(static_cast<const void *>(std::addressof(a)) != std::addressof(r));
    trace_type t = { operation::copy_constructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());

    constexpr char b = 1;
    constexpr auto c = convert_mutable<char>(b);
    CHECK_EQUAL(b, c);
    constexpr auto d = convert_mutable<char>(2);
    CHECK_EQUAL(2, d);
  }

  TEST(convert) {
    using T = foo;
    using U = foo;
    const U a;
    using R = decltype(convert<T>(a));
    CHECK_EQUAL(true, noexcept(convert<T>(a)));
    CHECK(std::is_same<R,const T&>::value);
    tracing_base::scope_printer P(std::cout);
    auto&& r = convert<T>(a);
    CHECK(static_cast<const void *>(std::addressof(a)) == std::addressof(r));
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());

    constexpr char b = 1;
    constexpr auto c = convert<char>(b);
    CHECK_EQUAL(b, c);
    constexpr auto d = convert<char>(2);
    CHECK_EQUAL(2, d);
  }

  TEST(costs) {
    CHECK_EQUAL(2u, converter<base,const derived& >::mutable_cost());
    CHECK_EQUAL(0u, converter<base,const derived& >::cost());
    CHECK_EQUAL(0u, converter<base,      derived& >::mutable_cost());
    CHECK_EQUAL(0u, converter<base,      derived& >::cost());
    CHECK_EQUAL(0u, converter<base,      derived&&>::mutable_cost());
    CHECK_EQUAL(0u, converter<base,      derived&&>::cost());
    CHECK_EQUAL(2u, converter<bar ,const foo&     >::mutable_cost());
    CHECK_EQUAL(2u, converter<bar ,const foo&     >::cost());
    CHECK_EQUAL(2u, converter<bar ,      foo&     >::mutable_cost());
    CHECK_EQUAL(2u, converter<bar ,      foo&     >::cost());
    CHECK_EQUAL(1u, converter<bar ,      foo&&    >::mutable_cost());
    CHECK_EQUAL(1u, converter<bar ,      foo&&    >::cost());

    const derived a;
    derived b;
    CHECK_EQUAL(2u, convert_mutable_cost<base>(a));
    CHECK_EQUAL(0u, convert_cost        <base>(a));
    CHECK_EQUAL(0u, convert_mutable_cost<base>(b));
    CHECK_EQUAL(0u, convert_cost        <base>(b));
    CHECK_EQUAL(0u, convert_mutable_cost<base>(derived()));
    CHECK_EQUAL(0u, convert_cost        <base>(derived()));
    const foo c;
    foo d;
    CHECK_EQUAL(2u, convert_mutable_cost<bar >(c));
    CHECK_EQUAL(2u, convert_cost        <bar >(c));
    CHECK_EQUAL(2u, convert_mutable_cost<bar >(d));
    CHECK_EQUAL(2u, convert_cost        <bar >(d));
    CHECK_EQUAL(1u, convert_mutable_cost<bar >(foo()));
    CHECK_EQUAL(1u, convert_cost        <bar >(foo()));
  }

}

SUITE(data_overloads) {

  using underlying = base;
  struct opaque_t : data<underlying, opaque_t> { };

  TEST(cmc) {
    const opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert_mutable<underlying>(a);
    }
    trace_type t = { operation::copy_constructor, operation::destructor };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(c_c) {
    const opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert<underlying>(a);
    }
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(cml) {
    opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert_mutable<underlying>(a);
    }
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(c_l) {
    opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert<underlying>(a);
    }
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(cmr) {
    opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert_mutable<underlying>(std::move(a));
    }
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(c_r) {
    opaque_t a;
    { tracing_base::scope_printer P(std::cout);
      auto&& x = convert<underlying>(std::move(a));
    }
    trace_type t = { };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }

  TEST(costs) {
    CHECK_EQUAL(2u, converter<base,const opaque_t& >::mutable_cost());
    CHECK_EQUAL(0u, converter<base,const opaque_t& >::cost());
    CHECK_EQUAL(0u, converter<base,      opaque_t& >::mutable_cost());
    CHECK_EQUAL(0u, converter<base,      opaque_t& >::cost());
    CHECK_EQUAL(0u, converter<base,      opaque_t&&>::mutable_cost());
    CHECK_EQUAL(0u, converter<base,      opaque_t&&>::cost());

    const opaque_t a;
    opaque_t b;
    CHECK_EQUAL(2u, convert_mutable_cost<base>(a));
    CHECK_EQUAL(0u, convert_cost        <base>(a));
    CHECK_EQUAL(0u, convert_mutable_cost<base>(b));
    CHECK_EQUAL(0u, convert_cost        <base>(b));
    CHECK_EQUAL(0u, convert_mutable_cost<base>(opaque_t()));
    CHECK_EQUAL(0u, convert_cost        <base>(opaque_t()));
  }

}
