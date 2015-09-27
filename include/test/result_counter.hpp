#ifndef ARR_TEST_RESULT_COUNTER_HPP
#define ARR_TEST_RESULT_COUNTER_HPP
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

///
/// \ingroup test
/// Counter for results
///
struct result_counter {

  /// Increment success counter
  void inc_passed() noexcept { ++num_passed; }

  /// Increment fail counter
  void inc_failed() noexcept { ++num_failed; }

  /// Increment exception counter
  void inc_raised() noexcept { ++num_raised; }

  /// Number of successes
  unsigned passed() const noexcept { return num_passed; }

  /// Number of failures
  unsigned failed() const noexcept { return num_failed; }

  /// Number of exceptions
  unsigned raised() const noexcept { return num_raised; }

  /// Total number of counts
  unsigned  total() const noexcept { return passed() + failed() + raised(); }

  /// Are the results completely successful?
  bool successful() const noexcept { return passed() == total(); }

private:
  unsigned num_passed = 0; ///< Number of passes
  unsigned num_failed = 0; ///< Number of failures
  unsigned num_raised = 0; ///< Number of exceptions
};

}
}

#endif
