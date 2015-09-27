#ifndef TRACING_BASE_HPP
#define TRACING_BASE_HPP
//
// Copyright (c) 2015
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
#include <vector>
#include <ostream>

struct tracing_base {

  enum class operation {
    destructor,
    default_constructor,
    copy_constructor,
    move_constructor,
    copy_assignment,
    move_assignment,
    mark,
  };

  static const char * decode(operation op) noexcept {
    switch (op) {
      case operation::destructor:          return "d-"; break;
      case operation::default_constructor: return "dc"; break;
      case operation::copy_constructor:    return "cc"; break;
      case operation::move_constructor:    return "mc"; break;
      case operation::copy_assignment:     return "c="; break;
      case operation::move_assignment:     return "m="; break;
      case operation::mark:                return "xx"; break;
      default: break;
    }
    return nullptr;
  }

  friend std::ostream& operator<<(std::ostream& stream, const operation& op) {
    return stream << decode(op);
  }

  static std::vector<operation> trace;

  static void clear_trace() noexcept { trace.clear(); }

  static std::ostream& output(std::ostream& stream) {
    for (const auto& element : trace) {
      stream << element;
    }
    return stream;
  }

  struct scope_printer {
    scope_printer(std::ostream& stream) : out(stream) { clear_trace(); }
    ~scope_printer() {
      out << "trace: ";
      output(out);
      out << '\n';
    }
    std::ostream& out;
  };

  ~tracing_base() noexcept {
    trace.push_back(operation::destructor);
  }

  tracing_base() noexcept {
    trace.push_back(operation::default_constructor);
  }

  tracing_base(const tracing_base& ) noexcept {
    trace.push_back(operation::copy_constructor);
  }

  tracing_base(     tracing_base&&) noexcept {
    trace.push_back(operation::move_constructor);
  }

  tracing_base& operator=(const tracing_base& ) noexcept {
    trace.push_back(operation::copy_assignment);
    return *this;
  }

  tracing_base& operator=(      tracing_base&&) noexcept {
    trace.push_back(operation::move_assignment);
    return *this;
  }

  static void mark() noexcept {
    trace.push_back(operation::mark);
  }

};

#endif
