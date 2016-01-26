#
# Copyright (c) 2015, 2016
# Kyle Markley.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the author nor the names of any contributors may be
#    used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

NAME = opaque
VERSION = 1.2.1

COMMON += -Iinclude
COMMON += -std=c++11 -pedantic
COMMON += -Wall -Wextra -Wformat=2 -Winit-self -Wmissing-include-dirs
COMMON += -Wold-style-cast -Wcast-qual -Wcast-align -Wconversion
COMMON += -Wmissing-declarations -Wredundant-decls -Wundef
COMMON += -Wfloat-equal -Wunused -Wsign-promo
COMMON += -Wswitch-default -Wswitch-enum
COMMON += -Woverloaded-virtual
COMMON += -Wshadow -Wdisabled-optimization

COMMON_FLAG += $(COMMON)
COMMON_FLAG += -c

COMMON_LINK += $(COMMON)
COMMON_LINK += -lpthread

 NORMAL_FLAG += -O3
 NORMAL_LINK = -s
  DEBUG_FLAG += -O0 -ggdb3
  DEBUG_LINK =
PROFILE_FLAG += -pg --coverage
PROFILE_LINK = -lgcov

STATIC_FLAG =
STATIC_LINK = -static-libstdc++
SHARED_FLAG = -fPIC
SHARED_LINK =

REAL_FLAG =
REAL_LINK =
TEST_FLAG =
TEST_LINK =

TARGET = -o
DEPS = -MMD

DIR_SENTINEL = .dir
DIR_CREATE   = mkdir -p ${@D} && touch $@
DIR_REMOVE   = rm -rf

DOXYGEN ?= doxygen

#
# Remove default suffix rules and CXXFLAGS
#
.SUFFIXES:
CXXFLAGS=

.PHONY: all lib test clean distclean check

all:   normal/bin
lib:   normal/lib
test:  normal/bin
clean: normal/clean
distclean:
	${DIR_REMOVE} normal debug profile doc
check: test
	normal/test_result_counter
	normal/test_test_context
	normal/test_ostreamable
	normal/test_type_name
	normal/test_result_reporter
	normal/test_evaluator
	normal/type_traits
	normal/convert
	normal/binop_function
	normal/binop_overload
	normal/binop_inherit
	normal/ostream
	normal/numeric_typedef
	normal/inconvertibool
	normal/safer_string_typedef
	normal/string_typedef
	normal/hash

everything: doc

doc: include/opaque/* include/opaque/binop/* include/arrtest/*
	$(DOXYGEN) Doxyfile
