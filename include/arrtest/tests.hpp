#ifndef ARR_TEST_TESTS_HPP
#define ARR_TEST_TESTS_HPP
//
// Copyright (c) 2013, 2015
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
//    specific prior written permission.  //
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

#include "test.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#ifdef ARRTEST_FILTER
#include <regex>
#endif

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Test list
///
struct tests {

  ///
  /// Add a test to the test list
  ///
  static void enlist(test * instance) {
    test_list.emplace_back(instance);
  }

  ///
  /// Run a test and report its results
  ///
  static result_counter run_test(result_reporter& r, test * instance) {
    auto test_results = instance->run(r);
    r.report_test(test_results, instance->name);
    return test_results;
  }

  ///
  /// Run the entire test list
  ///
  /// @param reporter The result_reporter to give results to
  /// @return EXIT_SUCCESS or EXIT_FAILURE, suitable to return from main
  ///
  static int run(result_reporter& reporter) {
    result_counter full_results;
    for (auto instance : test_list) {
      auto results = run_test(reporter, instance);
      if (results.successful()) {
        full_results.inc_passed();
      } else {
        full_results.inc_failed();
      }
    }
    reporter.report_full(full_results);
    return full_results.successful() ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  ///
  /// Run the entire test list, reporting to std::cout
  ///
  /// @return EXIT_SUCCESS or EXIT_FAILURE, suitable to return from main
  ///
  static int run() {
    result_reporter reporter{std::cout};
    return run(reporter);
  }

#ifdef ARRTEST_FILTER
  ///
  /// Filter the test list to those tests whose name match the given regex
  ///
  static void filter(const std::regex& regex) {
    auto i = std::remove_if(begin(test_list), end(test_list), [&](test * t) {
        return not std::regex_search(t->name, regex);
      });
    test_list.erase(i, end(test_list));
  }
#endif

  static std::vector<test *> test_list; ///< The test list
};

std::vector<test *> tests::test_list;

///
/// Test Helper
///
/// Enable the instantiation and execution of a test.  For each test, one
/// object of this type is created at global scope by the ::TEST_FIXTURE
/// macro, facilitating the registration of each test in the test list.
///
template <typename T>
struct test_helper : test {

  /// Register the test in the test list
  test_helper(const char * test_name) : test(test_name) {
    tests::enlist(this);
  }

  /// Instantiate an object of the test type and call its run method
  void run(evaluator& evaluator) override {
    T(evaluator).run();
  }
};

///
/// Empty default fixture for tests that do not specify a fixture
///
struct null_fixture { };

///
/// Define a test that does not use a fixture object
///
/// Example usage:
/// \code
/// TEST(name_of_test) {
///   bool expected = true;
///   bool actual   = true;
///   CHECK_EQUAL(expected, actual);
/// }
/// \endcode
///
#define TEST(name) TEST_FIXTURE(name, arr::test::null_fixture)

///
/// Define a test that uses a fixture object
///
/// A test is implemented as a member function of a type derived from the
/// fixture type.  This implies that all members of the fixture type are
/// available in the test function.
///
/// The fixture type must have a default constructor and a destructor.
/// The fixture object is created at the beginning of the test and destroyed
/// at the end of the test.
///
/// When more than one test uses the same fixture type, each test gets an
/// independent instance of the fixture.
///
/// Note that the fixture type must already be declared.
///
/// Example usage:
/// \code
/// struct my_fixture { bool data = true; };
/// TEST(name_of_test, my_fixture) {
///   CHECK_EQUAL(true, data);
/// }
/// \endcode
///
#define TEST_FIXTURE(name, fixture) \
struct test_##name : fixture { \
  test_##name(arr::test::evaluator& _eval_) : evaluator(_eval_) { } \
  void run(); \
  arr::test::evaluator& evaluator; \
}; \
arr::test::test_helper<test_##name> name##_instance(#name); \
void test_##name::run()

///
/// Define a test suite
///
/// This is a placeholder for potential future functionality.  Currently it
/// simply opens a namespace.
///
#define SUITE(name) \
namespace name

///
/// Define the default main() function for a unit test program
///
#ifdef ARRTEST_FILTER
#define UNIT_TEST_MAIN \
int main(int argc, char * argv[]) { \
  if (argc > 1) { \
    auto regex = std::regex(argv[1], \
          std::regex_constants::nosubs, std::regex_constants::ECMAScript); \
    arr::test::tests::filter(regex); \
  } \
  return arr::test::tests::run(); \
}
#else
#define UNIT_TEST_MAIN \
int main() { \
  return arr::test::tests::run(); \
}
#endif

/// @}

}
}

#endif
