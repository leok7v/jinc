#pragma once
#ifndef STR_H
#define STR_H
/*  Copyright (c) 2003-2013, Leo Kuznetsov
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of the {organization} nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* immutable hashcode and length preserving strings */

typedef const char* str_t;

#define const_str(name, const_char_string) \
    static char _##name##_static[sizeof(struct _str_s) + sizeof(const_char_string)] = \
    "STRS""\x00\x00\x00\x00""\xFF\xFF\xFF\xFF"const_char_string; \
    str_t name = (str_t)&(((struct _str_s*)&_##name##_static)->chars)

#define static_const_str(name, const_char_string) \
    static char _##name##_static[sizeof(struct _str_s) + sizeof(const_char_string)] = \
    "STRS""\x00\x00\x00\x00""\xFF\xFF\xFF\xFF"const_char_string; \
    static const str_t name = (str_t)&(((struct _str_s*)&_##name##_static)->chars)

extern str_t str_empty;

str_t   str_dup(const char* s); /* call str_free() when done with returned str_t */
#define str_stack_dup(const_char_star) (str_t)(strcpy((char*)_str_stack_alloc((int)strlen((const_char_star))), (const_char_star)))
#define str_stack_concat(s1, s2) _str_concat(_str_stack_alloc((int)strlen(s1) + (int)strlen(s2)), s1, s2)
#define str_concat(s1, s2) _str_concat(_str_alloc((int)strlen(s1) + (int)strlen(s2)), s1, s2) /* call str_free() when done with returned str_t */
#define str_stack_substring(s, from_inclusive, to_exclusive) _str_substring(_str_stack_alloc(to_exclusive - from_inclusive), s, from_inclusive, to_exclusive)
#define str_substring(s, from_inclusive, to_exclusive) _str_substring(_str_alloc(to_exclusive - from_inclusive), s, from_inclusive, to_exclusive)
#define str_equ(s1, s2) ((s1) == null || (s2) == null ? (s1) == (s2) : ((s1) == (s2) || strcmp((s1), (s2)) == 0))
#define str_stack_printf(format, ...) _str_stack_printf(_str_stack_alloc(snprintf(null, 0, (format), __VA_ARGS__)), (format), __VA_ARGS__)
int     str_length(str_t s);  /* >= 0 */
int     str_hash(str_t s);    /* >  0 */
void    str_free(str_t s);
int     str_hashcode(const char* s);

/* usage:

foo.h

extern str_t bar;

foo.c

const_str(bar, "foo");        // external immutable constant string with the header
static_const_str(foo, "bar"); // `file local` a.k.a. static immutable constant string with the header

str_t foo() {
    return str_dup("foo");
}

void bar() {
    str_t s = foo();
    printf("%s", s);
    str_t local = str_stack_dup(s);
    str_free(s);
    printf("%s", local);
}

// IMPORTANT: DO NOT mix const char* and str_t!
// they are not interchangeable. 
// str_t can be used in posix APIs in place of const char* but not vice versa

static_const_str(static_string, "foo");

static void print_str(const char* label, str_t s) {
    printf("%s \"%s\" length=%d bar hash=%d", label, s, str_length(s), str_hash(s));
}

int main(int argc, char** argv) {
    print_str("static_string", static_string);
    str_t stack = str_stack_dup(static_string);
    print_str("stack", stack);
    str_t heap = str_dup(stack);
    print_str("heap", heap);
    str_free(heap); heap = null;
    str_t stack_concat = str_stack_concat("foo", "bar");
    print_str("stack_concat", stack_concat);
    str_t heap_concat = str_concat("foo", "bar");
    print_str("heap_concat", heap_concat);
    str_t stack_foo = str_stack_substring(stack_concat, 0, 3);
    print_str("stack_foo", stack_foo);
    str_t stack_bar = str_stack_substring(stack_concat, 3, 6);
    print_str("stack_bar", stack_bar);
    str_t heap_foo = str_substring(heap_concat, 0, 3);
    print_str("heap_foo", heap_foo);
    str_t heap_bar = str_substring(heap_concat, 3, 6);
    print_str("heap_bar", heap_bar);
    str_free(heap_foo); heap_foo = null;
    str_free(heap_bar); heap_bar = null;
    str_free(heap_concat); heap_concat = null;
    str_t hello = str_stack_printf("Hello %s", "World!");
    print_str("hello", hello);
    return 1;
}

*/

/* private stuff; do not use; don't look; like C++ private: 
   use macros definitions above instead */

#pragma pack(push, 4)

struct _str_s { /* DO NOT USE in client code; treat as opaque; subject to change w/o notice */
    int32_t  tag;
    int32_t  hash;
    int32_t  length;
    char     chars[sizeof(int32_t)]; // actual "length + 1"
};

#pragma pack(pop)

str_t   _str_alloc(int bytes);
str_t   _str_stack_init(struct _str_s* s, int number_of_characters);
str_t   _str_concat(str_t, const char* s1, const char* s2);
str_t   _str_substring(str_t t, const char* s, int from_inclusive, int to_exclusive);
#define _str_stack_alloc(number_of_characters) _str_stack_init((struct _str_s*)stack_alloc(number_of_characters + sizeof(struct _str_s)), number_of_characters)
str_t   _str_stack_printf(str_t t, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* STR_H */
