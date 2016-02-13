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
#include <iostream>
#include <sstream>

UNIT_TEST_MAIN

TEST(emit_nullptr) {
  std::ostringstream stream;
  arr::test::emit_parameter(stream, nullptr);
  std::cout << stream.str() << std::endl;
  CHECK(std::string::npos != stream.str().find("(unprintable)"));
}

TEST(emit_null_string) {
  std::ostringstream stream;
  const char * c = nullptr;
  arr::test::emit_parameter(stream, c);
  std::cout << stream.str() << std::endl;
  CHECK(std::string::npos != stream.str().find("(nullptr)"));
}

TEST(emit_string) {
  std::ostringstream stream;
  arr::test::emit_parameter(stream, "Hello");
  std::cout << stream.str() << std::endl;
  CHECK(std::string::npos != stream.str().find("'Hello'"));
}

TEST(emit_int) {
  std::ostringstream stream;
  arr::test::emit_parameter(stream, 3);
  std::cout << stream.str() << std::endl;
  CHECK(std::string::npos != stream.str().find("'3'"));
}

TEST(emit_unprintable) {
  std::ostringstream stream;
  arr::test::emit_parameter(stream, std::common_type<int>());
  std::cout << stream.str() << std::endl;
  CHECK(std::string::npos != stream.str().find("(unprintable)"));
}

TEST(basechange) {
  arr::test::test_context c;
  c.emplace_back("foo", "bar", 16);
  std::ostringstream stream;
  stream << std::hex << c << "--" << 16;
  CHECK_EQUAL("foo @ bar:16--10", stream.str());
}
