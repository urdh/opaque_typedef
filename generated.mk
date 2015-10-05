#
# Copyright (c) 2015
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

#
# These widely-used variables are respected and may be set externally.  On
# some systems they are set by default and may need to be overridden.
#
# AR       - archiver
# ARFLAGS  - archiver flags
# CXX      - compiler
# CPPFLAGS - preprocessor flags
# CXXFLAGS - compiler flags
# LDFLAGS  - linker flags
# LDLIBS   - libraries
#

#
# These variables control the filename of the archive.
#
# NAME - Name of source collection
# VERSION - Version of source collection
#

#
# These variables control compile and link command line options:
#
# TARGET - Option to specify that the subsequent argument is the target
# DEPS   - Option to enable automatic dependency generation
#
# COMMON_FLAG - Universal compiler options
# COMMON_LINK - Universal linker   options
#
#  NORMAL_FLAG - Additional compiler options for the "normal"  style
#  NORMAL_LINK - Additional linker   options for the "normal"  style
#   DEBUG_FLAG - Additional compiler options for the "debug"   style
#   DEBUG_LINK - Additional linker   options for the "debug"   style
# PROFILE_FLAG - Additional compiler options for the "profile" style
# PROFILE_LINK - Additional linker   options for the "profile" style
#
# STATIC_FLAG - Additional compiler options for static linking
# STATIC_LINK - Additional linker   options for static linking
# SHARED_FLAG - Additional compiler options for shared linking
# SHARED_LINK - Additional linker   options for shared linking
#
# REAL_FLAG - Additional compiler options for real output
# REAL_LINK - Additional linker   options for real output
# TEST_FLAG - Additional compiler options for unit tests
# TEST_LINK - Additional linker   options for unit tests
#

#
# Directories
#
# Targets and dependencies are based on a sentinel file in each desired
# directory.  This avoids the problem of depending directly on a directory,
# where operations like adding a file updates the timestamp and causes targets
# to immediately appear out-of-date.
#
# Order-only prerequisites are a better solution, but would require gmake.
#
# A makefile must provide these variables:
#   DIR_SENTINEL is the name of a sentinel file
#   DIR_CREATE is a command to make a directory and its sentinel file
#              (The sentinel file name will be $@)
#   DIR_REMOVE is a command to remove a directory tree
#              (The root of the tree will be appended)
#

normal/example/demo_numeric_typedef.so: normal/example/${DIR_SENTINEL} example/demo_numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/demo_numeric_typedef.cpp
normal/example/tutorial.so: normal/example/${DIR_SENTINEL} example/tutorial.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/tutorial.cpp
normal/test/binop_function.so: normal/test/${DIR_SENTINEL} test/binop_function.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_function.cpp
normal/test/binop_inherit.so: normal/test/${DIR_SENTINEL} test/binop_inherit.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_inherit.cpp
normal/test/binop_overload.so: normal/test/${DIR_SENTINEL} test/binop_overload.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_overload.cpp
normal/test/convert.so: normal/test/${DIR_SENTINEL} test/convert.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/convert.cpp
normal/test/numeric_typedef.so: normal/test/${DIR_SENTINEL} test/numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/numeric_typedef.cpp
normal/test/test/test_evaluator.so: normal/test/test/${DIR_SENTINEL} test/test/test_evaluator.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_evaluator.cpp
normal/test/test/test_ostreamable.so: normal/test/test/${DIR_SENTINEL} test/test/test_ostreamable.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_ostreamable.cpp
normal/test/test/test_result_counter.so: normal/test/test/${DIR_SENTINEL} test/test/test_result_counter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_counter.cpp
normal/test/test/test_result_reporter.so: normal/test/test/${DIR_SENTINEL} test/test/test_result_reporter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_reporter.cpp
normal/test/test/test_test_context.so: normal/test/test/${DIR_SENTINEL} test/test/test_test_context.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_test_context.cpp
normal/test/test/test_type_name.so: normal/test/test/${DIR_SENTINEL} test/test/test_type_name.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_type_name.cpp
normal/test/type_traits.so: normal/test/${DIR_SENTINEL} test/type_traits.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${NORMAL_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/type_traits.cpp
normal/demo_numeric_typedef: normal/${DIR_SENTINEL} normal/example/demo_numeric_typedef.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/example/demo_numeric_typedef.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/tutorial: normal/${DIR_SENTINEL} normal/example/tutorial.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/example/tutorial.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/binop_function: normal/${DIR_SENTINEL} normal/test/binop_function.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/binop_function.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/binop_inherit: normal/${DIR_SENTINEL} normal/test/binop_inherit.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/binop_inherit.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/binop_overload: normal/${DIR_SENTINEL} normal/test/binop_overload.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/binop_overload.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/convert: normal/${DIR_SENTINEL} normal/test/convert.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/convert.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/numeric_typedef: normal/${DIR_SENTINEL} normal/test/numeric_typedef.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/numeric_typedef.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_evaluator: normal/${DIR_SENTINEL} normal/test/test/test_evaluator.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_evaluator.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_ostreamable: normal/${DIR_SENTINEL} normal/test/test/test_ostreamable.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_ostreamable.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_result_counter: normal/${DIR_SENTINEL} normal/test/test/test_result_counter.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_result_counter.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_result_reporter: normal/${DIR_SENTINEL} normal/test/test/test_result_reporter.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_result_reporter.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_test_context: normal/${DIR_SENTINEL} normal/test/test/test_test_context.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_test_context.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/test_type_name: normal/${DIR_SENTINEL} normal/test/test/test_type_name.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/test/test_type_name.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal/type_traits: normal/${DIR_SENTINEL} normal/test/type_traits.so normal/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} normal/test/type_traits.so normal/${NAME}-${VERSION}.so ${COMMON_LINK} ${NORMAL_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
normal_dep = normal/example/demo_numeric_typedef.d normal/example/tutorial.d normal/test/binop_function.d normal/test/binop_inherit.d normal/test/binop_overload.d normal/test/convert.d normal/test/numeric_typedef.d normal/test/test/test_evaluator.d normal/test/test/test_ostreamable.d normal/test/test/test_result_counter.d normal/test/test/test_result_reporter.d normal/test/test/test_test_context.d normal/test/test/test_type_name.d normal/test/type_traits.d
normal_obj = normal/example/demo_numeric_typedef.so normal/example/tutorial.so normal/test/binop_function.so normal/test/binop_inherit.so normal/test/binop_overload.so normal/test/convert.so normal/test/numeric_typedef.so normal/test/test/test_evaluator.so normal/test/test/test_ostreamable.so normal/test/test/test_result_counter.so normal/test/test/test_result_reporter.so normal/test/test/test_test_context.so normal/test/test/test_type_name.so normal/test/type_traits.so
normal_lib = 
normal_bin = normal/demo_numeric_typedef normal/tutorial normal/binop_function normal/binop_inherit normal/binop_overload normal/convert normal/numeric_typedef normal/test_evaluator normal/test_ostreamable normal/test_result_counter normal/test_result_reporter normal/test_test_context normal/test_type_name normal/type_traits
normal/${NAME}-${VERSION}.so: normal/${DIR_SENTINEL} ${normal_lib}
	${AR} ${ARFLAGS} $@ ${normal_lib}
normal/obj: ${normal_obj}
normal/lib: normal/${NAME}-${VERSION}.so
normal/bin: ${normal_bin}
normal/clean:
	${DIR_REMOVE} normal/
normal/check: normal/bin
.PHONY: normal/obj normal/lib normal/bin normal/check normal/clean
debug/example/demo_numeric_typedef.so: debug/example/${DIR_SENTINEL} example/demo_numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/demo_numeric_typedef.cpp
debug/example/tutorial.so: debug/example/${DIR_SENTINEL} example/tutorial.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/tutorial.cpp
debug/test/binop_function.so: debug/test/${DIR_SENTINEL} test/binop_function.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_function.cpp
debug/test/binop_inherit.so: debug/test/${DIR_SENTINEL} test/binop_inherit.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_inherit.cpp
debug/test/binop_overload.so: debug/test/${DIR_SENTINEL} test/binop_overload.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_overload.cpp
debug/test/convert.so: debug/test/${DIR_SENTINEL} test/convert.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/convert.cpp
debug/test/numeric_typedef.so: debug/test/${DIR_SENTINEL} test/numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/numeric_typedef.cpp
debug/test/test/test_evaluator.so: debug/test/test/${DIR_SENTINEL} test/test/test_evaluator.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_evaluator.cpp
debug/test/test/test_ostreamable.so: debug/test/test/${DIR_SENTINEL} test/test/test_ostreamable.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_ostreamable.cpp
debug/test/test/test_result_counter.so: debug/test/test/${DIR_SENTINEL} test/test/test_result_counter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_counter.cpp
debug/test/test/test_result_reporter.so: debug/test/test/${DIR_SENTINEL} test/test/test_result_reporter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_reporter.cpp
debug/test/test/test_test_context.so: debug/test/test/${DIR_SENTINEL} test/test/test_test_context.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_test_context.cpp
debug/test/test/test_type_name.so: debug/test/test/${DIR_SENTINEL} test/test/test_type_name.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_type_name.cpp
debug/test/type_traits.so: debug/test/${DIR_SENTINEL} test/type_traits.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${DEBUG_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/type_traits.cpp
debug/demo_numeric_typedef: debug/${DIR_SENTINEL} debug/example/demo_numeric_typedef.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/example/demo_numeric_typedef.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/tutorial: debug/${DIR_SENTINEL} debug/example/tutorial.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/example/tutorial.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/binop_function: debug/${DIR_SENTINEL} debug/test/binop_function.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/binop_function.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/binop_inherit: debug/${DIR_SENTINEL} debug/test/binop_inherit.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/binop_inherit.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/binop_overload: debug/${DIR_SENTINEL} debug/test/binop_overload.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/binop_overload.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/convert: debug/${DIR_SENTINEL} debug/test/convert.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/convert.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/numeric_typedef: debug/${DIR_SENTINEL} debug/test/numeric_typedef.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/numeric_typedef.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_evaluator: debug/${DIR_SENTINEL} debug/test/test/test_evaluator.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_evaluator.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_ostreamable: debug/${DIR_SENTINEL} debug/test/test/test_ostreamable.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_ostreamable.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_result_counter: debug/${DIR_SENTINEL} debug/test/test/test_result_counter.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_result_counter.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_result_reporter: debug/${DIR_SENTINEL} debug/test/test/test_result_reporter.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_result_reporter.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_test_context: debug/${DIR_SENTINEL} debug/test/test/test_test_context.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_test_context.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/test_type_name: debug/${DIR_SENTINEL} debug/test/test/test_type_name.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/test/test_type_name.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug/type_traits: debug/${DIR_SENTINEL} debug/test/type_traits.so debug/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} debug/test/type_traits.so debug/${NAME}-${VERSION}.so ${COMMON_LINK} ${DEBUG_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
debug_dep = debug/example/demo_numeric_typedef.d debug/example/tutorial.d debug/test/binop_function.d debug/test/binop_inherit.d debug/test/binop_overload.d debug/test/convert.d debug/test/numeric_typedef.d debug/test/test/test_evaluator.d debug/test/test/test_ostreamable.d debug/test/test/test_result_counter.d debug/test/test/test_result_reporter.d debug/test/test/test_test_context.d debug/test/test/test_type_name.d debug/test/type_traits.d
debug_obj = debug/example/demo_numeric_typedef.so debug/example/tutorial.so debug/test/binop_function.so debug/test/binop_inherit.so debug/test/binop_overload.so debug/test/convert.so debug/test/numeric_typedef.so debug/test/test/test_evaluator.so debug/test/test/test_ostreamable.so debug/test/test/test_result_counter.so debug/test/test/test_result_reporter.so debug/test/test/test_test_context.so debug/test/test/test_type_name.so debug/test/type_traits.so
debug_lib = 
debug_bin = debug/demo_numeric_typedef debug/tutorial debug/binop_function debug/binop_inherit debug/binop_overload debug/convert debug/numeric_typedef debug/test_evaluator debug/test_ostreamable debug/test_result_counter debug/test_result_reporter debug/test_test_context debug/test_type_name debug/type_traits
debug/${NAME}-${VERSION}.so: debug/${DIR_SENTINEL} ${debug_lib}
	${AR} ${ARFLAGS} $@ ${debug_lib}
debug/obj: ${debug_obj}
debug/lib: debug/${NAME}-${VERSION}.so
debug/bin: ${debug_bin}
debug/clean:
	${DIR_REMOVE} debug/
debug/check: debug/bin
.PHONY: debug/obj debug/lib debug/bin debug/check debug/clean
profile/example/demo_numeric_typedef.so: profile/example/${DIR_SENTINEL} example/demo_numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/demo_numeric_typedef.cpp
profile/example/tutorial.so: profile/example/${DIR_SENTINEL} example/tutorial.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} example/tutorial.cpp
profile/test/binop_function.so: profile/test/${DIR_SENTINEL} test/binop_function.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_function.cpp
profile/test/binop_inherit.so: profile/test/${DIR_SENTINEL} test/binop_inherit.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_inherit.cpp
profile/test/binop_overload.so: profile/test/${DIR_SENTINEL} test/binop_overload.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/binop_overload.cpp
profile/test/convert.so: profile/test/${DIR_SENTINEL} test/convert.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/convert.cpp
profile/test/numeric_typedef.so: profile/test/${DIR_SENTINEL} test/numeric_typedef.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/numeric_typedef.cpp
profile/test/test/test_evaluator.so: profile/test/test/${DIR_SENTINEL} test/test/test_evaluator.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_evaluator.cpp
profile/test/test/test_ostreamable.so: profile/test/test/${DIR_SENTINEL} test/test/test_ostreamable.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_ostreamable.cpp
profile/test/test/test_result_counter.so: profile/test/test/${DIR_SENTINEL} test/test/test_result_counter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_counter.cpp
profile/test/test/test_result_reporter.so: profile/test/test/${DIR_SENTINEL} test/test/test_result_reporter.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_result_reporter.cpp
profile/test/test/test_test_context.so: profile/test/test/${DIR_SENTINEL} test/test/test_test_context.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_test_context.cpp
profile/test/test/test_type_name.so: profile/test/test/${DIR_SENTINEL} test/test/test_type_name.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/test/test_type_name.cpp
profile/test/type_traits.so: profile/test/${DIR_SENTINEL} test/type_traits.cpp
	${CXX} ${TARGET} $@ ${DEPS} ${COMMON_FLAG} ${PROFILE_FLAG} ${SHARED_FLAG} ${TEST_FLAG} ${CPPFLAGS} ${CXXFLAGS} test/type_traits.cpp
profile/demo_numeric_typedef: profile/${DIR_SENTINEL} profile/example/demo_numeric_typedef.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/example/demo_numeric_typedef.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/tutorial: profile/${DIR_SENTINEL} profile/example/tutorial.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/example/tutorial.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/binop_function: profile/${DIR_SENTINEL} profile/test/binop_function.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/binop_function.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/binop_inherit: profile/${DIR_SENTINEL} profile/test/binop_inherit.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/binop_inherit.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/binop_overload: profile/${DIR_SENTINEL} profile/test/binop_overload.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/binop_overload.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/convert: profile/${DIR_SENTINEL} profile/test/convert.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/convert.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/numeric_typedef: profile/${DIR_SENTINEL} profile/test/numeric_typedef.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/numeric_typedef.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_evaluator: profile/${DIR_SENTINEL} profile/test/test/test_evaluator.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_evaluator.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_ostreamable: profile/${DIR_SENTINEL} profile/test/test/test_ostreamable.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_ostreamable.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_result_counter: profile/${DIR_SENTINEL} profile/test/test/test_result_counter.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_result_counter.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_result_reporter: profile/${DIR_SENTINEL} profile/test/test/test_result_reporter.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_result_reporter.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_test_context: profile/${DIR_SENTINEL} profile/test/test/test_test_context.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_test_context.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/test_type_name: profile/${DIR_SENTINEL} profile/test/test/test_type_name.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/test/test_type_name.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile/type_traits: profile/${DIR_SENTINEL} profile/test/type_traits.so profile/${NAME}-${VERSION}.so
	${CXX} ${TARGET} $@ ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} profile/test/type_traits.so profile/${NAME}-${VERSION}.so ${COMMON_LINK} ${PROFILE_LINK} ${SHARED_LINK} ${TEST_LINK} ${LDLIBS}
profile_dep = profile/example/demo_numeric_typedef.d profile/example/tutorial.d profile/test/binop_function.d profile/test/binop_inherit.d profile/test/binop_overload.d profile/test/convert.d profile/test/numeric_typedef.d profile/test/test/test_evaluator.d profile/test/test/test_ostreamable.d profile/test/test/test_result_counter.d profile/test/test/test_result_reporter.d profile/test/test/test_test_context.d profile/test/test/test_type_name.d profile/test/type_traits.d
profile_obj = profile/example/demo_numeric_typedef.so profile/example/tutorial.so profile/test/binop_function.so profile/test/binop_inherit.so profile/test/binop_overload.so profile/test/convert.so profile/test/numeric_typedef.so profile/test/test/test_evaluator.so profile/test/test/test_ostreamable.so profile/test/test/test_result_counter.so profile/test/test/test_result_reporter.so profile/test/test/test_test_context.so profile/test/test/test_type_name.so profile/test/type_traits.so
profile_lib = 
profile_bin = profile/demo_numeric_typedef profile/tutorial profile/binop_function profile/binop_inherit profile/binop_overload profile/convert profile/numeric_typedef profile/test_evaluator profile/test_ostreamable profile/test_result_counter profile/test_result_reporter profile/test_test_context profile/test_type_name profile/type_traits
profile/${NAME}-${VERSION}.so: profile/${DIR_SENTINEL} ${profile_lib}
	${AR} ${ARFLAGS} $@ ${profile_lib}
profile/obj: ${profile_obj}
profile/lib: profile/${NAME}-${VERSION}.so
profile/bin: ${profile_bin}
profile/clean:
	${DIR_REMOVE} profile/
profile/check: profile/bin
.PHONY: profile/obj profile/lib profile/bin profile/check profile/clean
debug/${DIR_SENTINEL}:
	${DIR_CREATE}
debug/example/${DIR_SENTINEL}: debug/${DIR_SENTINEL}
	${DIR_CREATE}
debug/test/${DIR_SENTINEL}: debug/${DIR_SENTINEL}
	${DIR_CREATE}
debug/test/test/${DIR_SENTINEL}: debug/test/${DIR_SENTINEL}
	${DIR_CREATE}
normal/${DIR_SENTINEL}:
	${DIR_CREATE}
normal/example/${DIR_SENTINEL}: normal/${DIR_SENTINEL}
	${DIR_CREATE}
normal/test/${DIR_SENTINEL}: normal/${DIR_SENTINEL}
	${DIR_CREATE}
normal/test/test/${DIR_SENTINEL}: normal/test/${DIR_SENTINEL}
	${DIR_CREATE}
profile/${DIR_SENTINEL}:
	${DIR_CREATE}
profile/example/${DIR_SENTINEL}: profile/${DIR_SENTINEL}
	${DIR_CREATE}
profile/test/${DIR_SENTINEL}: profile/${DIR_SENTINEL}
	${DIR_CREATE}
profile/test/test/${DIR_SENTINEL}: profile/test/${DIR_SENTINEL}
	${DIR_CREATE}
object_dependencies = ${normal_dep} ${debug_dep} ${profile_dep}
.PHONY: everything
everything: ${normal_bin} ${debug_bin} ${profile_bin}
