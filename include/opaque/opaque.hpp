#ifndef OPAQUE_HPP
#define OPAQUE_HPP
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

///
/// Opaque Typedefs
///
/// Types intended for wrapping other types, used instead of simple type
/// aliases.
///
namespace opaque {

///
/// Binary Operators
///
/// Machinery enabling inheritance of configurable operator@ based on
/// operator@=.
///
namespace binop { }

///
/// Experimental Opaque Typedefs
///
/// Opaque typedefs that may be useful but have not been used enough to
/// have confidence that their definition and interface is correct.
///
namespace experimental { }

}

///
/// \defgroup internal Internal implementation details
///
/// Internal implementation details not intended to be directly reused.
///

///
/// \defgroup miscellaneous Miscellaneous Utilities
///
/// Internal implementation details that may be useful to reuse directly.
///

///
/// \defgroup typedefs Opaque typedefs
///

#endif
