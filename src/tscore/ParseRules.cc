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

/*************************** -*- Mod: C++ -*- ******************************

   ParseRules.cc -- Implementation of ParseRules class methods


 ****************************************************************************/

#include <cstdint>
#include "tscore/ink_platform.h"
#include "tscore/ParseRules.h"

const unsigned int parseRulesCType[256] = {
#include "ParseRulesCType"
};
const char parseRulesCTypeToUpper[256] = {
#include "ParseRulesCTypeToUpper"
};
const char parseRulesCTypeToLower[256] = {
#include "ParseRulesCTypeToLower"
};

// Default constructor
ParseRules::ParseRules() noexcept = default;

// Helper function used by ink_atoi64 and ink_atoui64
int
ink_get_hex(char c)
{
  if (ParseRules::is_digit(c)) {
    return (c - '0');
  }
  c = ParseRules::ink_tolower(c);
  return ((c - 'a') + 10);
}

///////////////////////////////
// character tests implementations //
///////////////////////////////

CTypeResult
ParseRules::is_type(char c, uint32_t bitmask)
{
  return (parseRulesCType[static_cast<unsigned char>(c)] & bitmask);
}

CTypeResult
ParseRules::is_char(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_char_BIT);
#else
  return ((c & 0x80) == 0);
#endif
}

CTypeResult
ParseRules::is_upalpha(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_upalpha_BIT);
#else
  return (c >= 'A' && c <= 'Z');
#endif
}

CTypeResult
ParseRules::is_loalpha(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_loalpha_BIT);
#else
  return (c >= 'a' && c <= 'z');
#endif
}

CTypeResult
ParseRules::is_alpha(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_alpha_BIT);
#else
  return (is_upalpha(c) || is_loalpha(c));
#endif
}

CTypeResult
ParseRules::is_digit(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_digit_BIT);
#else
  return (c >= '0' && c <= '9');
#endif
}

CTypeResult
ParseRules::is_alnum(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_alnum_BIT);
#else
  return (is_alpha(c) || is_digit(c));
#endif
}

CTypeResult
ParseRules::is_ctl(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_ctl_BIT);
#else
  return ((!(c & 0x80) && c <= 31) || c == 127);
#endif
}

CTypeResult
ParseRules::is_ws(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_ws_BIT);
#else
  return (c == CHAR_SP || c == CHAR_HT);
#endif
}

CTypeResult
ParseRules::is_hex(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_hex_BIT);
#else
  return ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'));
#endif
}

CTypeResult
ParseRules::is_cr(char c)
{
  return (c == CHAR_CR);
}

CTypeResult
ParseRules::is_lf(char c)
{
  return (c == CHAR_LF);
}

CTypeResult
ParseRules::is_splf(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_splf_BIT);
#else
  return (c == CHAR_SP || c == CHAR_LF);
#endif
}

CTypeResult
ParseRules::is_spcr(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_spcr_BIT);
#else
  return (c == CHAR_SP || c == CHAR_CR);
#endif
}

CTypeResult
ParseRules::is_wslfcr(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_wslfcr_BIT);
#else
  return ParseRules::is_ws(c) || ParseRules::is_splf(c) || ParseRules::is_spcr(c);
#endif
}

CTypeResult
ParseRules::is_extra(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_extra_BIT);
#else
  switch (c) {
  case '!':
  case '*':
  case '\'':
  case '(':
  case ')':
  case ',':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_safe(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_safe_BIT);
#else
  return (c == '$' || c == '-' || c == '_' || c == '.' || c == '+');
#endif
}

CTypeResult
ParseRules::is_unsafe(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_unsafe_BIT);
#else
  if (is_ctl(c)) {
    return (true);
  }

  switch (c) {
  case ' ':
  case '\"':
  case '#':
  case '%':
  case '<':
  case '>':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_reserved(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_reserved_BIT);
#else
  switch (c) {
  case ';':
  case '/':
  case '?':
  case ':':
  case '@':
  case '&':
  case '=':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_national(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_national_BIT);
#else
  switch (c) {
  case '{':
  case '}':
  case '|':
  case '\\':
  case '^':
  case '~':
  case '[':
  case ']':
  case '`':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_unreserved(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_unreserved_BIT);
#else
  return (is_alpha(c) || is_digit(c) || is_safe(c) || is_extra(c) || is_national(c));
#endif
}

CTypeResult
ParseRules::is_punct(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_punct_BIT);
#else
  switch (c) {
  case '!':
  case '"':
  case '#':
  case '%':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case '-':
  case '.':
  case '/':
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '@':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '_':
  case '`':
  case '{':
  case '|':
  case '}':
  case '~':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_end_of_url(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_end_of_url_BIT);
#else
  return (c == '\0' || c == '\n' || c == ' ' || ParseRules::is_ctl(c));
#endif
}

CTypeResult
ParseRules::is_escape(const char *seq)
{
  return (seq[0] == '%' && is_hex(seq[1]) && is_hex(seq[2]));
}

CTypeResult
ParseRules::is_uchar(const char *seq)
{
  return (is_unreserved(seq[0]) || is_escape(seq));
}

//
// have to cheat on this one
//
CTypeResult
ParseRules::is_pchar(const char *seq)
{
#ifndef COMPILE_PARSE_RULES
  if (*seq != '%') {
    return (parseRulesCType[static_cast<uint8_t>(*seq)] & is_pchar_BIT);
  } else {
    return is_hex(seq[1]) && is_hex(seq[2]);
  }
#else
  if (is_unreserved(*seq)) {
    return (true);
  }

  switch (seq[0]) {
  case ':':
  case '@':
  case '&':
  case '=':
  case '+':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_tspecials(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_tspecials_BIT);
#else
  switch (c) {
  case '(':
  case ')':
  case '<':
  case '>':
  case '@':
  case ',':
  case ';':
  case ':':
  case '\\':
  case '"':
  case '/':
  case '[':
  case ']':
  case '?':
  case '=':
  case '{':
  case '}':
  case CHAR_SP:
  case CHAR_HT:
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_token(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_token_BIT);
#else
  return (is_char(c) && !(is_ctl(c) || is_tspecials(c)));
#endif
}

char
ParseRules::ink_toupper(char c)
{
#ifndef COMPILE_PARSE_RULES
  return parseRulesCTypeToUpper[static_cast<unsigned char>(c)];
#else
  int       up_case      = c;
  const int up_case_diff = 'a' - 'A';

  if (c >= 'a' && c <= 'z') {
    up_case = c - up_case_diff;
  }
  return (up_case);
#endif
}

char
ParseRules::ink_tolower(char c)
{
#ifndef COMPILE_PARSE_RULES
  return parseRulesCTypeToLower[static_cast<unsigned char>(c)];
#else
  int       lo_case      = c;
  const int lo_case_diff = 'a' - 'A';

  if (c >= 'A' && c <= 'Z') {
    lo_case = c + lo_case_diff;
  }
  return (lo_case);
#endif
}

CTypeResult
ParseRules::is_eow(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_eow_BIT);
#else
  return (c == '\0' || c == '\r' || c == '\n');
#endif
}

CTypeResult
ParseRules::is_uri(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_uri_BIT);
#else
  if (is_alnum(c)) {
    return (true);
  }

  switch (c) {
  case ':':
  case '/':
  case '?':
  case '#':
  case '[':
  case ']':
  case '@':
  case '!':
  case '$':
  case '&':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case ',':
  case ';':
  case '=':
  case '-':
  case '.':
  case '_':
  case '~':
  case '%':
    return (true);
  }
  return (false);
#endif
}

CTypeResult
ParseRules::is_sep(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_sep_BIT);
#else
  return (!c || c == ',' || c == ':' || c == '!' || is_wslfcr(c));
#endif
}

CTypeResult
ParseRules::is_empty(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_empty_BIT);
#else
  return (c == '#' || is_wslfcr(c));
#endif
}

CTypeResult
ParseRules::is_space(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_space_BIT);
#else
  switch (c) {
  case CHAR_SP:
  case CHAR_HT:
  case CHAR_LF:
  case CHAR_VT:
  case CHAR_NP:
  case CHAR_CR:
    return (true);
  }
  return (false);
#endif
}

/**
   Return true if @c is a control char except HTAB(0x09) and SP(0x20).
   If you need to check @c is HTAB or SP, use `ParseRules::is_ws`.
 */
CTypeResult
ParseRules::is_control(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_control_BIT);
#else
  if (c == CHAR_HT || c == CHAR_SP) {
    return false;
  }

  if ((static_cast<unsigned char>(c)) < 0x20 || c == 0x7f) {
    return true;
  }

  return false;
#endif
}

CTypeResult
ParseRules::is_mime_sep(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_mime_sep_BIT);
#else
  if ((c == '(') || (c == ')') || (c == '<') || (c == '>') || (c == '@') || (c == ',') || (c == ';') || (c == '\\') ||
      (c == '\"') || (c == '/') || (c == '[') || (c == ']') || (c == '?') || (c == '{') || (c == '}') || (c == ' ') ||
      (c == '\t')) {
    return true;
  }
  return false;
#endif
}

CTypeResult
ParseRules::is_http_field_name(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_http_field_name_BIT);
#else
  if (!is_char(c) || is_control(c) || (is_mime_sep(c) && c != '@') || c == '=' || c == ':') {
    return false;
  }
  return true;
#endif
}

CTypeResult
ParseRules::is_http_field_value(char c)
{
#ifndef COMPILE_PARSE_RULES
  return (parseRulesCType[static_cast<unsigned char>(c)] & is_http_field_value_BIT);
#else
  switch (c) {
  case CHAR_CR:
  case CHAR_LF:
  case '\"':
  case ',':
    return false;
  }
  return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
//      strncasecmp_eow(s1, s2, count)
//
//      This wacky little function compares if two strings <s1> and <s2> match
//      (case-insensitively) up to <count> characters long, stopping not only
//      at the end of string ('\0'), but also at end of line (CR or LF).
//
////////////////////////////////////////////////////////////////////////////////

CTypeResult
ParseRules::strncasecmp_eow(const char *s1, const char *s2, int count)
{
  for (int i = 0; i < count; i++) {
    const char &a = s1[i];
    const char &b = s2[i];

    ///////////////////////////////////////////////////////////////
    // if they are different; only match if both are terminators //
    ///////////////////////////////////////////////////////////////
    if (ink_tolower(a) != ink_tolower(b)) {
      return (is_eow(a) && is_eow(b));
    }
  }
  return (true);
}

////////////////////////////////////////////////////////////////////////////////
//
//  strlen_eow()
//
//  return the length of a string
////////////////////////////////////////////////////////////////////////////////
int
ParseRules::strlen_eow(const char *s)
{
  for (int i = 0; true; i++) {
    if (is_eow(s[i])) {
      return (i);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//  strstr_eow()
//
//  This function is the same as strstr(), except that it accepts strings
//  that are terminated with '\r', '\n' or null.
//  It returns a pointer to the first occurrence of s2 within s1 (or null).
////////////////////////////////////////////////////////////////////////////////
const char *
ParseRules::strstr_eow(const char *s1, const char *s2)
{
  int i1;

  int s2_len = strlen_eow(s2);

  for (i1 = 0; !is_eow(s1[i1]); i1++) {
    if (ink_tolower(s1[i1]) == ink_tolower(s2[0])) {
      if (strncasecmp_eow(&s1[i1], &s2[0], s2_len)) {
        return (&s1[i1]);
      }
    }
  }

  return (nullptr);
}

const char *
ParseRules::strcasestr(const char *s1, const char *s2)
{
  int i1;

  size_t s2_len = strlen(s2);

  for (i1 = 0; s1[i1] != '\0'; i1++) {
    if (ink_tolower(s1[i1]) == ink_tolower(s2[0])) {
      if (strncasecmp_eow(&s1[i1], &s2[0], static_cast<int>(s2_len))) {
        return (&s1[i1]);
      }
    }
  }

  return (nullptr);
}

const char *
ParseRules::memchr(const char *s, char c, int max_length)
{
  for (int i = 0; i < max_length; i++) {
    if (s[i] == c) {
      return (&s[i]);
    }
  }
  return (nullptr);
}

const char *
ParseRules::strchr(const char *s, char c)
{
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] == c) {
      return (&s[i]);
    }
  }
  return (nullptr);
}

// Implement our atol() / strtol() utility functions. Note that these will
// deal with two cases atol does not:
//
//   1. They will handle both base 10 and base 16, always. 0x indicates hex.
//   2. They all honor the SI multipliers (i.e. K, M, G and T.
//
std::int64_t
ink_atoi64(const char *str, const char **end)
{
  std::int64_t num      = 0;
  int          negative = 0;

  while (*str && ParseRules::is_wslfcr(*str)) {
    str += 1;
  }

  if (unlikely(str[0] == '0' && str[1] == 'x')) {
    str += 2;
    while (*str && ParseRules::is_hex(*str)) {
      num = (num << 4) + ink_get_hex(*str++);
    }
  } else {
    if (unlikely(*str == '-')) {
      negative  = 1;
      str      += 1;
    }

    /*
      NOTE: we first compute the value as negative then correct the
      sign back to positive. This enables us to correctly parse MININT.
    */
    while (*str && ParseRules::is_digit(*str)) {
      num = (num * 10) - (*str++ - '0');
    }
#if USE_SI_MULTIPLIERS
    if (*str) {
      if (*str == 'K') {
        num = num * (1LL << 10);
        str++;
      } else if (*str == 'M') {
        num = num * (1LL << 20);
        str++;
      } else if (*str == 'G') {
        num = num * (1LL << 30);
        str++;
      } else if (*str == 'T') {
        num = num * (1LL << 40);
        str++;
      }
    }
#endif
    if (!negative) {
      num = -num;
    }
  }

  if (end != nullptr) {
    *end = str;
  }

  return num;
}

std::uint64_t
ink_atoui64(const char *str)
{
  std::uint64_t num = 0;

  while (*str && ParseRules::is_wslfcr(*str)) {
    str += 1;
  }

  if (unlikely(str[0] == '0' && str[1] == 'x')) {
    str += 2;
    while (*str && ParseRules::is_hex(*str)) {
      num = (num << 4) + ink_get_hex(*str++);
    }
  } else {
    while (*str && ParseRules::is_digit(*str)) {
      num = (num * 10) + (*str++ - '0');
    }
#if USE_SI_MULTIPLIERS
    if (*str) {
      if (*str == 'K') {
        num = num * (1LL << 10);
      } else if (*str == 'M') {
        num = num * (1LL << 20);
      } else if (*str == 'G') {
        num = num * (1LL << 30);
      } else if (*str == 'T') {
        num = num * (1LL << 40);
      }
    }
#endif
  }
  return num;
}

std::int64_t
ink_atoi64(const char *str, int len)
{
  std::int64_t num      = 0;
  int          negative = 0;

  while (len > 0 && *str && ParseRules::is_wslfcr(*str)) {
    str += 1;
    len--;
  }

  if (len < 1) {
    return 0;
  }

  if (unlikely(str[0] == '0' && len > 1 && str[1] == 'x')) {
    str += 2;
    while (len > 0 && *str && ParseRules::is_hex(*str)) {
      num = (num << 4) + ink_get_hex(*str++);
      len--;
    }
  } else {
    if (unlikely(*str == '-')) {
      negative  = 1;
      str      += 1;
    }

    /*
      NOTE: we first compute the value as negative then correct the
      sign back to positive. This enables us to correctly parse MININT.
    */
    while (len > 0 && *str && ParseRules::is_digit(*str)) {
      num = (num * 10) - (*str++ - '0');
      len--;
    }
#if USE_SI_MULTIPLIERS
    if (len > 0 && *str) {
      if (*str == 'K') {
        num = num * (1 << 10);
      } else if (*str == 'M') {
        num = num * (1 << 20);
      } else if (*str == 'G') {
        num = num * (1 << 30);
      }
    }
#endif

    if (!negative) {
      num = -num;
    }
  }
  return num;
}
