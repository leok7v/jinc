#pragma once
#ifndef STR2STR_H
#define STR2STR_H
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

#include "std.h"
#include "str.h"

/* str2str duplicates both key and value data and owns it.
   Keys MUST be str_t immutable hashed strings.
   Values just regular const char* or str_t. 
   Only not null strings are allowed as keys and values.
   null returned as a value for the absent key. 
   If you need to workaround this limitation: declare
     static_const_str(NULL_STRING, "");
   in your code and use it instead of "null"s.
*/

typedef void* str2str_t;

str2str_t   str2str_create(int capacity, boolean can_grow);
str_t   str2str_get(const str2str_t map, str_t key); /* returns "access" to the value - do not cache, do not free */
boolean str2str_put(str2str_t map, str_t, const char* v); /* false on memory overflow or bad parameters */
boolean str2str_put_all(str2str_t destination, const str2str_t source);
void    str2str_remove(str2str_t map, str_t key);
void    str2str_for_each(void* that, str2str_t map, void (*each)(void* t, str2str_t m, str_t key));
int     str2str_size(const str2str_t map);
void    str2str_clear(str2str_t map); 
void    str2str_dispose(str2str_t map);

str2str_t str2str_init_static(void* address_of_static_zero_initialized_memory, int bytes);

/* it is OK to remove current key-value inside "each()" call */

/* usage example:
    static_const_str(SUNDAY,    "Sunday");
    static_const_str(MONDAY,    "Monday");
    static_const_str(TUESDAY,   "Tuesday");
    static_const_str(WEDNESDAY, "Wednesday");
    static_const_str(THURSDAY,  "Thursday");
    static_const_str(FRIDAY,    "Friday");
    static_const_str(SATURDAY,  "Saturday");

    str2str_t m = str2str_create(16, true);
    str2str_put(m, SUNDAY,    "domingo");
    str2str_put(m, MONDAY,    "lunes");
    str2str_put(m, TUESDAY,   "martes");
    str2str_put(m, WEDNESDAY, "miércoles");
    str2str_put(m, THURSDAY,  "jueves");
    str2str_put(m, FRIDAY,    "viernes");
    str2str_put(m, SATURDAY,  "sábado");
    str2str_dispose(m);
*/

#ifdef __cplusplus
}
#endif

#endif /* STR2STR_H */
