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
#include "opaque/type_traits.hpp"
#include "arrtest/arrtest.hpp"

using namespace opaque;

UNIT_TEST_MAIN

TEST(is_decayed) {
  CHECK_EQUAL(true , is_decayed<int>::value);
  CHECK_EQUAL(false, is_decayed<int&>::value);
  CHECK_EQUAL(false, is_decayed<int&&>::value);
  CHECK_EQUAL(false, is_decayed<const int>::value);
  CHECK_EQUAL(false, is_decayed<const int&>::value);
  CHECK_EQUAL(false, is_decayed<const int&&>::value);
}

TEST(is_related) {
  struct base { };
  struct derived : base { };
  struct unrelated { };
  CHECK_EQUAL(false, is_related<int      , int      >::value);
  CHECK_EQUAL(true , is_related<unrelated, unrelated>::value);
  CHECK_EQUAL(false, is_related<unrelated, base     >::value);
  CHECK_EQUAL(false, is_related<unrelated, derived  >::value);
  CHECK_EQUAL(false, is_related<base     , unrelated>::value);
  CHECK_EQUAL(true , is_related<base     , base     >::value);
  CHECK_EQUAL(true , is_related<base     , derived  >::value);
  CHECK_EQUAL(false, is_related<derived  , unrelated>::value);
  CHECK_EQUAL(true , is_related<derived  , base     >::value);
  CHECK_EQUAL(true , is_related<derived  , derived  >::value);
}

TEST(is_cast_free) {
  struct base { };
  struct derived : base { };
  struct unrelated { };
  CHECK_EQUAL(true , is_cast_free<int      , int      >::value);
  CHECK_EQUAL(true , is_cast_free<unrelated, unrelated>::value);
  CHECK_EQUAL(false, is_cast_free<unrelated, base     >::value);
  CHECK_EQUAL(false, is_cast_free<unrelated, derived  >::value);
  CHECK_EQUAL(false, is_cast_free<base     , unrelated>::value);
  CHECK_EQUAL(true , is_cast_free<base     , base     >::value);
  CHECK_EQUAL(true , is_cast_free<base     , derived  >::value);
  CHECK_EQUAL(false, is_cast_free<derived  , unrelated>::value);
  CHECK_EQUAL(true , is_cast_free<derived  , base     >::value);
  CHECK_EQUAL(true , is_cast_free<derived  , derived  >::value);
}
