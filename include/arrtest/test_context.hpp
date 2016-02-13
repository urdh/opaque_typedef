#ifndef ARR_TEST_TEST_CONTEXT_HPP
#define ARR_TEST_TEST_CONTEXT_HPP
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

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Source code point
///
struct source_point {

  constexpr source_point(
      const char * label,
      const char * file_name,
      unsigned line_number) noexcept
    : name(label)
    , file(file_name)
    , line(line_number)
  { }

  const char * name;    ///< Label
  const char * file;    ///< Source file name
  unsigned     line;    ///< Source line number
};

///
/// Create a arr::test::source_point object using the current source point
///
#define SOURCE_POINT arr::test::source_point{__func__, __FILE__, __LINE__}

///
/// Temporary object to maintain test call stack information
///
template <typename Container>
struct framer {
  Container& stack;
  bool valid = true;
  framer(Container& c, const typename Container::value_type& v)
    : stack(c) { stack.push_back(v); }
  ~framer() { if (valid) stack.pop_back(); }
  framer(const framer&) = delete;
  framer(framer&& peer) : stack(peer.stack) { valid = false; }
};

/// @}

}
}

#endif
