#pragma once
#ifndef I2I_H
#define I2I_H
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

/* only positive keys and values are allowed.
   Zero returned as a value for the absent key.
   this is why _put(any_key, 0) is prohibited. */

typedef void* i2i_t;

i2i_t   i2i_create(int capacity, boolean can_grow); 
int64_t i2i_get(const i2i_t m, int64_t k); /* ~ 20 microseconds i7 2.4Ghz x64 release */
boolean i2i_put(i2i_t m, int64_t k, int64_t v); /* ~ 8 microseconds i7 2.4Ghz x64 release */
boolean i2i_put_all(i2i_t destination, const i2i_t source);
void    i2i_remove(i2i_t map, int64_t key);
int     i2i_size(const i2i_t m);
void    i2i_foreach(void* that, i2i_t map, void (*each)(void* t, i2i_t m, int64_t key));
void    i2i_clear(i2i_t map); /* remove all entries */
void    i2i_dispose(i2i_t m);

i2i_t   i2i_init_static(void* address_of_static_zero_initialized_memory, int bytes);

#ifdef __cplusplus
}
#endif

#endif /* I2I_H */
