#ifndef ARR_TEST_TEST_CONTEXT_HPP
#define ARR_TEST_TEST_CONTEXT_HPP
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

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Container for file name and line number
///
struct file_line {

  constexpr file_line(
      const char * file_name,
      unsigned line_number) noexcept
    : file(file_name)
    , line(line_number)
  { }

  const char * file;    ///< Source file name
  unsigned     line;    ///< Source line number
};

///
/// Create a arr::test::file_line object using the current source code context
///
#define FILE_LINE arr::test::file_line{__FILE__, __LINE__}

///
/// Container for test context
///
struct test_context : file_line {

  constexpr test_context(
      const char * test_name,
      const char * file_name,
      unsigned line_number) noexcept
    : file_line(file_name, line_number)
    , name(test_name)
  { }

  const char * name;    ///< Test name
};

/// @}

}
}

#endif
