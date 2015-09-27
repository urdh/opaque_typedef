#ifndef ARR_TEST_TEST_HPP
#define ARR_TEST_TEST_HPP
//
// Copyright (c) 2013, 2014
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

#include "result_counter.hpp"
#include "test_context.hpp"
#include "result_reporter.hpp"
#include "evaluator.hpp"

/// \file

namespace arr {
namespace test {

///
/// \ingroup test
/// Base class for tests
///
/// This is a base class for all tests, providing an interface for invoking
/// the test.
///
struct test {

  constexpr test(const char * test_name) noexcept
    : name(test_name) { }

  ///
  /// Run the client's test code and configurably catch unexpected exceptions
  ///
  result_counter run(result_reporter& reporter) {
    result_counter counter;
    test_context context{name, nullptr, 0u};
    evaluator eval{reporter, counter, context};
    if (catch_unexpected) {
      try {
        run(eval);
      } catch (const std::exception& exception) {
        counter.inc_raised();
        reporter.unexpected(context, exception);
      } catch (...) {
        counter.inc_raised();
        reporter.unexpected(context, std::current_exception());
      }
    } else {
      run(eval);
    }
    return counter;
  }

  const char * const name; ///< Name of test
  static bool catch_unexpected; ///< Catch unexpected exceptions?

private:

  ///
  /// Run the client's test code
  ///
  virtual void run(evaluator&) = 0;
};

bool test::catch_unexpected = true;

}
}

#endif
