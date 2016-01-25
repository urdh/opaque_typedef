//
// Copyright (c) 2016
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
#include "opaque/experimental/safer_string_typedef.hpp"
#include "arrtest/arrtest.hpp"

using namespace std;
using namespace opaque;

UNIT_TEST_MAIN

struct a_string
  : opaque::experimental::safer_string_typedef<std::string, a_string>
{
  using base = opaque::experimental::safer_string_typedef<std::string, a_string>;
  using base::base;
};

SUITE(construction) {
  TEST(ctor_traits) {
    CHECK_EQUAL(true , std::is_constructible<a_string, std::string>::value);
    CHECK_EQUAL(false, std::is_convertible<std::string, a_string>::value);
    CHECK_EQUAL(true , std::is_constructible<a_string, const char *>::value);
    CHECK_EQUAL(false, std::is_convertible<const char *, a_string>::value);
  }

  TEST(ctor_examples) {
    const std::string empty;
    const std::string str("hi");
    a_string a;
    a_string b(str);
    a_string c(std::string("hi"));
    CHECK_EQUAL(a.value, empty);
    CHECK_EQUAL(b, c);
  }
}
