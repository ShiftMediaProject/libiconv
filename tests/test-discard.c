/* Copyright (C) 2024 Free Software Foundation, Inc.
   This file is part of the GNU LIBICONV Library.

   The GNU LIBICONV Library is free software; you can redistribute it
   and/or modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.

   The GNU LIBICONV Library is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU LIBICONV Library; see the file COPYING.LIB.
   If not, see <https://www.gnu.org/licenses/>.  */

/* Comment out this line, to build against glibc instead of libiconv. */
#include "config.h"

#include <locale.h>
#include <stdlib.h>
#include <iconv.h>
#include <errno.h>

/* This test checks the behaviour of iconv() with suffix //IGNORE,
   and also the equivalent options set through iconvctl(). */

static const char input1[7] = "3\xd4\xe2\x84\x83\xc3\x9f"; /* "3<D4>℃ß" */
static const char input2[7] = "3\xe2\x84\x83\xd4\xc3\x9f"; /* "3℃<D4>ß" */

static void test_default (iconv_t cd)
{
  {
    char output[10];
    char *inbuf = (char *) input1;
    size_t inbytesleft = sizeof (input1);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == (size_t)(-1) && errno == EILSEQ && sizeof (input1) - inbytesleft == 1))
      abort ();
    if (!(sizeof (output) - outbytesleft == 1
          && output[0] == '3'))
      abort ();
  }
  {
    char output[10];
    char *inbuf = (char *) input2;
    size_t inbytesleft = sizeof (input2);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == (size_t)(-1) && errno == EILSEQ && sizeof (input2) - inbytesleft == 1))
      abort ();
    if (!(sizeof (output) - outbytesleft == 1
          && output[0] == '3'))
      abort ();
  }
  #ifdef _LIBICONV_VERSION
  int x;
  if (iconvctl (cd, ICONV_GET_TRANSLITERATE, &x) != 0)
    abort ();
  if (x != 0)
    abort ();
  if (iconvctl (cd, ICONV_GET_DISCARD_ILSEQ, &x) != 0)
    abort ();
  if (x != 0)
    abort ();
  #endif
}

static void test_translit (iconv_t cd)
{
  {
    char output[10];
    char *inbuf = (char *) input1;
    size_t inbytesleft = sizeof (input1);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == (size_t)(-1) && errno == EILSEQ && sizeof (input1) - inbytesleft == 1))
      abort ();
    if (!(sizeof (output) - outbytesleft == 1
          && output[0] == '3'))
      abort ();
  }
  {
    char output[10];
    char *inbuf = (char *) input2;
    size_t inbytesleft = sizeof (input2);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == (size_t)(-1) && errno == EILSEQ && sizeof (input2) - inbytesleft == 4))
      abort ();
    if (!(sizeof (output) - outbytesleft == 3
          && output[0] == '3' && output[1] == '\xb0' && output[2] == 'C'))
      abort ();
  }
  #ifdef _LIBICONV_VERSION
  int x;
  if (iconvctl (cd, ICONV_GET_TRANSLITERATE, &x) != 0)
    abort ();
  if (x != 1)
    abort ();
  if (iconvctl (cd, ICONV_GET_DISCARD_ILSEQ, &x) != 0)
    abort ();
  if (x != 0)
    abort ();
  #endif
}

static void test_ignore (iconv_t cd)
{
  {
    char output[10];
    char *inbuf = (char *) input1;
    size_t inbytesleft = sizeof (input1);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    #ifdef _LIBICONV_VERSION
    if (!(ret == 1 && inbytesleft == 0))
      abort ();
    #else /* glibc */
    if (!(ret == (size_t)(-1) && errno == EILSEQ && inbytesleft == 0))
      abort ();
    #endif
    if (!(sizeof (output) - outbytesleft == 2
          && output[0] == '3' && output[1] == '\xdf'))
      abort ();
  }
  {
    char output[10];
    char *inbuf = (char *) input2;
    size_t inbytesleft = sizeof (input2);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    #ifdef _LIBICONV_VERSION
    if (!(ret == 1 && inbytesleft == 0))
      abort ();
    #else /* glibc */
    if (!(ret == (size_t)(-1) && errno == EILSEQ && inbytesleft == 0))
      abort ();
    #endif
    if (!(sizeof (output) - outbytesleft == 2
          && output[0] == '3' && output[1] == '\xdf'))
      abort ();
  }
  #ifdef _LIBICONV_VERSION
  int x;
  if (iconvctl (cd, ICONV_GET_TRANSLITERATE, &x) != 0)
    abort ();
  if (x != 0)
    abort ();
  if (iconvctl (cd, ICONV_GET_DISCARD_ILSEQ, &x) != 0)
    abort ();
  if (x != 1)
    abort ();
  #endif
}

static void test_ignore_translit (iconv_t cd)
{
  {
    char output[10];
    char *inbuf = (char *) input1;
    size_t inbytesleft = sizeof (input1);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == 1 && inbytesleft == 0))
      abort ();
    if (!(sizeof (output) - outbytesleft == 4
          && output[0] == '3' && output[1] == '\xb0' && output[2] == 'C' && output[3] == '\xdf'))
      abort ();
  }
  {
    char output[10];
    char *inbuf = (char *) input2;
    size_t inbytesleft = sizeof (input2);
    char *outbuf = output;
    size_t outbytesleft = sizeof (output);
    size_t ret = iconv (cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
    if (!(ret == 1 && inbytesleft == 0))
      abort ();
    if (!(sizeof (output) - outbytesleft == 4
          && output[0] == '3' && output[1] == '\xb0' && output[2] == 'C' && output[3] == '\xdf'))
      abort ();
  }
  #ifdef _LIBICONV_VERSION
  int x;
  if (iconvctl (cd, ICONV_GET_TRANSLITERATE, &x) != 0)
    abort ();
  if (x != 1)
    abort ();
  if (iconvctl (cd, ICONV_GET_DISCARD_ILSEQ, &x) != 0)
    abort ();
  if (x != 1)
    abort ();
  #endif
}

int main ()
{
  #ifndef _LIBICONV_VERSION
  /* For glibc: Enable locale-dependent transliterations. */
  setlocale (LC_ALL, "en_US.UTF-8");
  #endif

  {
    iconv_t cd = iconv_open ("ISO-8859-1", "UTF-8");
    test_default (cd);
    iconv_close (cd);
  }

  {
    iconv_t cd = iconv_open ("ISO-8859-1//TRANSLIT", "UTF-8");
    test_translit (cd);
    iconv_close (cd);
  }
  #ifdef _LIBICONV_VERSION
  {
    iconv_t cd = iconv_open ("ISO-8859-1", "UTF-8");
    { int x = 1; iconvctl (cd, ICONV_SET_TRANSLITERATE, &x); }
    test_translit (cd);
    iconv_close (cd);
  }
  #endif

  {
    iconv_t cd = iconv_open ("ISO-8859-1//IGNORE", "UTF-8");
    test_ignore (cd);
    iconv_close (cd);
  }
  #ifdef _LIBICONV_VERSION
  {
    iconv_t cd = iconv_open ("ISO-8859-1", "UTF-8");
    { int x = 1; iconvctl (cd, ICONV_SET_DISCARD_ILSEQ, &x); }
    test_ignore (cd);
    iconv_close (cd);
  }
  #endif

  {
    iconv_t cd = iconv_open ("ISO-8859-1//IGNORE//TRANSLIT", "UTF-8");
    test_ignore_translit (cd);
    iconv_close (cd);
  }
  {
    iconv_t cd = iconv_open ("ISO-8859-1//TRANSLIT//IGNORE", "UTF-8");
    test_ignore_translit (cd);
    iconv_close (cd);
  }
  #ifdef _LIBICONV_VERSION
  {
    iconv_t cd = iconv_open ("ISO-8859-1", "UTF-8");
    { int x = 1; iconvctl (cd, ICONV_SET_TRANSLITERATE, &x); }
    { int x = 1; iconvctl (cd, ICONV_SET_DISCARD_ILSEQ, &x); }
    test_ignore_translit (cd);
    iconv_close (cd);
  }
  #endif

  return 0;
}
