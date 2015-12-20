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
#include "opaque/inconvertibool.hpp"
#include "arrtest/arrtest.hpp"

using namespace opaque;

UNIT_TEST_MAIN

template <typename To, typename From>
using assignment_t = decltype(std::declval<To&>() = std::declval<From>());

template <typename T, typename F, typename = void>
struct assignment_well_formed : std::false_type { };

template <typename T, typename F>
struct assignment_well_formed<T, F, void_t<assignment_t<T,F>>>
  : std::true_type { };

using ibool = opaque::inconvertibool;

TEST(construction) {
  // Can create an inconvertibool from a bool
  // Cannot create an inconvertibool from an int
  CHECK_EQUAL(true , std::is_constructible<ibool, bool>::value);
  CHECK_EQUAL(false, std::is_constructible<ibool, int >::value);
  bool x = false;
  ibool a(x);      // direct initialization from bool lvalue
  ibool b(false);  // direct initialization from bool rvalue
  ibool c = x;     // copy initialization from bool lvalue
  ibool d = false; // copy initialization from bool rvalue
  CHECK_EQUAL(false, a.value);
  CHECK_EQUAL(false, b.value);
  CHECK_EQUAL(false, c.value);
  CHECK_EQUAL(false, d.value);
}

TEST(assignment_to) {
  // Can assign an inconvertibool from a bool
  // Cannot assign an inconvertibool from an int
  CHECK_EQUAL(true , assignment_well_formed<ibool, bool>::value);
  CHECK_EQUAL(false, assignment_well_formed<ibool, int >::value);
  ibool a;
  a = true;
  CHECK_EQUAL(true, a.value);
}

TEST(assignment_from) {
  // Cannot assign a bool from an inconvertibool
  // Cannot assign an int from an inconvertibool
  CHECK_EQUAL(false, assignment_well_formed<bool, ibool>::value);
  CHECK_EQUAL(false, assignment_well_formed<int , ibool>::value);
}

TEST(comparison) {
  // Can compare an inconvertibool to a bool
  ibool a(true);
  CHECK_EQUAL(true , a == true);
  CHECK_EQUAL(false, a != true);
  CHECK_EQUAL(true , true == a);
  CHECK_EQUAL(false, true != a);
}

TEST(logic) {
  // Can perform logical operations with an inconvertibool and a bool
  ibool a(true);
  CHECK_EQUAL(false, a & false);
  CHECK_EQUAL(true , a ^ false);
  CHECK_EQUAL(false, false & a);
  CHECK_EQUAL(true , false ^ a);
}

TEST(contextual_conversion) {
  // An invertibool is contextually convertible to bool
  ibool a(true);
  CHECK_EQUAL(1, a ? 1 : 2);
}
