#ifndef OPAQUE_EXPERIMENTAL_STRING_TYPEDEF_HPP
#define OPAQUE_EXPERIMENTAL_STRING_TYPEDEF_HPP
//
// Copyright (c) 2016
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
#include "../data.hpp"
#include <memory>
#include <string>

namespace opaque {
namespace experimental {

/// \addtogroup typedefs
/// @{

///
/// String opaque typedef base type
///
/// This is an opaque typedef base class for standard strings.
///
/// Template arguments:
///  -# S : The string type (e.g. std::string)
///  -# R : The result type, your subclass
///
template <typename S, typename R>
struct string_typedef : data<S,R> {
private:
  using base = opaque::data<S,R>;
public:
  using typename base::underlying_type;
  using typename base::opaque_type;
  using base::value;

  typedef typename S::traits_type            traits_type;
  typedef typename S::value_type             value_type;
  typedef typename S::allocator_type         allocator_type;
  typedef typename S::size_type              size_type;
  typedef typename S::difference_type        difference_type;
  typedef typename S::reference              reference;
  typedef typename S::const_reference        const_reference;
  typedef typename S::pointer                pointer;
  typedef typename S::const_pointer          const_pointer;
  typedef typename S::iterator               iterator;
  typedef typename S::const_iterator         const_iterator;
  typedef typename S::reverse_iterator       reverse_iterator;
  typedef typename S::const_reverse_iterator const_reverse_iterator;
  static const size_type npos = size_type(0)-size_type(1);

private:
  typedef          allocator_type Allocator;
  typedef typename std::allocator_traits<Allocator>::value_type charT;
public:

  //
  // Constructor philosophy
  //
  // 1) Provide for explicit construction from an instance of underlying_type.
  //
  // 2) Constructors that have an argument of underlying_type are provided in
  //    in two flavors: an explicit one taking underlying_type, and one taking
  //    opaque_type that has the explicitness of the original.
  //
  // 3) Constructors that do not have an argument of underlying_type are made
  //    explicit.
  //

  explicit string_typedef(const underlying_type&  str) noexcept
    : base(str) { }
  explicit string_typedef(      underlying_type&& str) noexcept
    : base(std::move(str)) { }

  //
  // The default constructor has the C++14 interface; a zero-argument
  // form (defaulted below) and a single-argument form taking an Allocator.
  //
  explicit string_typedef(const Allocator& a)
    : base(a) { }
  explicit string_typedef(const underlying_type& str, size_type pos,
      size_type n = npos, const Allocator& a = Allocator())
    : base(str.value, pos, n, a) { }
  string_typedef(const opaque_type& str, size_type pos,
      size_type n = npos, const Allocator& a = Allocator())
    : base(str.value, pos, n, a) { }
  explicit string_typedef(const charT* s, size_type n,
      const Allocator& a = Allocator())
    : base(s, n, a) { }
  explicit string_typedef(const charT* s, const Allocator& a = Allocator())
    : base(s, a) { }
  explicit string_typedef(size_type n, charT c,
      const Allocator& a = Allocator())
    : base(n, c, a) { }
  template <class InputIterator>
  explicit string_typedef(InputIterator Begin, InputIterator End,
      const Allocator& a = Allocator())
    : base(Begin, End, a) { }
  explicit string_typedef(std::initializer_list<charT> il,
      const Allocator& a = Allocator())
    : base(il, a) { }
  explicit string_typedef(const underlying_type& str, const Allocator& a)
    : base(str.value, a) { }
  explicit string_typedef(underlying_type&& str, const Allocator& a)
    : base(std::move(str.value), a) { }
  string_typedef(const opaque_type& str, const Allocator& a)
    : base(str.value, a) { }
  string_typedef(opaque_type&& str, const Allocator& a)
    : base(std::move(str.value), a) { }

  opaque_type& operator=(const charT* s) {
    value = s;
    return downcast();
  }
  opaque_type& operator=(charT c) {
    value = c;
    return downcast();
  }
  opaque_type& operator=(std::initializer_list<charT> il) {
    value = il;
    return downcast();
  }

  iterator                 begin()       noexcept { return value.begin()  ; }
  const_iterator           begin() const noexcept { return value.begin()  ; }
  iterator                   end()       noexcept { return value.end()    ; }
  const_iterator             end() const noexcept { return value.end()    ; }
  reverse_iterator        rbegin()       noexcept { return value.rbegin() ; }
  const_reverse_iterator  rbegin() const noexcept { return value.rbegin() ; }
  reverse_iterator          rend()       noexcept { return value.rend()   ; }
  const_reverse_iterator    rend() const noexcept { return value.rend()   ; }
  const_iterator          cbegin() const noexcept { return value.cbegin() ; }
  const_iterator            cend() const noexcept { return value.cend()   ; }
  const_reverse_iterator crbegin() const noexcept { return value.crbegin(); }
  const_reverse_iterator   crend() const noexcept { return value.crend()  ; }

  size_type     size() const noexcept { return value.size(); }
  size_type   length() const noexcept { return value.length(); }
  size_type max_size() const noexcept { return value.max_size(); }
  void resize(size_type n, charT c)   { return value.resize(n, c); }
  void resize(size_type n)            { return value.resize(n); }
  size_type capacity() const noexcept { return value.capacity(); }
  void reserve(size_type res_arg = 0) { return value.reserve(res_arg); }
  void shrink_to_fit()                { return value.shrink_to_fit(); }
  void clear()               noexcept { return value.clear(); }
  bool empty()         const noexcept { return value.empty(); }

  const_reference operator[](size_type pos) const { return value[pos]; }
        reference operator[](size_type pos)       { return value[pos]; }
  const_reference at(size_type n)           const { return value.at(n); }
        reference at(size_type n)                 { return value.at(n); }

  const charT& front() const { return value.front(); }
        charT& front()       { return value.front(); }
  const charT& back()  const { return value.back(); }
        charT& back()        { return value.back(); }

  opaque_type& operator+=(const opaque_type& str) {
    value += str.value;
    return downcast();
  }
  opaque_type& operator+=(const charT* s) {
    value += s;
    return downcast();
  }
  opaque_type& operator+=(charT c) {
    value += c;
    return downcast();
  }
  opaque_type& operator+=(std::initializer_list<charT> il) {
    value += il;
    return downcast();
  }
  opaque_type& append(const opaque_type& str) {
    value.append(str.value);
    return downcast();
  }
  // C++14 interface
  opaque_type& append(const opaque_type& str, size_type pos,
      size_type n = npos) {
    value.append(str.value, pos, n);
    return downcast();
  }
  opaque_type& append(const charT* s, size_type n) {
    value.append(s, n);
    return downcast();
  }
  opaque_type& append(const charT* s) {
    value.append(s);
    return downcast();
  }
  opaque_type& append(size_type n, charT c) {
    value.append(n, c);
    return downcast();
  }
  template <class InputIterator>
  opaque_type& append(InputIterator first, InputIterator last) {
    value.append(first, last);
    return downcast();
  }
  opaque_type& append(std::initializer_list<charT> il) {
    value.append(il);
    return downcast();
  }
  void push_back(charT c) {
    return value.push_back(c);
  }

  opaque_type& assign(const opaque_type& str) {
    value.assign(str.value);
    return downcast();
  }
  opaque_type& assign(opaque_type&& str) noexcept {
    value.assign(std::move(str.value));
    return downcast();
  }
  // C++14 interface
  opaque_type& assign(const opaque_type& str, size_type pos,
      size_type n = npos) {
    value.assign(str.value, pos, n);
    return downcast();
  }
  opaque_type& assign(const charT* s, size_type n) {
    value.assign(s, n);
    return downcast();
  }
  opaque_type& assign(const charT* s) {
    value.assign(s);
    return downcast();
  }
  opaque_type& assign(size_type n, charT c) {
    value.assign(n, c);
    return downcast();
  }
  template <class InputIterator>
  opaque_type& assign(InputIterator first, InputIterator last) {
    value.assign(first, last);
    return downcast();
  }
  opaque_type& assign(std::initializer_list<charT> il) {
    value.assign(il);
    return downcast();
  }

  opaque_type& insert(size_type pos1, const opaque_type& str) {
    value.insert(pos1, str.value);
    return downcast();
  }
  // C++14 interface
  opaque_type& insert(size_type pos1, const opaque_type& str,
      size_type pos2, size_type n = npos) {
    value.insert(pos1, str.value, pos2, n);
    return downcast();
  }
  opaque_type& insert(size_type pos, const charT* s, size_type n) {
    value.insert(pos, s, n);
    return downcast();
  }
  opaque_type& insert(size_type pos, const charT* s) {
    value.insert(pos, s);
    return downcast();
  }
  opaque_type& insert(size_type pos, size_type n, charT c) {
    value.insert(pos, n, c);
    return downcast();
  }
  // C++03 interface
  iterator insert(iterator p, charT c) {
    return insert(p, c);
  }
  // C++03 interface
  iterator insert(iterator p, size_type n, charT c) {
    return insert(p, n, c);
  }
  // C++03 interface
  template <class InputIterator>
  iterator insert(iterator p, InputIterator first, InputIterator last) {
    return insert(p, first, last);
  }
  iterator insert(const_iterator p, charT c) {
    return insert(p, c);
  }
  iterator insert(const_iterator p, size_type n, charT c) {
    return insert(p, n, c);
  }
  template <class InputIterator>
  iterator insert(const_iterator p, InputIterator first, InputIterator last) {
    return insert(p, first, last);
  }
  iterator insert(const_iterator p, std::initializer_list<charT> il) {
    return insert(p, il);
  }

  opaque_type& erase(size_type pos = 0, size_type n = npos) {
    value.erase(pos, n);
    return downcast();
  }
  // C++03 interface
  iterator erase(iterator p) {
    return value.erase(p);
  }
  // C++03 interface
  iterator erase(iterator first, iterator last) {
    return value.erase(first, last);
  }
  iterator erase(const_iterator p) {
    return value.erase(p);
  }
  iterator erase(const_iterator first, const_iterator last) {
    return value.erase(first, last);
  }

  void pop_back() { return value.pop_back(); }

  opaque_type& replace(size_type pos1, size_type n1, const opaque_type& str) {
    value.replace(pos1, n1, str.value);
    return downcast();
  }
  // C++14 interface
  opaque_type& replace(size_type pos1, size_type n1, const opaque_type& str,
      size_type pos2, size_type n2 = npos) {
    value.replace(pos1, n1, str.value, pos2, n2);
    return downcast();
  }
  opaque_type& replace(size_type pos, size_type n1, const charT* s,
      size_type n2) {
    value.replace(pos, n1, s, n2);
    return downcast();
  }
  opaque_type& replace(size_type pos, size_type n1, const charT* s) {
    value.replace(pos, n1, s);
    return downcast();
  }
  opaque_type& replace(size_type pos, size_type n1, size_type n2, charT c) {
    value.replace(pos, n1, n2, c);
    return downcast();
  }
  opaque_type& replace(const_iterator i1, const_iterator i2,
      const opaque_type& str) {
    value.replace(i1, i2, str.value);
    return downcast();
  }
  opaque_type& replace(const_iterator i1, const_iterator i2,
      const charT* s, size_type n) {
    value.replace(i1, i2, s, n);
    return downcast();
  }
  opaque_type& replace(const_iterator i1, const_iterator i2,
      const charT* s) {
    value.replace(i1, i2, s);
    return downcast();
  }
  opaque_type& replace(const_iterator i1, const_iterator i2,
      size_type n, charT c) {
    value.replace(i1, i2, n, c);
    return downcast();
  }
  template <class InputIterator>
  opaque_type& replace(const_iterator i1, const_iterator i2,
      InputIterator j1, InputIterator j2) {
    value.replace(i1, i2, j1, j2);
    return downcast();
  }
  opaque_type& replace(const_iterator i1, const_iterator i2,
      std::initializer_list<charT> il) {
    value.replace(i1, i2, il);
    return downcast();
  }

  size_type copy(charT* s, size_type n, size_type pos = 0) const {
    return value.copy(s, n, pos);
  }
  void swap(opaque_type& str) { return value.swap(str.value); }

  const charT* c_str() const noexcept { return value.c_str(); }
  const charT* data()  const noexcept { return value.data(); }
  allocator_type get_allocator() const noexcept {
    return value.get_allocator();
  }

  size_type find (const opaque_type& str, size_type pos = 0) const noexcept {
    return value.find(str.value, pos);
  }
  size_type find (const charT* s, size_type pos, size_type n) const {
    return value.find(s, pos, n);
  }
  size_type find (const charT* s, size_type pos = 0) const {
    return value.find(s, pos);
  }
  size_type find (charT c, size_type pos = 0) const noexcept {
    return value.find(c, pos);
  }
  size_type rfind(const opaque_type& str, size_type pos = npos) const noexcept {
    return value.rfind(str.value, pos);
  }
  size_type rfind(const charT* s, size_type pos, size_type n) const {
    return value.rfind(s, pos, n);
  }
  size_type rfind(const charT* s, size_type pos = npos) const {
    return value.rfind(s, pos);
  }
  size_type rfind(charT c, size_type pos = npos) const noexcept {
    return value.rfind(c, pos);
  }

  size_type find_first_of(const opaque_type& str, size_type pos = 0) const noexcept {
    return value.find_first_of(str.value, pos);
  }
  size_type find_first_of(const charT* s, size_type pos, size_type n) const {
    return value.find_first_of(s, pos, n);
  }
  size_type find_first_of(const charT* s, size_type pos = 0) const {
    return value.find_first_of(s, pos);
  }
  size_type find_first_of(charT c, size_type pos = 0) const noexcept {
    return value.find_first_of(c, pos);
  }
  size_type find_last_of (const opaque_type& str, size_type pos = npos) const noexcept {
    return value.find_last_of(str.value, pos);
  }
  size_type find_last_of (const charT* s, size_type pos, size_type n) const {
    return value.find_last_of(s, pos, n);
  }
  size_type find_last_of (const charT* s, size_type pos = npos) const {
    return value.find_last_of(s, pos);
  }
  size_type find_last_of (charT c, size_type pos = npos) const noexcept {
    return value.find_last_of(c, pos);
  }

  size_type find_first_not_of(const opaque_type& str, size_type pos = 0) const noexcept {
    return value.find_first_not_of(str.value, pos);
  }
  size_type find_first_not_of(const charT* s, size_type pos, size_type n) const {
    return value.find_first_not_of(s, pos, n);
  }
  size_type find_first_not_of(const charT* s, size_type pos = 0) const {
    return value.find_first_not_of(s, pos);
  }
  size_type find_first_not_of(charT c, size_type pos = 0) const noexcept {
    return value.find_first_not_of(c, pos);
  }
  size_type find_last_not_of (const opaque_type& str, size_type pos = npos) const noexcept {
    return value.find_last_not_of(str.value, pos);
  }
  size_type find_last_not_of (const charT* s, size_type pos, size_type n) const {
    return value.find_last_not_of(s, pos, n);
  }
  size_type find_last_not_of (const charT* s, size_type pos = npos) const {
    return value.find_last_not_of(s, pos);
  }
  size_type find_last_not_of (charT c, size_type pos = npos) const noexcept {
    return value.find_last_not_of(c, pos);
  }

  opaque_type substr(size_type pos = 0, size_type n = npos) const {
    return opaque_type(value.substr(pos, n));
  }
  int compare(const opaque_type& str) const noexcept {
    return value.compare(str.value);
  }
  int compare(size_type pos1, size_type n1, const opaque_type& str) const {
    return value.compare(pos1, n1, str.value);
  }
  // C++14 interface
  int compare(size_type pos1, size_type n1, const opaque_type& str,
      size_type pos2, size_type n2 = npos) const {
    return value.compare(pos1, n1, str.value, pos2, n2);
  }
  int compare(const charT* s) const {
    return value.compare(s);
  }
  int compare(size_type pos1, size_type n1, const charT* s) const {
    return value.compare(pos1, n1, s);
  }
  int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const {
    return value.compare(pos1, n1, s, n2);
  }

  friend opaque_type operator+(const opaque_type&  l, const opaque_type&  r) {
    return opaque_type(          l.value  +           r.value );
  }
  friend opaque_type operator+(      opaque_type&& l, const opaque_type&  r) {
    return opaque_type(std::move(l.value) +           r.value );
  }
  friend opaque_type operator+(const opaque_type&  l,       opaque_type&& r) {
    return opaque_type(          l.value  + std::move(r.value));
  }
  friend opaque_type operator+(      opaque_type&& l,       opaque_type&& r) {
    return opaque_type(std::move(l.value) + std::move(r.value));
  }
  friend opaque_type operator+(const charT* lhs, const opaque_type&  rhs) {
    return lhs +           rhs.value ;
  }
  friend opaque_type operator+(const charT* lhs,       opaque_type&& rhs) {
    return lhs + std::move(rhs.value);
  }
  friend opaque_type operator+(      charT  lhs, const opaque_type&  rhs) {
    return lhs +           rhs.value ;
  }
  friend opaque_type operator+(      charT  lhs,       opaque_type&& rhs) {
    return lhs + std::move(rhs.value);
  }
  friend opaque_type operator+(const opaque_type&  lhs, const charT* rhs) {
    return           lhs.value  + rhs;
  }
  friend opaque_type operator+(      opaque_type&& lhs, const charT* rhs) {
    return std::move(lhs.value) + rhs;
  }
  friend opaque_type operator+(const opaque_type&  lhs,       charT  rhs) {
    return           lhs.value  + rhs;
  }
  friend opaque_type operator+(      opaque_type&& lhs,       charT  rhs) {
    return std::move(lhs.value) + rhs;
  }

  friend bool operator==(const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value == rhs.value;
  }
  friend bool operator==(const       charT* lhs, const opaque_type& rhs) {
    return lhs       == rhs.value;
  }
  friend bool operator==(const opaque_type& lhs, const       charT* rhs) {
    return lhs.value == rhs      ;
  }
  friend bool operator!=(const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value != rhs.value;
  }
  friend bool operator!=(const       charT* lhs, const opaque_type& rhs) {
    return lhs       != rhs.value;
  }
  friend bool operator!=(const opaque_type& lhs, const       charT* rhs) {
    return lhs.value != rhs      ;
  }

  friend bool operator< (const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value <  rhs.value;
  }
  friend bool operator< (const       charT* lhs, const opaque_type& rhs) {
    return lhs       <  rhs.value;
  }
  friend bool operator< (const opaque_type& lhs, const       charT* rhs) {
    return lhs.value <  rhs      ;
  }
  friend bool operator> (const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value >  rhs.value;
  }
  friend bool operator> (const       charT* lhs, const opaque_type& rhs) {
    return lhs       >  rhs.value;
  }
  friend bool operator> (const opaque_type& lhs, const       charT* rhs) {
    return lhs.value >  rhs      ;
  }

  friend bool operator<=(const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value <= rhs.value;
  }
  friend bool operator<=(const       charT* lhs, const opaque_type& rhs) {
    return lhs       <= rhs.value;
  }
  friend bool operator<=(const opaque_type& lhs, const       charT* rhs) {
    return lhs.value <= rhs      ;
  }
  friend bool operator>=(const opaque_type& lhs, const opaque_type& rhs) {
    return lhs.value >= rhs.value;
  }
  friend bool operator>=(const       charT* lhs, const opaque_type& rhs) {
    return lhs       >= rhs.value;
  }
  friend bool operator>=(const opaque_type& lhs, const       charT* rhs) {
    return lhs.value >= rhs      ;
  }

  friend void swap(opaque_type& lhs, opaque_type& rhs) {
    using std::swap;
    return swap(lhs.value, rhs.value);
  }

  string_typedef() = default;
  string_typedef(const string_typedef& ) = default;
  string_typedef(      string_typedef&&) = default;
  string_typedef& operator=(const string_typedef& ) = default;
  string_typedef& operator=(      string_typedef&&) = default;
protected:
  ~string_typedef() = default;
  using base::downcast;
};

/// @}

}
}

#endif
