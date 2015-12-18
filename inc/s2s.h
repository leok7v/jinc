#pragma once
#ifndef S2S_H
#define S2S_H
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
#include "std.h"

#ifdef __cplusplus
extern "C" {
#endif

/* s2s duplicates only value data and owns it.
   The keys are referenced and expected to be static or
   client code allocated and managed character pointers.
   Only not null strings are allowed as keys and values.
   null returned as a value for the absent key. 
   If you need to workaround this limitation: declare
     static const char* NULL_STRING = "";
   in your code and use it instead of "null"s.
*/

typedef void* s2s_t;

s2s_t   s2s_create(int capacity, boolean can_grow);
const char*   s2s_get(const s2s_t map, const char* key); /* returns "access" to the value - do not cache, do not free */
boolean s2s_put(s2s_t map, const char*, const char* v); /* false on memory overflow or bad parameters */
boolean s2s_put_all(s2s_t destination, const s2s_t source);
void    s2s_remove(s2s_t map, const char* key);
void    s2s_for_each(void* that, s2s_t map, void (*each)(void* t, s2s_t m, const char* key));
int     s2s_size(const s2s_t map);
void    s2s_clear(s2s_t map); 
void    s2s_dispose(s2s_t map);

s2s_t   s2s_init_static(void* address_of_static_zero_initialized_memory, int bytes);

/* it is OK to remove current key-value inside "each()" call */

/* usage example:
    static_const_str(SUNDAY,    "Sunday");
    static_const_str(MONDAY,    "Monday");
    static_const_str(TUESDAY,   "Tuesday");
    static_const_str(WEDNESDAY, "Wednesday");
    static_const_str(THURSDAY,  "Thursday");
    static_const_str(FRIDAY,    "Friday");
    static_const_str(SATURDAY,  "Saturday");

    s2s_t m = s2s_create(16, true);
    s2s_put(m, SUNDAY,    "domingo");
    s2s_put(m, MONDAY,    "lunes");
    s2s_put(m, TUESDAY,   "martes");
    s2s_put(m, WEDNESDAY, "miércoles");
    s2s_put(m, THURSDAY,  "jueves");
    s2s_put(m, FRIDAY,    "viernes");
    s2s_put(m, SATURDAY,  "sábado");
    s2s_dispose(m);
*/

#ifdef __cplusplus
}
#endif

#endif /* s2s_H */
