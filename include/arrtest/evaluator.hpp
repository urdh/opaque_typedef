#ifndef ARR_TEST_EVALUATOR_HPP
#define ARR_TEST_EVALUATOR_HPP
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

#include "result_reporter.hpp"
#include "result_counter.hpp"
#include "test_context.hpp"
#include <exception>
#include <cstring>
#include <chrono>

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Check Evaluator
///
/// A test contains a sequence of checks.  The check evaluator performs those
/// checks and records their results.  A check evaluator object is a parameter
/// of every test.
///
struct evaluator {
  evaluator(
      result_reporter& reporter,
      result_counter& counter,
      test_context& context) noexcept
    : begin(std::chrono::high_resolution_clock::now())
    , _reporter(reporter)
    , _counter(counter)
    , _context(context)
    { }

  /// Update the context of the test
  void operator()(source_point sp) noexcept {
    if (1 == _context.size()) { // Don't replace test name
      sp.name = _context.back().name;
    }
    _context.back() = sp;
  }

  /// Create a framer
  framer<test_context> call_frame(source_point sp) {
    operator()(sp);
    return framer<test_context>(_context, sp);
  }

  /// Check whether the argument is true
  template <typename A>
  void check(const A& actual) noexcept {
    try {
      if (actual) {
        _counter.inc_passed();
        _reporter.passed(_context, true, actual);
      } else {
        _counter.inc_failed();
        _reporter.failed(_context, true, actual);
      }
    } catch (const std::exception& exception) {
      _counter.inc_raised();
      _reporter.raised(_context, true, actual, exception);
    } catch (...) {
      _counter.inc_raised();
      _reporter.raised(_context, true, actual, std::current_exception());
    }
  }

  /// Check whether two values are equal according to operator==
  template <typename E, typename A>
  void equal(const E& expected, const A& actual) noexcept {
    try {
      if (expected == actual) {
        _counter.inc_passed();
        _reporter.passed(_context, expected, actual);
      } else {
        _counter.inc_failed();
        _reporter.failed(_context, expected, actual);
      }
    } catch (const std::exception& exception) {
      _counter.inc_raised();
      _reporter.raised(_context, expected, actual, exception);
    } catch (...) {
      _counter.inc_raised();
      _reporter.raised(_context, expected, actual, std::current_exception());
    }
  }

  /// Check whether two iterator ranges are equal according to operator==
  template <typename Eiter, typename Aiter, typename Count>
  void range_equal(Eiter expected, Aiter actual, Count count) noexcept {
    for (Count i=0; i<count; ++i, ++expected, ++actual) {
      equal(*expected, *actual);
      if (not (*expected == *actual)) {
        _reporter.range_index(i);
      }
    }
  }

  /// Check whether two values are close according to operator<
  template <typename E, typename A, typename T>
  void close(const E& expected, const A& actual, const T& tolerance) noexcept {
    try {
      if (expected-tolerance < actual and actual < expected+tolerance) {
        _counter.inc_passed();
        _reporter.passed(_context, expected, actual);
      } else {
        _counter.inc_failed();
        _reporter.failed(_context, expected, actual);
      }
    } catch (const std::exception& exception) {
      _counter.inc_raised();
      _reporter.raised(_context, expected, actual, exception);
    } catch (...) {
      _counter.inc_raised();
      _reporter.raised(_context, expected, actual, std::current_exception());
    }
  }

  ///
  /// Check whether two C-style strings have the same content
  ///
  /// Note that evaluator::equal will compare the raw pointer values.
  ///
  void strings(const char * expected, const char * actual) noexcept {
    if (nullptr != expected and nullptr != actual and
        0 == std::strcmp(expected, actual)) {
      _counter.inc_passed();
      _reporter.passed(_context, expected, actual);
    } else {
      _counter.inc_failed();
      _reporter.failed(_context, expected, actual);
    }
  }

  /// Check whether the elapsed time of the test is less than the given limit
  template <typename Rep, typename Period>
  void time(const std::chrono::duration<Rep,Period>& limit) {
    auto elapsed = std::chrono::high_resolution_clock::now() - begin;
    if (elapsed < limit) {
      _counter.inc_passed();
      _reporter.time_passed(_context, limit, elapsed);
    } else {
      _counter.inc_failed();
      _reporter.time_failed(_context, limit, elapsed);
    }
  }

  /// Record that an exception was raised properly
  void raised(const char * type_name) {
    _counter.inc_passed();
     _reporter.raised(type_name, _context);
  }

  /// Record that an exception should have been raised, but was not
  void unraised(const char * type_name) {
    _counter.inc_failed();
    _reporter.unraised(type_name, _context);
  }

private:
  /// Time at the beginning of the test
  const decltype(std::chrono::high_resolution_clock::now()) begin;
  /// Reporter to create messages about the execution of the test
  result_reporter& _reporter;
  /// Accumulated results of the test's checks
  result_counter& _counter;
  /// Current context within the test
  test_context& _context;
};

///
/// Check whether (actual) argument is true
///
/// Example usage:
/// \code
/// CHECK(object.member_function());
/// \endcode
///
#define CHECK evaluator(SOURCE_POINT); evaluator.check

///
/// Check whether (expected, actual) arguments are equal using operator==
///
/// Example usage:
/// \code
/// CHECK_EQUAL(15, object.member_function());
/// \endcode
///
#define CHECK_EQUAL evaluator(SOURCE_POINT); evaluator.equal

///
/// Check whether a sequence of values are equal
///
/// The parameters are (expected_begin_iter, actual_begin_iter, count).
///
/// Example usage:
/// \code
/// CHECK_EQUAL(expected.size(), actual.size())
/// CHECK_RANGE_EQUAL(expected.begin(), actual.begin(), expected.size())
/// \endcode
///
#define CHECK_RANGE_EQUAL evaluator(SOURCE_POINT); evaluator.range_equal

///
/// Check whether the arguments are close to equal using operator<
///
/// The parameters are (expected, actual, tolerance).
///
/// Example usage:
/// \code
/// CHECK_CLOSE(15.0, object.member_function(), 0.01);
/// \endcode
///
#define CHECK_CLOSE evaluator(SOURCE_POINT); evaluator.close

///
/// Check whether (expected, actual) C-style strings have the same content
///
/// Example usage:
/// \code
/// const char greeting[] = "Hello, world!";
/// CHECK_STRINGS("Hello, world!", greeting);
/// \endcode
///
#define CHECK_STRINGS evaluator(SOURCE_POINT); evaluator.strings

///
/// Check whether the elapsed time of the test is less than the given limit
///
/// Example usage:
/// \code
/// CHECK_TIME(std::chrono::milliseconds{5});
/// \endcode
///
#define CHECK_TIME evaluator(SOURCE_POINT); evaluator.time

///
/// Check that an exception of the given type is thrown, and catch it
///
/// To use this macro, first open a \c try block, then write the statement
/// that should throw, then use this macro.  After the macro, the context is
/// a \c catch block, which can be used to check properties of the exception
/// object.
///
/// The macro arguments are a type name and a variable name.  The exception
/// will be caught by reference.  (Do not include a \c & token in the type
/// name.)
///
/// Example usage:
/// \code
/// try {
///   throwing_statement;
///   CHECK_CATCH(std::runtime_error, caught);
///   CHECK_STRINGS("Oh no", caught.what());
/// }
/// \endcode
///
#define CHECK_CATCH(type_name, variable) \
  evaluator(SOURCE_POINT); \
  evaluator.unraised(#type_name); \
} catch (type_name& variable) { \
  evaluator.raised(#type_name); \

///
/// Include stack trace for test subroutine
///
/// Exammple usage:
/// \code
/// void void_func(arr::test::evaluator& evaluator) {
///   CHECK(true);
/// }
/// bool bool_func(arr::test::evaluator& evaluator) {
///   CHECK(true);
///   return true;
/// }
/// TEST(foo) {
///   TEST_CALL void_func(evaluator);
///   bool ret = TEST_CALL bool_func(evaluator);
/// }
/// \endcode
///
#define TEST_CALL evaluator.call_frame(SOURCE_POINT) ? throw 0 :

/// @}

}
}

#endif
