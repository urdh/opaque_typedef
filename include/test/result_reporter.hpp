#ifndef ARR_TEST_RESULT_REPORTER_HPP
#define ARR_TEST_RESULT_REPORTER_HPP
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

#include "result_counter.hpp"
#include "test_context.hpp"
#include "ostreamable.hpp"
#include "type_name.hpp"
#include <exception>
#include <chrono>

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Formatted output for test_context
///
inline std::ostream& operator<<(std::ostream& stream, const test_context& c) {
  if (c.name) stream << c.name;
  if (c.name and c.file) stream << " @ ";
  if (c.file) stream << c.file;
  if (c.line) stream << ':' << c.line;
  return stream;
}

///
/// @name Parameter output
/// @{
/// @details
///
/// First output the parameter's type name.  Then, if the parameter can be
/// formatted through a std::ostream, output that representation in single
/// quotation marks.  If the parameter cannot be formatted through a
/// std::ostream, output a message that it is unprintable.
///

template <typename T> inline
typename std::enable_if<    std::is_pointer<T>::value>::type
emit_parameter(std::ostream& stream, const T& t) {
  if (t) {
    stream << type_name(t) << " '" << t << '\'';
  } else {
    stream << type_name(t) << " (nullptr)";
  }
}

template <typename T> inline
typename std::enable_if<not std::is_pointer<T>::value
                        and arrtest_trait::is_ostreamable<T>::value>::type
emit_parameter(std::ostream& stream, const T& t) {
  stream << type_name(t) << " '" << t << '\'';
}

template <typename T> inline
typename std::enable_if<not arrtest_trait::is_ostreamable<T>::value>::type
emit_parameter(std::ostream& stream, const T& t) {
  stream << type_name(t) << " (unprintable)";
}

/// @}

///
/// Result Message Reporter
///
/// This object formats all output from the unit test library.
///
struct result_reporter {

  result_reporter(std::ostream& destination) : stream(destination) { }

  /// Check passed
  template <typename E, typename A>
  void passed(
      const test_context&,
      const E&,
      const A&) {
    // Emit nothing for passing cases
  }

  /// Check failed
  template <typename E, typename A>
  void failed(
      const test_context& context,
      const E& expected,
      const A& actual) {
    stream << "Failed check in test " << context << '\n';
    emit_parameters(expected, actual);
  }

  /// Emit index for a range comparison
  template <typename Count>
  void range_index(Count i) {
    stream << "  mismatch at position " << i << " in range.\n";
  }

  /// Check raised a std::exception
  template <typename E, typename A>
  void raised(
      const test_context& context,
      const E& expected,
      const A& actual,
      const std::exception& exception) {
    stream << "Recovered from exception in test " << context << '\n';
    emit_parameters(expected, actual);
    report_std_exception(exception);
  }

  /// Check raised a non-standard exception
  template <typename E, typename A>
  void raised(
      const test_context& context,
      const E& expected,
      const A& actual,
      const std::exception_ptr&) {
    stream << "Recovered from exception in test " << context << '\n';
    emit_parameters(expected, actual);
    report_mystery_exception();
  }

  /// Check properly raised an exception
  void raised(
      const char *,
      const test_context&) {
    // Emit nothing for passing cases
  }

  /// Check failed to raise an exception
  void unraised(
      const char * type_name,
      const test_context& context) {
    stream << "Failed to raise exception in test " << context << '\n';
    stream << "  expected exception of type " << type_name << '\n';
  }

  /// Unexpected std::exception during test
  void unexpected(
      const test_context& context,
      const std::exception& exception) {
    stream << "Aborted test " << context << '\n';
    report_std_exception(exception);
  }

  /// Unexpected non-standard exception during test
  void unexpected(
      const test_context& context,
      const std::exception_ptr&) {
    stream << "Aborted test " << context << '\n';
    report_mystery_exception();
  }

  /// Elapsed time check passed
  template <typename Duration1, typename Duration2>
  void time_passed(
      const test_context&,
      const Duration1&,
      const Duration2&) {
    // Emit nothing for passing cases
  }

  /// Elapsed time check failed
  template <typename Duration1, typename Duration2>
  void time_failed(
      const test_context& context,
      const Duration1& limit,
      const Duration2& elapsed) {
    stream << "Failed time limit check in test " << context << '\n';
    using ms = std::chrono::milliseconds;
    auto l = std::chrono::duration_cast<ms>(limit).count();
    auto e = std::chrono::duration_cast<ms>(elapsed).count();
    stream << "  limit " << l << "ms, elapsed " << e << "ms\n";
  }

  /// Report results of a test
  void report_test(const result_counter& results, const char * name) {
    if (results.successful()) {
      stream << "Passed checks in test '" << name << "': ";
    } else {
      stream << "FAILED checks in test '" << name << "': ";
    }
    stream << results.passed() << " passed, ";
    stream << results.failed() << " failed, ";
    stream << results.raised() << " raised ";
    stream << '(' << results.total() << " total)";
    stream << '\n';
  }

  /// Report results of all tests
  void report_full(const result_counter& results) {
    stream << '\n';
    stream << "Overall test results: ";
    stream << results.passed() << " passed, ";
    stream << results.failed() << " failed ";
    stream << '(' << results.total() << " total)";
    stream << '\n';
  }

private:

  /// Emit expected and actual parameters
  template <typename E, typename A>
  void emit_parameters(const E& expected, const A& actual) {
    stream << "  expected ";
    emit_parameter(stream, expected);
    stream << '\n';
    stream << "  actual   ";
    emit_parameter(stream, actual);
    stream << '\n';
  }

  /// Report an exception derived from std::exception
  void report_std_exception(const std::exception& exception) {
    stream << "  exception type: " << type_name(exception) << '\n';
    stream << "  with message: " << exception.what() << '\n';
  }

  /// Report an exception not derived from std::exception
  void report_mystery_exception() {
    stream << "  exception type not derived from std::exception\n";
  }

  std::ostream& stream; ///< Output stream to write messages to
};

/// @}

}
}

#endif
