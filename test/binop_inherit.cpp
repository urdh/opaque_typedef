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
#include "opaque/binop/binop_inherit.hpp"
#include "arrtest/arrtest.hpp"
#include "tracing_base.hpp"
#include <iostream>

using namespace opaque;

std::vector<tracing_base::operation> tracing_base::trace;
using trace_type = decltype(tracing_base::trace);
using operation = tracing_base::operation;

UNIT_TEST_MAIN

// Trace the underlying type to make conversions visible
struct underlying : tracing_base {
  underlying& operator^=(const underlying&) { mark(); return *this; }
};

//
// Scenarios to test
//

template <typename T>
void scenario_constL_constR() {
  tracing_base::scope_printer P(std::cout);
  const T l, r;
  auto x = l ^ r;
}

template <typename T>
void scenario_constL_tempR() {
  tracing_base::scope_printer P(std::cout);
  const T l;
  auto x = l ^ T();
}

template <typename T>
void scenario_tempL_constR() {
  tracing_base::scope_printer P(std::cout);
  const T r;
  auto x = T() ^ r;
}

template <typename T>
void scenario_tempL_tempR() {
  tracing_base::scope_printer P(std::cout);
  auto x = T() ^ T();
}

//
// Types to test
//

struct noncommutative : data<underlying, noncommutative>
  , binop::bitxorable<noncommutative, false> {
  using self_t = noncommutative;
  template <typename T> noncommutative(T&& u) : data(std::forward<T>(u)) { }
  noncommutative() { } // For const instances without initializer
  self_t& operator^=(const self_t& peer) {
    value ^= peer.value;
    return downcast();
  }
};

struct commutative : data<underlying, commutative>
  , binop::bitxorable<commutative, true> {
  using self_t = commutative;
  template <typename T> commutative(T&& u) : data(std::forward<T>(u)) { }
  commutative() { } // For const instances without initializer
  self_t& operator^=(const self_t& peer) {
    value ^= peer.value;
    return downcast();
  }
};

struct operand_converting : data<underlying, operand_converting>
  , binop::bitxorable<operand_converting, true,
                      operand_converting, operand_converting,
                      underlying, underlying> {
  using self_t = operand_converting;
  template <typename T> operand_converting(T&& u) : data(std::forward<T>(u)) { }
  operand_converting() { } // For const instances without initializer
  self_t& operator^=(const self_t& peer) {
    value ^= peer.value;
    return downcast();
  }
};

//
// In all scenarios, the trace will begin with two constructors (for the
// operands) and will end with three destructors (for the returned value and
// the operands).
//

TEST(show_noncommutative) {
  using T = noncommutative;
  {
    scenario_constL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::copy_constructor, // temp / x
      operation::mark,
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_constL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::copy_constructor, // temp / x
      operation::mark,
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // x
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // x
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(show_commutative) {
  using T = commutative;
  {
    scenario_constL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::copy_constructor, // temp / x
      operation::mark,
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_constL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // x
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // x
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // x
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(show_operand_converting) {
  using T = operand_converting;
  {
    scenario_constL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::copy_constructor, // temp (underlying)
      operation::mark,
      operation::move_constructor, // convert to T, elide return
      operation::destructor,       // temp
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_constL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // return by value an underlying from op
      operation::move_constructor, // convert to T, elide return
      operation::destructor,       // anonymous temporary
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_constR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // return by value an underlying from op
      operation::move_constructor, // convert to T, elide return
      operation::destructor,       // anonymous temporary
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  {
    scenario_tempL_tempR<T>();
    trace_type t = {
      operation::default_constructor, operation::default_constructor,
      operation::mark,             // operate on argument
      operation::move_constructor, // return by value an underlying from op
      operation::move_constructor, // convert to T, elide return
      operation::destructor,       // anonymous temporary
      operation::destructor, operation::destructor, operation::destructor,
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}
