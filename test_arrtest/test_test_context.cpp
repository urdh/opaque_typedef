//
// Copyright (c) 2013, 2016
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

#include "arrtest/arrtest.hpp"
#include <vector>

UNIT_TEST_MAIN

TEST(source_point) {
  constexpr auto sp = arr::test::source_point("hello", "world", 42u);
  CHECK_STRINGS("hello", sp.name);
  CHECK_STRINGS("world", sp.file);
  CHECK_EQUAL  (42u    , sp.line);
}

using stack_type = std::vector<int>;
using framer_type = arr::test::framer<stack_type>;

TEST(framer_statements) {
  stack_type stack;
  {
    framer_type x(stack, 5);
    CHECK_EQUAL(1u, stack.size());
    CHECK_EQUAL( 5, stack.back());
    {
      framer_type y(stack, 6);
      CHECK_EQUAL(2u, stack.size());
      CHECK_EQUAL( 6, stack.back());
    }
    CHECK_EQUAL(1u, stack.size());
    CHECK_EQUAL( 5, stack.back());
  }
  CHECK_EQUAL(0u, stack.size());
}

SUITE(framer_expressions) {

stack_type stack;

inline void func2(arr::test::evaluator& evaluator) {
  CHECK_EQUAL(2u, stack.size());
}

inline void func1(arr::test::evaluator& evaluator) {
  CHECK_EQUAL(1u, stack.size());
  framer_type(stack, 6), func2(evaluator);
  CHECK_EQUAL(1u, stack.size());
}

TEST(calls) {
  CHECK_EQUAL(0u, stack.size());
  framer_type(stack, 5), func1(evaluator);
  CHECK_EQUAL(0u, stack.size());
}

}
