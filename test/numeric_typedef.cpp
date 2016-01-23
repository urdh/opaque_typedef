//
// Copyright (c) 2015, 2016
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
#include "opaque/numeric_typedef.hpp"
#include "arrtest/arrtest.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace opaque;

UNIT_TEST_MAIN

template <typename... TP>
std::ostream&
operator<<(std::ostream& stream, const numeric_typedef_base<TP...>& v) {
  return stream << v.value;
}

template <typename... TP>
std::istream&
operator>>(std::istream& stream,       numeric_typedef_base<TP...>& v) {
  return stream >> v.value;
}

struct safe_int : public numeric_typedef<int, safe_int> {
  using base = numeric_typedef<int, safe_int>;
  using base::base;
};

SUITE(traits) {
  TEST(size) {
    CHECK_EQUAL(sizeof(int), sizeof(safe_int));
  }
  TEST(primary) {
    CHECK_EQUAL(false, std::is_void<safe_int>::value);
    CHECK_EQUAL(false, std::is_integral<safe_int>::value);
    CHECK_EQUAL(false, std::is_floating_point<safe_int>::value);
    CHECK_EQUAL(false, std::is_array<safe_int>::value);
    CHECK_EQUAL(false, std::is_pointer<safe_int>::value);
    CHECK_EQUAL(false, std::is_lvalue_reference<safe_int>::value);
    CHECK_EQUAL(false, std::is_rvalue_reference<safe_int>::value);
    CHECK_EQUAL(false, std::is_member_object_pointer<safe_int>::value);
    CHECK_EQUAL(false, std::is_member_function_pointer<safe_int>::value);
    CHECK_EQUAL(false, std::is_enum<safe_int>::value);
    CHECK_EQUAL(false, std::is_union<safe_int>::value);
    CHECK_EQUAL(true , std::is_class<safe_int>::value);
    CHECK_EQUAL(false, std::is_function<safe_int>::value);
  }
  TEST(composite) {
    CHECK_EQUAL(false, std::is_reference<safe_int>::value);
    CHECK_EQUAL(false, std::is_arithmetic<safe_int>::value);
    CHECK_EQUAL(false, std::is_fundamental<safe_int>::value);
    CHECK_EQUAL(true , std::is_object<safe_int>::value);
    CHECK_EQUAL(false, std::is_scalar<safe_int>::value);
    CHECK_EQUAL(true , std::is_compound<safe_int>::value);
    CHECK_EQUAL(false, std::is_member_pointer<safe_int>::value);
  }
  TEST(properties) {
    CHECK_EQUAL(false, std::is_const<safe_int>::value);
    CHECK_EQUAL(false, std::is_volatile<safe_int>::value);
#ifdef TEST_TRIVIALITY
  // In gcc 4.x, is_trivial fails due to the opaque::data constructor.  It
  // did not fail for the single-argument form, but fails for the variadic.
    CHECK_EQUAL(true , std::is_trivial<safe_int>::value);
  // Unavailable in gcc 4.x
    CHECK_EQUAL(true , std::is_trivially_copyable<safe_int>::value);
#endif
    CHECK_EQUAL(true , std::is_standard_layout<safe_int>::value);
#ifdef TEST_TRIVIALITY
  // In gcc 4.x, fails because is_trivial fails.
    CHECK_EQUAL(true , std::is_pod<safe_int>::value);
#endif
    CHECK_EQUAL(true , std::is_literal_type<safe_int>::value);
    CHECK_EQUAL(false, std::is_empty<safe_int>::value);
    CHECK_EQUAL(false, std::is_polymorphic<safe_int>::value);
    CHECK_EQUAL(false, std::is_abstract<safe_int>::value);
    CHECK_EQUAL(false, std::is_signed<safe_int>::value);
    CHECK_EQUAL(false, std::is_unsigned<safe_int>::value);
#ifdef TEST_TRIVIALITY
  // Unavailable in gcc 4.x
    CHECK_EQUAL(false, std::is_trivially_constructible<safe_int, int>::value);
    CHECK_EQUAL(true , std::is_trivially_default_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_trivially_copy_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_trivially_move_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_trivially_copy_assignable<safe_int>::value);
    CHECK_EQUAL(true , std::is_trivially_move_assignable<safe_int>::value);
#endif
    CHECK_EQUAL(true , std::is_trivially_destructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_constructible<safe_int, int>::value);
    CHECK_EQUAL(true , std::is_nothrow_default_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_copy_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_move_constructible<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_copy_assignable<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_move_assignable<safe_int>::value);
    CHECK_EQUAL(true , std::is_nothrow_destructible<safe_int>::value);
    CHECK_EQUAL(false, std::has_virtual_destructor<safe_int>::value);
  }
}

SUITE(creation) {

  TEST(ctor) {
    safe_int a(1);
    constexpr safe_int b(2);
    CHECK_EQUAL(1, a.value);
    CHECK_EQUAL(2, b.value);
    bool nothrow = std::is_nothrow_constructible<safe_int, int>::value;
    CHECK_EQUAL(true, nothrow);
  }

  struct multi_arg {
    constexpr multi_arg(char arg_c, int arg_i) : c(arg_c), i(arg_i) { }
    char c;
    int i;
  };

  struct safe_multi : public numeric_typedef<multi_arg, safe_multi> {
    using base = numeric_typedef<multi_arg, safe_multi>;
    using base::base;
  };

  TEST(multi_ctor) {
    CHECK_EQUAL(true, std::is_constructible<safe_multi, char, int>::value);
    constexpr safe_multi m('a', 0);
    CHECK_EQUAL('a', m.value.c);
    CHECK_EQUAL(  0, m.value.i);
  }

  TEST(dtor) {
    CHECK_EQUAL(true , std::is_destructible<safe_int              >::value);
    CHECK_EQUAL(false, std::is_destructible<safe_int::base        >::value);
    CHECK_EQUAL(false, std::is_destructible<safe_int::base::self_t>::value);
  }

}

SUITE(mutation) {

  struct mutation_fixture {
    mutation_fixture() = default;
    safe_int a{10};
    int      e{10};
  };

  TEST_FIXTURE(multiply, mutation_fixture) {
    bool a_noexcept = noexcept(a *= safe_int(3));
    bool e_noexcept = noexcept(e *=          3 );
                               a *= safe_int(3) ;
                               e *=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(divide, mutation_fixture) {
    bool a_noexcept = noexcept(a /= safe_int(3));
    bool e_noexcept = noexcept(e /=          3 );
                               a /= safe_int(3) ;
                               e /=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(modulus, mutation_fixture) {
    bool a_noexcept = noexcept(a %= safe_int(3));
    bool e_noexcept = noexcept(e %=          3 );
                               a %= safe_int(3) ;
                               e %=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(add, mutation_fixture) {
    bool a_noexcept = noexcept(a += safe_int(3));
    bool e_noexcept = noexcept(e +=          3 );
                               a += safe_int(3) ;
                               e +=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(subtract, mutation_fixture) {
    bool a_noexcept = noexcept(a -= safe_int(3));
    bool e_noexcept = noexcept(e -=          3 );
                               a -= safe_int(3) ;
                               e -=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(shl, mutation_fixture) {
    bool a_noexcept = noexcept(a <<= 3u);
    bool e_noexcept = noexcept(e <<= 3u);
                               a <<= 3u ;
                               e <<= 3u ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(shr, mutation_fixture) {
    bool a_noexcept = noexcept(a >>= 3u);
    bool e_noexcept = noexcept(e >>= 3u);
                               a >>= 3u ;
                               e >>= 3u ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(and_equal, mutation_fixture) {
    bool a_noexcept = noexcept(a &= safe_int(8));
    bool e_noexcept = noexcept(e &=          8 );
                               a &= safe_int(8) ;
                               e &=          8  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(xor_equal, mutation_fixture) {
    bool a_noexcept = noexcept(a ^= safe_int(9));
    bool e_noexcept = noexcept(e ^=          9 );
                               a ^= safe_int(9) ;
                               e ^=          9  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(or_equal, mutation_fixture) {
    bool a_noexcept = noexcept(a |= safe_int(3));
    bool e_noexcept = noexcept(e |=          3 );
                               a |= safe_int(3) ;
                               e |=          3  ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(pre_inc, mutation_fixture) {
    bool a_noexcept = noexcept(++a);
    bool e_noexcept = noexcept(++e);
    auto a_return   =          ++a ;
    auto e_return   =          ++e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST_FIXTURE(pre_dec, mutation_fixture) {
    bool a_noexcept = noexcept(--a);
    bool e_noexcept = noexcept(--e);
    auto a_return   =          --a ;
    auto e_return   =          --e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST_FIXTURE(post_inc, mutation_fixture) {
    bool a_noexcept = noexcept(a++);
    bool e_noexcept = noexcept(e++);
    auto a_return   =          a++ ;
    auto e_return   =          e++ ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST_FIXTURE(post_dec, mutation_fixture) {
    bool a_noexcept = noexcept(a--);
    bool e_noexcept = noexcept(e--);
    auto a_return   =          a-- ;
    auto e_return   =          e-- ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

}

SUITE(unary) {

  TEST(plus) {
    const safe_int a(10);
    const int      e(10);
    bool a_noexcept = noexcept(+a);
    bool e_noexcept = noexcept(+e);
    auto a_return   =          +a ;
    auto e_return   =          +e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST(minus) {
    const safe_int a(10);
    const int      e(10);
    bool a_noexcept = noexcept(-a);
    bool e_noexcept = noexcept(-e);
    auto a_return   =          -a ;
    auto e_return   =          -e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST(bitwise_not) {
    const safe_int a(10);
    const int      e(10);
    bool a_noexcept = noexcept(~a);
    bool e_noexcept = noexcept(~e);
    auto a_return   =          ~a ;
    auto e_return   =          ~e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return.value);
  }

  TEST(boolean_not_nonzero) {
    const safe_int a(10);
    const int      e(10);
    bool a_noexcept = noexcept(!a);
    bool e_noexcept = noexcept(!e);
    auto a_return   =          !a ;
    auto e_return   =          !e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST(boolean_not_zero) {
    const safe_int a(0);
    const int      e(0);
    bool a_noexcept = noexcept(!a);
    bool e_noexcept = noexcept(!e);
    auto a_return   =          !a ;
    auto e_return   =          !e ;
    CHECK_EQUAL(e, a.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

}

SUITE(compare) {

  struct compare_fixture {
    compare_fixture() = default;
    const safe_int a{10};
    const safe_int b{11};
    const int      e{10};
    const int      f{11};
  };

  TEST_FIXTURE(equal_same, compare_fixture) {
    bool a_noexcept = noexcept(a==a);
    bool e_noexcept = noexcept(e==e);
    auto a_return   =          a==a ;
    auto e_return   =          true ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(equal_different, compare_fixture) {
    bool a_noexcept = noexcept(a==b);
    bool e_noexcept = noexcept(e==f);
    auto a_return   =          a==b ;
    auto e_return   =          e==f ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(unequal_same, compare_fixture) {
    bool a_noexcept = noexcept(a!=a);
    bool e_noexcept = noexcept(e!=e);
    auto a_return   =          a!=a ;
    auto e_return   =          false;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(unequal_different, compare_fixture) {
    bool a_noexcept = noexcept(a!=b);
    bool e_noexcept = noexcept(e!=f);
    auto a_return   =          a!=b ;
    auto e_return   =          e!=f ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(less_same, compare_fixture) {
    bool a_noexcept = noexcept(a< a);
    bool e_noexcept = noexcept(e< e);
    auto a_return   =          a< a ;
    auto e_return   =          false;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(less_different, compare_fixture) {
    bool a_noexcept = noexcept(a< b);
    bool e_noexcept = noexcept(e< f);
    auto a_return   =          a< b ;
    auto e_return   =          e< f ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(greater_same, compare_fixture) {
    bool a_noexcept = noexcept(a> a);
    bool e_noexcept = noexcept(e> e);
    auto a_return   =          a> a ;
    auto e_return   =          false;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(greater_different, compare_fixture) {
    bool a_noexcept = noexcept(a> b);
    bool e_noexcept = noexcept(e> f);
    auto a_return   =          b> a ;
    auto e_return   =          f> e ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(less_equal_same, compare_fixture) {
    bool a_noexcept = noexcept(a<=a);
    bool e_noexcept = noexcept(e<=e);
    auto a_return   =          a<=a ;
    auto e_return   =          true ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(less_equal_different, compare_fixture) {
    bool a_noexcept = noexcept(a<=b);
    bool e_noexcept = noexcept(e<=f);
    auto a_return   =          a<=b ;
    auto e_return   =          e<=f ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(greater_equal_same, compare_fixture) {
    bool a_noexcept = noexcept(a>=a);
    bool e_noexcept = noexcept(e>=e);
    auto a_return   =          a>=a ;
    auto e_return   =          true ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

  TEST_FIXTURE(greater_equal_different, compare_fixture) {
    bool a_noexcept = noexcept(a>=b);
    bool e_noexcept = noexcept(e>=f);
    auto a_return   =          b>=a ;
    auto e_return   =          f>=e ;
    CHECK_EQUAL(e_noexcept, a_noexcept);
    CHECK_EQUAL(e_return, a_return);
  }

}

SUITE(binary) {

  struct binary_fixture {
    binary_fixture() = default;
    const safe_int a{10};
    const int      e{10};
  };

  TEST_FIXTURE(multiply, binary_fixture) {
    bool a_noexcept = noexcept(a * safe_int(3));
    bool e_noexcept = noexcept(e *          3 );
    auto a_result =            a * safe_int(3) ;
    auto e_result =            e *          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(divide, binary_fixture) {
    bool a_noexcept = noexcept(a / safe_int(3));
    bool e_noexcept = noexcept(e /          3 );
    auto a_result =            a / safe_int(3) ;
    auto e_result =            e /          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(modulus, binary_fixture) {
    bool a_noexcept = noexcept(a % safe_int(3));
    bool e_noexcept = noexcept(e %          3 );
    auto a_result =            a % safe_int(3) ;
    auto e_result =            e %          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(add, binary_fixture) {
    bool a_noexcept = noexcept(a + safe_int(3));
    bool e_noexcept = noexcept(e +          3 );
    auto a_result =            a + safe_int(3) ;
    auto e_result =            e +          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(subtract, binary_fixture) {
    bool a_noexcept = noexcept(a - safe_int(3));
    bool e_noexcept = noexcept(e -          3 );
    auto a_result =            a - safe_int(3) ;
    auto e_result =            e -          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(shl, binary_fixture) {
    bool a_noexcept = noexcept(a << 3);
    bool e_noexcept = noexcept(e << 3);
    auto a_result =            a << 3 ;
    auto e_result =            e << 3 ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(shr, binary_fixture) {
    bool a_noexcept = noexcept(a >> 3);
    bool e_noexcept = noexcept(e >> 3);
    auto a_result =            a >> 3 ;
    auto e_result =            e >> 3 ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(bit_and, binary_fixture) {
    bool a_noexcept = noexcept(a & safe_int(8));
    bool e_noexcept = noexcept(e &          8 );
    auto a_result =            a & safe_int(8) ;
    auto e_result =            e &          8  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(bit_xor, binary_fixture) {
    bool a_noexcept = noexcept(a ^ safe_int(9));
    bool e_noexcept = noexcept(e ^          9 );
    auto a_result =            a ^ safe_int(9) ;
    auto e_result =            e ^          9  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

  TEST_FIXTURE(bit_or, binary_fixture) {
    bool a_noexcept = noexcept(a | safe_int(3));
    bool e_noexcept = noexcept(e |          3 );
    auto a_result =            a | safe_int(3) ;
    auto e_result =            e |          3  ;
    CHECK_EQUAL(e_result, a_result.value);
    CHECK_EQUAL(e_noexcept, a_noexcept);
  }

}

SUITE(chain) {

  TEST(simple) {
    safe_int a(10);
    int      e(10);
    (a *= safe_int(3)) += safe_int(5);
    (e *=          3 ) +=          5 ;
    CHECK_EQUAL(e, a.value);
  }

}

template <typename T, typename U,
         template <typename, typename> class E, typename = void>
struct is_well_formed : std::false_type { };
template <typename T, typename U,
         template <typename, typename> class E>
struct is_well_formed<T,U,E,void_t<E<T,U>>> : std::true_type { };

template <typename T, typename U>
using is_addable_t      = decltype(std::declval<T>() + std::declval<U>());
template <typename T, typename U>
using is_subtractable_t = decltype(std::declval<T>() - std::declval<U>());

template <typename T, typename U>
using is_addable      = is_well_formed<T,U,is_addable_t>;
template <typename T, typename U>
using is_subtractable = is_well_formed<T,U,is_subtractable_t>;

struct coord_x : public numeric_typedef<float, coord_x> {
  using base = numeric_typedef<float, coord_x>;
  using base::base;
};

struct coord_y : public numeric_typedef<float, coord_y> {
  using base = numeric_typedef<float, coord_y>;
  using base::base;
};

SUITE(coordinates) {

  TEST(size) {
    CHECK_EQUAL(sizeof(float), sizeof(coord_x));
  }

  TEST(print) {
    coord_x x1(5.3f);
    coord_x x2(4.7f);
    std::ostringstream s;
    s << x1+x2;
    CHECK_EQUAL("10", s.str());
  }

  TEST(scan) {
    coord_x x(0.0f);
    std::istringstream s("20.0");
    s >> x;
    // Test in terms of float
    CHECK_CLOSE(20.0f, x.value, 0.01f);
    // Test in terms of coord_x
    CHECK_CLOSE(coord_x(20.0f), x, coord_x(0.01f));
  }

  TEST(no_add) {
    coord_x x(1.0f);
    coord_y y(2.0f);
    // x+y; // should be a compile-time error
    CHECK_EQUAL(false, is_addable<coord_x,coord_y>::value);
  }

  TEST(add) {
    coord_x x1(5.3f);
    coord_x x2(4.7f);
    // Test in terms of float
    CHECK_CLOSE(10.0f, (x1+x2).value, 0.01f);
    // Test in terms of coord_x
    CHECK_CLOSE(coord_x(10.0f), x1+x2, coord_x(0.01f));
  }

}


//
// Realistic usage test
//
// Define template "address" and "offset" types with the following behavior:
//   A = A + A  error
//   A = A + O  legal
//   A = O + A  legal
//   A = O + O  error
//   O = A + A  error
//   O = A + O  error
//   O = O + A  error
//   O = O + O  legal
//
//   A = A - A  error
//   A = A - O  legal
//   A = O - A  error
//   A = O - O  error
//   O = A - A  legal
//   O = A - O  error
//   O = O - A  error
//   O = O - O  legal
//

template <typename T> struct offset : numeric_typedef<T, offset<T>> {
  using base = numeric_typedef<T, offset<T>>;
  using base::base;
};

template <typename T> struct address : numeric_typedef_base<T, address<T>>
    // A = A + O and A = A - O become supported by operator+= and -= below
  , binop::addable     <address<T>, true , address<T>,  offset<T>>
  , binop::subtractable<address<T>, false, address<T>,  offset<T>>
    // A = O + A is supported by operator+= because + is commutative
  , binop::addable     <address<T>, true ,  offset<T>, address<T>>
    // O = A - A requires conversion to T because we delete -= below
  , binop::subtractable<offset<T> , false, address<T>, address<T>, T, T>
{
  using base = numeric_typedef_base<T, address<T>>;
  using base::base;

  // Remove unwanted operations
  address& operator+=(const address&) = delete;
  address& operator-=(const address&) = delete;

  // Add desired operations
  address& operator+=(const offset<T>& o) noexcept(noexcept(
    std::declval<T&>() += o.value )) {
           this->value += o.value;
    return *this;
  }
  address& operator-=(const offset<T>& o) noexcept(noexcept(
    std::declval<T&>() -= o.value )) {
           this->value -= o.value;
    return *this;
  }

};


SUITE(realistic) {

  TEST(traits) {
    using N = unsigned;
    using A = address<N>;
    using O =  offset<N>;
    CHECK_EQUAL(false, is_addable<A,A>::value);
    CHECK_EQUAL(true , is_addable<A,O>::value);
    CHECK_EQUAL(true , is_addable<O,A>::value);
    CHECK_EQUAL(true , is_addable<O,O>::value);
    CHECK_EQUAL(true, std::is_same<A,
        decltype(std::declval<A>()+std::declval<O>())>::value);
    CHECK_EQUAL(true, std::is_same<A,
        decltype(std::declval<O>()+std::declval<A>())>::value);
    CHECK_EQUAL(true, std::is_same<O,
        decltype(std::declval<O>()+std::declval<O>())>::value);

    CHECK_EQUAL(true , is_subtractable<A,A>::value);
    CHECK_EQUAL(true , is_subtractable<A,O>::value);
    CHECK_EQUAL(false, is_subtractable<O,A>::value);
    CHECK_EQUAL(true , is_subtractable<O,O>::value);
    CHECK_EQUAL(true, std::is_same<O,
        decltype(std::declval<A>()-std::declval<A>())>::value);
    CHECK_EQUAL(true, std::is_same<A,
        decltype(std::declval<A>()-std::declval<O>())>::value);
    CHECK_EQUAL(true, std::is_same<O,
        decltype(std::declval<O>()-std::declval<O>())>::value);
  }

  TEST(add) {
    using N = unsigned;
    using A = address<N>;
    using O =  offset<N>;
    A a(0u);
    O o(0u);
  //a = a + a; // error
    a = a + o; // legal
    a = o + a; // legal
  //a = o + o; // error
  //o = a + a; // error
  //o = a + o; // error
  //o = o + a; // error
    o = o + o; // legal
    a += o; // legal
  }

  TEST(sub) {
    using N = unsigned;
    using A = address<N>;
    using O =  offset<N>;
    A a(0u);
    O o(0u);
  //a = a - a; // error
    a = a - o; // legal
  //a = o - a; // error
  //a = o - o; // error
    o = a - a; // legal
  //o = a - o; // error
  //o = o - a; // error
    o = o - o; // legal
    a -= o; // legal
  }

  TEST(commutative_flavors) {
    using N = unsigned;
    using A = address<N>;
    using O =  offset<N>;
    A a(0u);
    O o(0u);
    a =   a   +   o  ;
    a =   a   + O(0u);
    a = A(0u) +   o  ;
    a = A(0u) + O(0u);
    a =   o   +   a  ;
    a =   o   + A(0u);
    a = O(0u) +   a  ;
    a = O(0u) + A(0u);
    a =   a   -   o  ;
    a =   a   - O(0u);
    a = A(0u) -   o  ;
    a = A(0u) - O(0u);
  }

  TEST(noexcept) {
    using N = unsigned;
    using A = address<N>;
    using O =  offset<N>;
    A a(0u);
    O o(0u);
    bool create_destroy = noexcept( A(1u) );
    CHECK_EQUAL(true, create_destroy);
    bool operate = noexcept( a = o + a );
    CHECK_EQUAL(true, operate);
    bool modify = noexcept( a += o );
    CHECK_EQUAL(true, modify);
  }
}

//
// Concrete example
//

struct time_duration : numeric_typedef<int, time_duration> {
  using base_t = numeric_typedef<int, time_duration>;
  using base_t::base_t;
};

struct time_point : numeric_typedef_base<int, time_point>
  , binop::addable     <time_point, true , time_point, time_duration>
  , binop::addable     <time_point, true , time_duration, time_point>
  , binop::subtractable<time_point, false, time_point, time_duration>
  , binop::subtractable<time_duration, false, time_point, time_point, int, int>
{
  using base_t = numeric_typedef_base<int, time_point>;
  using base_t::base_t;
  using self_t = time_point;

  // Remove unwanted operations
  self_t& operator*=(const self_t&) = delete;
  self_t& operator/=(const self_t&) = delete;
  self_t& operator%=(const self_t&) = delete;
  self_t& operator+=(const self_t&) = delete;
  self_t& operator-=(const self_t&) = delete;
  self_t& operator<<=(const self_t&) = delete;
  self_t& operator>>=(const self_t&) = delete;
  self_t& operator&=(const self_t&) = delete;
  self_t& operator^=(const self_t&) = delete;
  self_t& operator|=(const self_t&) = delete;
  self_t& operator++() = delete;
  self_t& operator--() = delete;
  self_t  operator++(int) = delete;
  self_t  operator--(int) = delete;
  self_t  operator+() = delete;
  self_t  operator-() = delete;
  self_t  operator~() = delete;

  // Add desired operations
  self_t& operator+=(const time_duration& o) {
    this->value += o.value;
    return *this;
  }
  self_t& operator-=(const time_duration& o) {
    this->value -= o.value;
    return *this;
  }
};

//
// Explicit instantiations to verify the entire interface can be compiled
//
namespace opaque {
  template struct numeric_typedef<int, time_duration>;
  template struct numeric_typedef_base<int, time_point>;
}
