// 2001-08-23 Benjamin Kosnik  <bkoz@redhat.com>

// Copyright (C) 2001-2025 Free Software Foundation, Inc.
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

// 22.2.6.3.1 moneypunct members

#include <locale>
#include <string>
#include <testsuite_hooks.h>

void test01()
{
  using namespace std;
  typedef money_base::part part;
  typedef money_base::pattern pattern;

  // basic construction
  locale loc_c = locale::classic();

  // cache the moneypunct facets
  typedef moneypunct<char, true> __money_true;
  typedef moneypunct<char, false> __money_false;
  const __money_true& monp_c_t = use_facet<__money_true>(loc_c); 
  const __money_false& monp_c_f = use_facet<__money_false>(loc_c); 

  // quick sanity check for data.
  char q1 = monp_c_t.decimal_point();
  char q2 = monp_c_t.thousands_sep();
  char q3 = monp_c_f.decimal_point();
  char q4 = monp_c_f.thousands_sep();
  string g1 = monp_c_t.grouping();
  string g2 = monp_c_f.grouping();
  string cs1 = monp_c_t.curr_symbol();
  string cs2 = monp_c_f.curr_symbol();
  string ps1 = monp_c_t.positive_sign();
  string ns1 = monp_c_t.negative_sign();
  string ps2 = monp_c_f.positive_sign();
  string ns2 = monp_c_f.negative_sign();
  int fd1 = monp_c_t.frac_digits();
  int fd2 = monp_c_f.frac_digits();
  pattern pos1 = monp_c_t.pos_format();
  pattern neg1 = monp_c_t.neg_format();
  pattern pos2 = monp_c_f.pos_format();
  pattern neg2 = monp_c_f.neg_format();
  neg1 = neg1;
  neg2 = neg2;

  VERIFY( q1 == '.' );
  VERIFY( q3 == '.' );
  VERIFY( q2 == ',' );
  VERIFY( q4 == ',' );
  VERIFY( g1 == "" );
  VERIFY( g2 == "" );
  VERIFY( cs1 == "" );
  VERIFY( cs2 == "" );
  VERIFY( ps1 == "" );
  VERIFY( ps2 == "" );
  VERIFY( ns1 == "" );
  VERIFY( ns2 == "" );
  VERIFY( fd1 == 0 );
  VERIFY( fd2 == 0 );

  VERIFY(static_cast<part>(pos1.field[0]) == static_cast<part>(pos2.field[0]));
  VERIFY(static_cast<part>(pos1.field[1]) == static_cast<part>(pos2.field[1]));
  VERIFY(static_cast<part>(pos1.field[2]) == static_cast<part>(pos2.field[2]));
  VERIFY(static_cast<part>(pos1.field[3]) == static_cast<part>(pos2.field[3]));

#if 0
  VERIFY( pos1[0] == money_base::_S_default_pattern[0] );
  VERIFY( pos1[1] == money_base::_S_default_pattern[1] );
  VERIFY( pos1[2] == money_base::_S_default_pattern[2] );
  VERIFY( pos1[3] == money_base::_S_default_pattern[3] );
  VERIFY( pos2 == money_base::_S_default_pattern );
  VERIFY( neg1 == money_base::_S_default_pattern );
  VERIFY( neg2 == money_base::_S_default_pattern );
#endif
}

int main()
{
  test01();
  return 0;
}
