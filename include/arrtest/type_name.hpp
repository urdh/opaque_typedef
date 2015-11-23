#ifndef ARR_TEST_TYPE_NAME_HPP
#define ARR_TEST_TYPE_NAME_HPP
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

#include <cxxabi.h>
#include <typeinfo>
#include <cstdlib>
#include <memory>
#include <string>

/// \file

namespace arr {
namespace test {

/// \addtogroup test
/// @{

///
/// Demangle the given string if it represents a type name
///
inline std::string demangle(const char * type_info_name) {
  int status;
  std::unique_ptr<char, decltype(std::free)*> name{
      abi::__cxa_demangle(type_info_name, nullptr, nullptr, &status),
      std::free};
  if (0 == status) {
    return { name.get() };
  } else {
    return { type_info_name };
  }
}

///
/// Return a string representing the type name of the argument
///
template <typename T> inline std::string type_name(const T& t) {
  return demangle(typeid(t).name());
}

///
/// Return a string representing the type name of the template
///
template <typename T> inline std::string type_name() {
  return demangle(typeid(T).name());
}

/// @}

}
}

#endif
