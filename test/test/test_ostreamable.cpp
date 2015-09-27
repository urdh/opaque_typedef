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

#include "../../include/test/arrtest.hpp"
#include <ostream>
#include <vector>

UNIT_TEST_MAIN

struct foo { };
struct bar { };
std::ostream& operator<<(std::ostream&, const bar&);
std::ostream& operator<<(std::ostream&, const std::vector<int>&);
std::ostream& operator<<(std::ostream&, const std::vector<bar>&);

TEST(all) {
  constexpr bool _void = arrtest_trait::is_ostreamable<void>::value;
  constexpr bool _int = arrtest_trait::is_ostreamable<int>::value;
  CHECK_EQUAL(false, _void);
  CHECK_EQUAL(true , _int);
  constexpr bool _foo = arrtest_trait::is_ostreamable<foo>::value;
  constexpr bool _bar = arrtest_trait::is_ostreamable<bar>::value;
  CHECK_EQUAL(false, _foo);
  CHECK_EQUAL(true , _bar);
  using vec_char = std::vector<char>;
  using vec_int  = std::vector<int>;
  using vec_foo  = std::vector<foo>;
  using vec_bar  = std::vector<bar>;
  constexpr bool vc = arrtest_trait::is_ostreamable<vec_char>::value;
  constexpr bool vi = arrtest_trait::is_ostreamable<vec_int>::value;
  constexpr bool vf = arrtest_trait::is_ostreamable<vec_foo>::value;
  constexpr bool vb = arrtest_trait::is_ostreamable<vec_bar>::value;
  CHECK_EQUAL(false, vc);
  CHECK_EQUAL(true , vi);
  CHECK_EQUAL(false, vf);
  CHECK_EQUAL(true , vb);
}
