//
// Copyright (c) 2013, 2015
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
#include <iostream>

UNIT_TEST_MAIN

TEST(evaluator) {
  arr::test::result_reporter reporter(std::cout);
  arr::test::result_counter  counter;
  arr::test::test_context    context{"FAKE", nullptr, 0u};
  arr::test::evaluator       eval(reporter, counter, context);
  eval(arr::test::file_line("hi", 42u));
  CHECK_STRINGS("hi", context.file);
  CHECK_EQUAL  (42u , context.line);
  eval(FILE_LINE); eval.equal(false, false);
  CHECK_EQUAL(1u, counter.passed());
  CHECK_EQUAL(0u, counter.failed());
  eval(FILE_LINE); eval.equal(false, true );
  CHECK_EQUAL(1u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  const char greeting[] = "Hello, world!";
  eval(FILE_LINE); eval.strings("Hello, world!", greeting);
  CHECK_EQUAL(2u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  eval(FILE_LINE); eval.time(std::chrono::seconds{5});
  CHECK_EQUAL(3u, counter.passed());
  CHECK_EQUAL(1u, counter.failed());
  eval(FILE_LINE); eval.time(std::chrono::seconds{0});
  CHECK_EQUAL(3u, counter.passed());
  CHECK_EQUAL(2u, counter.failed());

  // Expansions of CHECK_CATCH macro

  // Exception raised
  try {
    throw false;
  } catch (bool&) {
    eval.raised("bool");
  }
  CHECK_EQUAL(4u, counter.passed());
  CHECK_EQUAL(2u, counter.failed());

  // Exception not raised
  try {
    // throw false;
    eval(FILE_LINE);
    eval.unraised("bool");
  } catch (bool&) {
    eval.raised("bool");
  }
  CHECK_EQUAL(4u, counter.passed());
  CHECK_EQUAL(3u, counter.failed());

  // Actual passing usage
  try {
    throw false;
    CHECK_CATCH(bool, caught);
    CHECK_EQUAL(false, caught);
  }

  eval(FILE_LINE); eval.check(true);
  CHECK_EQUAL(5u, counter.passed());
  CHECK_EQUAL(3u, counter.failed());
  eval(FILE_LINE); eval.check(nullptr);
  CHECK_EQUAL(5u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());

  eval(FILE_LINE); eval.close(10,  9, 2);
  CHECK_EQUAL(6u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());
  eval(FILE_LINE); eval.close(10, 11, 2);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(4u, counter.failed());
  eval(FILE_LINE); eval.close(10,  9, 1);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(5u, counter.failed());
  eval(FILE_LINE); eval.close(10, 11, 1);
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(6u, counter.failed());

  struct explicit_bool {
    explicit operator bool() const { return false; }
  };
  eval(FILE_LINE); eval.check(explicit_bool());
  CHECK_EQUAL(7u, counter.passed());
  CHECK_EQUAL(7u, counter.failed());

}

struct foo { bool data = true; };

TEST_FIXTURE(test_fixture, foo) {
  CHECK_EQUAL(true, data);
}
