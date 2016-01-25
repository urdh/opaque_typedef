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
#include "opaque/numeric_typedef.hpp"
#include "opaque/experimental/string_typedef.hpp"
#include "opaque/hash.hpp"
#include "arrtest/arrtest.hpp"
#include <unordered_set>

UNIT_TEST_MAIN

struct safe_int : opaque::numeric_typedef<int, safe_int> {
  using base = opaque::numeric_typedef<int, safe_int>;
  using base::base;
};

struct a_string : opaque::experimental::string_typedef<std::string, a_string> {
  using base = opaque::experimental::string_typedef<std::string, a_string>;
  using base::base;
};

OPAQUE_HASHABLE(safe_int)
OPAQUE_HASHABLE(a_string)

TEST(numeric) {
  std::unordered_set<safe_int> s;
  s.emplace(5);
}

TEST(string) {
  std::unordered_set<a_string> s;
  s.emplace("Hello");
}
