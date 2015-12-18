#include "std.h"
#include "str.h"
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

#if __BYTE_ORDER == __BIG_ENDIAN

enum {
    STR_TAG_CONST_STATIC = 'STRS',
    STR_TAG_CONST_STACK  = 'STRT',
    STR_TAG_CONST_HEAP   = 'STRH'
};

#else

enum {
    STR_TAG_CONST_STATIC = 'SRTS',
    STR_TAG_CONST_STACK  = 'TRTS',
    STR_TAG_CONST_HEAP   = 'HRTS'
};

#endif

const static int _str_header_size = (int)(((char*)&((struct _str_s*)0xF00)->chars) - (char*)((struct _str_s*)0xF00));

const_str(str_empty, "");

static inline str_t str_check_empty() {
    const struct _str_s* s = (const struct _str_s*)(str_empty - _str_header_size); 
    assert(s->length <= 0 && s->hash == 0 && s->chars[0] == 0); (void)s;
    return str_empty;
}

str_t _str_alloc(int number_of_characters) {
    assert(sizeof(struct _str_s) == 16);
    assert(_str_header_size == 12);
    assert(number_of_characters >= 0);
    if (number_of_characters == 0) {
        return str_check_empty();
    }
    char* p = (char*)malloc(number_of_characters + sizeof(struct _str_s));
    if (p != null) {
        struct _str_s* s = (struct _str_s*)p;
        s->tag = STR_TAG_CONST_HEAP;
        s->length = number_of_characters;
        s->hash   = 0;
        s->chars[0] = 0;
        return (str_t)s->chars;
    }
    return null;
}

void str_free(str_t p) {
    assert(sizeof(struct _str_s) == 16);
    assert(_str_header_size == 12);
    if (p != null) {
        if (p == str_empty) {
            str_check_empty(); 
            return;
        }
        struct _str_s* s = (struct _str_s*)(p - _str_header_size);
        assert(s->tag == STR_TAG_CONST_HEAP);
        s->tag = 0;
        free(s);
    }
}

str_t str_dup(const char* s) {
    int n = (int)strlen(s);
    if (n == 0) {
        return str_check_empty();
    }
    str_t p = _str_alloc(n);
    if (p != null) {
        struct _str_s* r = (struct _str_s*)(p - _str_header_size);
        r->length = n;
        memcpy(r->chars, s, n + 1);
    }
    return p;
}

str_t _str_stack_init(struct _str_s* s, int number_of_characters) {
    if (s == null) {
        return null;
    } else {
        s->tag = STR_TAG_CONST_STACK;
        s->hash = 0;
        s->length = number_of_characters;
        return s->chars;
    }
}

str_t _str_stack_printf(str_t t, const char* f, ...) {
    va_list vl;
    va_start(vl, f);
    if (t != null) {
        struct _str_s* s = (struct _str_s*)(t - _str_header_size);
        assert(s->tag == STR_TAG_CONST_STACK);
        int n = vsnprintf(s->chars, s->length + 1, f, vl);
        assert(n == s->length); (void)n;
    }    
    va_end(vl);
    return t;
}

str_t _str_concat(str_t t, const char* s1, const char* s2) {
    if (t != null) {
        struct _str_s* s = (struct _str_s*)(t - _str_header_size);
        assert(s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP);
        int n1 = (int)strlen(s1);
        int n2 = (int)strlen(s2);
        assert(s->length == n1 + n2);
        if ((s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP) && 
            s->length == n1 + n2 && s->hash == 0) {
            memcpy((char*)t, s1, n1);
            memcpy((char*)(t + n1), s2, n2 + 1);
        } else {
            return null;
        }
    }
    return t;
}

str_t _str_substring(str_t t, const char* cs, int from_inclusive, int to_exclusive) {
    assert(0 <= from_inclusive && from_inclusive <= to_exclusive && to_exclusive <= (int)strlen(cs));
    const int n = to_exclusive - from_inclusive; 
    if (n == 0) {
        return str_check_empty();
    }
    if (t != null) {
        struct _str_s* s = (struct _str_s*)(t - _str_header_size);
        assert(s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP);
        assert(s->hash == 0);
        assert(s->length == n);
        if ((s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP) && 
            s->length == n && s->hash == 0) {
            memcpy((char*)t, cs + from_inclusive, n);
            ((char*)t)[to_exclusive - from_inclusive] = 0;
        } else {
            return null;
        }
    }
    return t;
}

int str_length(str_t p) {
    assert(sizeof(struct _str_s) == 16);
    assert(_str_header_size == 12);
    if (p == str_empty) { return 0; }
    struct _str_s* s = (struct _str_s*)(p - _str_header_size);
    assert(s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP || s->tag == STR_TAG_CONST_STATIC);
    if (s->length < 0) {
        s->length = (int)strlen(s->chars);
    }
    return s->length;
}

int str_hashcode(const char* s) {
    int h = 0;
    while (*s != 0) {
        h = (h << 5) - h + *s++;
    }
    h = (h & 0x7FFFFFFF);
    return h == 0 ? 1 : h;
}

int str_hash(str_t p) {
    assert(sizeof(struct _str_s) == 16);
    assert(_str_header_size == 12);
    if (p == str_empty) {
        return 1; /* by contract hash > 0 */
    }
    struct _str_s* s = (struct _str_s*)(p - _str_header_size);
    assert(s->tag == STR_TAG_CONST_STACK || s->tag == STR_TAG_CONST_HEAP || s->tag == STR_TAG_CONST_STATIC);
    if (s->hash <= 0) { // http://www.javamex.com/tutorials/collections/hash_function_technical_2.shtml
        s->hash = str_hashcode(s->chars);
    }
    return s->hash;
}

#ifdef __cplusplus
}
#endif
