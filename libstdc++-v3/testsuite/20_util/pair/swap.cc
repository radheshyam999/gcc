// { dg-do run { target c++11 } }

// Copyright (C) 2007-2025 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.


#include <utility>
#include <testsuite_allocator.h>
#include <testsuite_hooks.h>

void test01()
{
  typedef __gnu_test::uneq_allocator<double> ua_type;
  ua_type one(1), two(2);

  std::pair<ua_type, int> p1(one, 1), p2(two, 2);

  p1.swap(p2);
  VERIFY( p1.first.get_personality() == 2 );
  VERIFY( p1.second == 2 );
  VERIFY( p2.first.get_personality() == 1 );
  VERIFY( p2.second == 1 );

  swap(p1, p2);
  VERIFY( p2.first.get_personality() == 2 );
  VERIFY( p2.second == 2 );
  VERIFY( p1.first.get_personality() == 1 );
  VERIFY( p1.second == 1 );
}

int main()
{
  test01();
  return 0;
}
