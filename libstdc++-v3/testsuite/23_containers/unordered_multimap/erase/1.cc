// { dg-do run { target c++11 } }

// 2010-02-10  Paolo Carlini  <paolo.carlini@oracle.com> 
//
// Copyright (C) 2010-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include <unordered_map>
#include <string>
#include <testsuite_hooks.h>

namespace
{
  std::size_t
  get_nb_bucket_elems(const std::unordered_multimap<std::string, int>& us)
  {
    std::size_t nb = 0;
    for (std::size_t b = 0; b != us.bucket_count(); ++b)
      nb += us.bucket_size(b);
    return nb;
  }
}

void test01()
{
  typedef std::unordered_multimap<std::string, int> Mmap;
  typedef Mmap::iterator       iterator;
  typedef Mmap::const_iterator const_iterator;
  typedef Mmap::value_type     value_type;

  Mmap mm1;

  mm1.insert(value_type("because to why", 1));
  mm1.insert(value_type("the stockholm syndrome", 2));
  mm1.insert(value_type("a cereous night", 3));
  mm1.insert(value_type("eeilo", 4));
  mm1.insert(value_type("protean", 5));
  mm1.insert(value_type("the way you are when", 6));
  mm1.insert(value_type("tillsammans", 7));
  mm1.insert(value_type("umbra/penumbra", 8));
  mm1.insert(value_type("belonging (no longer mix)", 9));
  mm1.insert(value_type("one line behind", 10));
  mm1.insert(value_type("because to why", 11));
  VERIFY( mm1.size() == 11 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );

  VERIFY( mm1.erase("eeilo") == 1 );
  VERIFY( mm1.size() == 10 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  iterator it1 = mm1.find("eeilo");
  VERIFY( it1 == mm1.end() );

  VERIFY( mm1.erase("tillsammans") == 1 );
  VERIFY( mm1.size() == 9 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  iterator it2 = mm1.find("tillsammans");
  VERIFY( it2 == mm1.end() );

  // Must work (see DR 526)
  iterator it3 = mm1.find("belonging (no longer mix)");
  VERIFY( it3 != mm1.end() );
  VERIFY( mm1.erase(it3->first) == 1 );
  VERIFY( mm1.size() == 8 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  it3 = mm1.find("belonging (no longer mix)");
  VERIFY( it3 == mm1.end() );

  VERIFY( !mm1.erase("abra") );
  VERIFY( mm1.size() == 8 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );

  VERIFY( !mm1.erase("eeilo") );
  VERIFY( mm1.size() == 8 );

  VERIFY( mm1.erase("because to why") == 2 );
  VERIFY( mm1.size() == 6 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  iterator it4 = mm1.find("because to why");
  VERIFY( it4 == mm1.end() );

  iterator it5 = mm1.find("umbra/penumbra");
  iterator it6 = mm1.find("one line behind");
  VERIFY( it5 != mm1.end() );
  VERIFY( it6 != mm1.end() );

  VERIFY( mm1.find("the stockholm syndrome") != mm1.end() );
  VERIFY( mm1.find("a cereous night") != mm1.end() );
  VERIFY( mm1.find("the way you are when") != mm1.end() );
  VERIFY( mm1.find("a cereous night") != mm1.end() );

  VERIFY( mm1.erase(it5->first) == 1 );
  VERIFY( mm1.size() == 5 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  it5 = mm1.find("umbra/penumbra");
  VERIFY( it5 == mm1.end() );

  VERIFY( mm1.erase(it6->first) == 1 );
  VERIFY( mm1.size() == 4 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  it6 = mm1.find("one line behind");
  VERIFY( it6 == mm1.end() );

  iterator it7 = mm1.begin();
  iterator it8 = it7;
  ++it8;
  iterator it9 = it8;
  ++it9;

  VERIFY( mm1.erase(it8->first) == 1 );
  VERIFY( mm1.size() == 3 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  VERIFY( ++it7 == it9 );

  iterator it10 = it9;
  ++it10;
  iterator it11 = it10;

  VERIFY( mm1.erase(it9->first) == 1 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  VERIFY( mm1.size() == 2 );
  VERIFY( ++it10 == mm1.end() );

  VERIFY( mm1.erase(mm1.begin()) != mm1.end() );  
  VERIFY( mm1.size() == 1 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  VERIFY( mm1.begin() == it11 );

  VERIFY( mm1.erase(mm1.begin()->first) == 1 );  
  VERIFY( mm1.size() == 0 );
  VERIFY( get_nb_bucket_elems(mm1) == mm1.size() );
  VERIFY( distance(mm1.begin(), mm1.end()) - mm1.size() == 0 );
  VERIFY( mm1.begin() == mm1.end() );
}

int main()
{
  test01();
  return 0;
}
