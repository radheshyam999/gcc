/* Software floating-point emulation.
   Return 1 iff a > b, 0 otherwise.
   Copyright (C) 1997-2025 Free Software Foundation, Inc.
   Contributed by Richard Henderson (rth@cygnus.com) and
		  Jakub Jelinek (jj@ultra.linux.cz).

   This file is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with GCC; see the file COPYING.LIB.  If not see
   <http://www.gnu.org/licenses/>.  */

#include <soft-fp/soft-fp.h>
#include <soft-fp/single.h>

CMPtype __c6xabi_gtf(SFtype a, SFtype b)
{
  FP_DECL_EX;
  FP_DECL_S(A); FP_DECL_S(B);
  CMPtype r;

  FP_UNPACK_RAW_S(A, a);
  FP_UNPACK_RAW_S(B, b);
  FP_CMP_S(r, A, B, -2, 2);
  FP_HANDLE_EXCEPTIONS;

  return r > 0;
}
