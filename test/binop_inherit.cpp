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

struct underlying : tracing_base {
  underlying& operator-=(const underlying&) { mark(); return *this; }
  underlying& operator&=(const underlying&) { mark(); return *this; }
  underlying& operator^=(const underlying&) { mark(); return *this; }
};

struct opaque_t : data<underlying, opaque_t>
  , binop::subtractable<opaque_t>
  , binop::bitandable<opaque_t>
  , binop::bitxorable<opaque_t, true, opaque_t, opaque_t,
                      underlying, underlying>
{
  explicit opaque_t(underlying u) : data(std::move(u)) { }
  opaque_t() { } // User-provided for const instances without initializer
  opaque_t& operator-=(const opaque_t& peer) {
    value -= peer.value;
    return downcast();
  }
  opaque_t& operator&=(const opaque_t& peer) {
    value &= peer.value;
    return downcast();
  }
  opaque_t& operator^=(const opaque_t& peer) {
    value ^= peer.value;
    return downcast();
  }
};

TEST(show_noncommutative) {
  using T = opaque_t;
  { tracing_base::scope_printer P(std::cout);
    const T l, r;
    auto x = l - r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T l;
    auto x = l - T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T r;
    auto x = T() - r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l, r;
    auto x = l - r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l;
    auto x = l - T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T r;
    auto x = T() - r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    auto x = T() - T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(show_commutative) {
  using T = opaque_t;
  { tracing_base::scope_printer P(std::cout);
    const T l, r;
    auto x = l & r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T l;
    auto x = l & T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // temp / x
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T r;
    auto x = T() & r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l, r;
    auto x = l & r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp / x
      operation::mark               , // operation
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l;
    auto x = l & T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // temp / x
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T r;
    auto x = T() & r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    auto x = T() & T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::mark               , // operation
      operation::move_constructor   , // x
    //operation::destructor         , // x not destroyed yet
      operation::destructor         , // r
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}

TEST(show_converting) {
  using T = opaque_t;
  { tracing_base::scope_printer P(std::cout);
    const T l, r;
    auto x = l ^ r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
      operation::destructor         , // temp
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T l;
    auto x = l ^ T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
    //operation::copy_constructor   , // no temp (commutative)
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
    //operation::destructor         , // no temp (commutative)
      operation::destructor         , // r
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    const T r;
    auto x = T() ^ r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
    //operation::copy_constructor   , // no temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
    //operation::destructor         , // no temp
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l, r;
    auto x = l ^ r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
      operation::copy_constructor   , // temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
      operation::destructor         , // temp
    //operation::destructor         , // r not destroyed yet
    //operation::destructor         , // l not destroyed yet
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T l;
    auto x = l ^ T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
    //operation::copy_constructor   , // no temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
    //operation::destructor         , // no temp
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    T r;
    auto x = T() ^ r;
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
    //operation::copy_constructor   , // no temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
    //operation::destructor         , // no temp
    //operation::destructor         , // r not destroyed yet
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
  { tracing_base::scope_printer P(std::cout);
    auto x = T() ^ T();
    trace_type t = {
      operation::default_constructor, // l
      operation::default_constructor, // r
    //operation::copy_constructor   , // no temp
      operation::mark               , // operation
      operation::move_constructor   , // T ctor argument
      operation::move_constructor   , // data value
      operation::destructor         , // T ctor argument
    //operation::destructor         , // no temp
      operation::destructor         , // r
      operation::destructor         , // l
    };
    CHECK_EQUAL(t.size(), tracing_base::trace.size());
    CHECK_RANGE_EQUAL(t.begin(), tracing_base::trace.begin(), t.size());
  }
}
