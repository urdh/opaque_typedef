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
#include "../include/binop/binop_inherit.hpp"
#include "../include/test/arrtest.hpp"
#include "tracing_base.hpp"
#include <iostream>

using namespace opaque;

std::vector<tracing_base::operation> tracing_base::trace;
using trace_type = decltype(tracing_base::trace);
using operation = tracing_base::operation;

UNIT_TEST_MAIN

struct foo
  : tracing_base
  , binop::multipliable<foo>
  , binop::subtractable<foo>
{
  foo& operator*=(const foo&) noexcept { mark(); return *this; }
  foo& operator-=(const foo&) noexcept { mark(); return *this; }
};

TEST(show_commutative) {
  using T = foo;
  { tracing_base::scope_printer P(std::cout);
    const T l, r;
    auto x = l * r;
  }
  { tracing_base::scope_printer P(std::cout);
    const T l;
    auto x = l * foo();
  }
  { tracing_base::scope_printer P(std::cout);
    const T r;
    auto x = foo() * r;
  }
  { tracing_base::scope_printer P(std::cout);
    auto x = foo() * foo();
  }
}

TEST(show_noncommutative) {
  using T = foo;
  { tracing_base::scope_printer P(std::cout);
    const T l, r;
    auto x = l - r;
  }
  { tracing_base::scope_printer P(std::cout);
    const T l;
    auto x = l - foo();
  }
  { tracing_base::scope_printer P(std::cout);
    const T r;
    auto x = foo() - r;
  }
  { tracing_base::scope_printer P(std::cout);
    auto x = foo() - foo();
  }
}
