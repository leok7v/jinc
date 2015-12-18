#pragma once
#ifndef S2I_H
#define SI2_H
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

/* The keys are referenced and expected to be static or
   client code allocated and managed character pointers.
   Only not null and not empty strings are allowed as keys.
   Zero returned as a value for the absent key.
   this is why _put(any_key, 0) is prohibited too. */

typedef void* s2i_t;

s2i_t   s2i_create(int capacity, boolean can_grow); 
int64_t s2i_get(const s2i_t m, const char* k);
boolean s2i_put(s2i_t m, const char* k, int64_t v);
boolean s2i_put_all(s2i_t destination, const s2i_t source);
void    s2i_remove(s2i_t map, const char* key);
int     s2i_size(const s2i_t m);
void    s2i_foreach(void* that, s2i_t map, void (*each)(void* t, s2i_t m, int64_t key));
void    s2i_clear(s2i_t map); /* remove all entries */
void    s2i_dispose(s2i_t m);

s2i_t   s2i_init_static(void* address_of_static_zero_initialized_memory, int bytes); 

/*
s2i_init_static is useful for global static maps:

static char storage[16 * 1024];
s2i_t map;

void foo(const char* label) {
    if (map == null) {
        map = s2i_init_static(storage , sizeof(storage));
    }
    ....
}
*/

#ifdef __cplusplus
}
#endif

#endif /* S2I_H */
