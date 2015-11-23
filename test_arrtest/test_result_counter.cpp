//
// Copyright (c) 2013
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

UNIT_TEST_MAIN

TEST(constexpr) {
  constexpr arr::test::result_counter c;
  CHECK_EQUAL(0u, c.passed());
  CHECK_EQUAL(0u, c.failed());
  CHECK_EQUAL(0u, c.raised());
  CHECK_EQUAL(0u, c.total());
  CHECK_EQUAL(true , c.successful());
}

TEST(all) {
  arr::test::result_counter c;
  c.inc_passed();
  CHECK_EQUAL(1u, c.passed());
  CHECK_EQUAL(0u, c.failed());
  CHECK_EQUAL(0u, c.raised());
  CHECK_EQUAL(1u, c.total());
  CHECK_EQUAL(true , c.successful());
  c.inc_failed();
  CHECK_EQUAL(1u, c.passed());
  CHECK_EQUAL(1u, c.failed());
  CHECK_EQUAL(0u, c.raised());
  CHECK_EQUAL(2u, c.total());
  CHECK_EQUAL(false, c.successful());
  c.inc_raised();
  CHECK_EQUAL(1u, c.passed());
  CHECK_EQUAL(1u, c.failed());
  CHECK_EQUAL(1u, c.raised());
  CHECK_EQUAL(3u, c.total());
  CHECK_EQUAL(false, c.successful());
}
