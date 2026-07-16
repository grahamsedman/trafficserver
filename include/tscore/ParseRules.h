/** @file
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>
#include <cstring>

#include "tscore/ink_platform.h"
#include "tscore/ink_defs.h"
#include "tscore/ink_apidefs.h"

using CTypeResult = unsigned int;

// Set this to 0 to disable SI
// decimal multipliers
#define USE_SI_MULTIPLIERS 1

#define is_char_BIT            (1 << 0)
#define is_upalpha_BIT         (1 << 1)
#define is_loalpha_BIT         (1 << 2)
#define is_alpha_BIT           (1 << 3)
#define is_digit_BIT           (1 << 4)
#define is_ctl_BIT             (1 << 5)
#define is_ws_BIT              (1 << 6)
#define is_hex_BIT             (1 << 7)
#define is_pchar_BIT           (1 << 8)
#define is_extra_BIT           (1 << 9)
#define is_safe_BIT            (1 << 10)
#define is_unsafe_BIT          (1 << 11)
#define is_national_BIT        (1 << 12)
#define is_reserved_BIT        (1 << 13)
#define is_unreserved_BIT      (1 << 14)
#define is_punct_BIT           (1 << 15)
#define is_end_of_url_BIT      (1 << 16)
#define is_tspecials_BIT       (1 << 17)
#define is_spcr_BIT            (1 << 18)
#define is_splf_BIT            (1 << 19)
#define is_wslfcr_BIT          (1 << 20)
#define is_eow_BIT             (1 << 21)
#define is_token_BIT           (1 << 22)
#define is_uri_BIT             (1 << 23)
#define is_sep_BIT             (1 << 24)
#define is_empty_BIT           (1 << 25)
#define is_alnum_BIT           (1 << 26)
#define is_space_BIT           (1 << 27)
#define is_control_BIT         (1 << 28)
#define is_mime_sep_BIT        (1 << 29)
#define is_http_field_name_BIT (1 << 30)
/* shut up the DEC compiler */
#define is_http_field_value_BIT (((CTypeResult)1) << 31)

extern const CTypeResult parseRulesCType[];
extern const char        parseRulesCTypeToUpper[];
extern const char        parseRulesCTypeToLower[];

class ParseRules
{
public:
  ParseRules();

  ////////////////////////////
  // whitespace definitions //
  ////////////////////////////

  enum {
    CHAR_SP = 32, /* space           */
    CHAR_HT = 9,  /* horizontal tab  */
    CHAR_LF = 10, /* line feed       */
    CHAR_VT = 11, /* vertical tab    */
    CHAR_NP = 12, /* new page        */
    CHAR_CR = 13  /* carriage return */
  };

  /////////////////////
  // character tests //
  /////////////////////

  static CTypeResult is_type(char c, uint32_t bit);

  static CTypeResult is_char(char c);             // ASCII 0-127
  static CTypeResult is_upalpha(char c);          // A-Z
  static CTypeResult is_loalpha(char c);          // a-z
  static CTypeResult is_alpha(char c);            // A-Z,a-z
  static CTypeResult is_digit(char c);            // 0-9
  static CTypeResult is_ctl(char c);              // ASCII 0-31,127 (includes ws)
  static CTypeResult is_hex(char c);              // 0-9,A-F,a-f
  static CTypeResult is_ws(char c);               // SP,HT
  static CTypeResult is_cr(char c);               // CR
  static CTypeResult is_lf(char c);               // LF
  static CTypeResult is_spcr(char c);             // SP,CR
  static CTypeResult is_splf(char c);             // SP,LF
  static CTypeResult is_wslfcr(char c);           // SP,HT,LF,CR
  static CTypeResult is_tspecials(char c);        // HTTP chars that need quoting
  static CTypeResult is_token(char c);            // token (not CTL or specials)
  static CTypeResult is_extra(char c);            // !,*,QUOT,(,),COMMA
  static CTypeResult is_safe(char c);             // [$-_.+]
  static CTypeResult is_unsafe(char c);           // SP,DBLQUOT,#,%,<,>
  static CTypeResult is_national(char c);         // {,},|,BACKSLASH,^,~,[,],`
  static CTypeResult is_reserved(char c);         // :,/,?,:,@,&,=
  static CTypeResult is_unreserved(char c);       // alpha,digit,safe,extra,nat.
  static CTypeResult is_punct(char c);            // !"#$%&'()*+,-./:;<>=?@_{}|~
  static CTypeResult is_end_of_url(char c);       // NUL,CR,SP
  static CTypeResult is_eow(char c);              // NUL,CR,LF
  static CTypeResult is_uri(char c);              // A-Z,a-z,0-9 :/?#[]@!$&'()*+,;=-._~%
  static CTypeResult is_sep(char c);              // nullptr,COMMA,':','!',wslfcr
  static CTypeResult is_empty(char c);            // wslfcr,#
  static CTypeResult is_alnum(char c);            // 0-9,A-Z,a-z
  static CTypeResult is_space(char c);            // ' ' HT,VT,NP,CR,LF
  static CTypeResult is_control(char c);          // 0x00-0x08, 0x0a-0x1f, 0x7f
  static CTypeResult is_mime_sep(char c);         // @()<>,;\"/[]?{} \t
  static CTypeResult is_http_field_name(char c);  // not :, =, 0x80+, control, or mime_sep except for @
  static CTypeResult is_http_field_value(char c); // not CR, LF, comma, or "

  //////////////////
  // string tests //
  //////////////////

  static CTypeResult is_escape(const char *seq); // %<hex><hex>
  static CTypeResult is_uchar(const char *seq);  // starts unreserved or is escape
  static CTypeResult is_pchar(const char *seq);  // uchar,:,@,&,=,+ (see code)

  ///////////////////
  // unimplemented //
  ///////////////////

  // static CTypeResult   is_comment(const char * str);
  // static CTypeResult   is_ctext(const char * str);

  ////////////////
  // operations //
  ////////////////

  static CTypeResult strncasecmp_eow(const char *s1, const char *s2, int n);
  static const char *strcasestr(const char *s1, const char *s2);
  static int         strlen_eow(const char *s);
  static const char *strstr_eow(const char *s1, const char *s2);

  static char        ink_toupper(char c);
  static char        ink_tolower(char c);
  static const char *memchr(const char *s, char c, int max_length);
  static const char *strchr(const char *s, char c);

  // noncopyable
  ParseRules(const ParseRules &)            = delete;
  ParseRules &operator=(const ParseRules &) = delete;
};

// Helper functions - declarations only
std::int64_t  ink_atoi64(const char *, const char **end = nullptr);
std::uint64_t ink_atoui64(const char *);
std::int64_t  ink_atoi64(const char *, int);
int           ink_get_hex(char c);

// Static inline helper functions that are performance-critical and used across the codebase
// These remain inline as they are called in hot paths
static inline int
ink_atoi(const char *str)
{
  std::int64_t val = ink_atoi64(str);

  if (val > INT_MAX) {
    return INT_MAX;
  } else if (val < INT_MIN) {
    return INT_MIN;
  } else {
    return static_cast<int>(val);
  }
}

static inline int
ink_atoi(const char *str, int len)
{
  std::int64_t val = ink_atoi64(str, len);

  if (val > INT_MAX) {
    return INT_MAX;
  } else if (val < INT_MIN) {
    return INT_MIN;
  } else {
    return static_cast<int>(val);
  }
}

static inline unsigned int
ink_atoui(const char *str)
{
  std::uint64_t val = ink_atoui64(str);

  if (val > UINT_MAX) {
    return UINT_MAX;
  } else {
    return static_cast<unsigned int>(val);
  }
}
